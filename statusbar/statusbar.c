#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

enum {
  Icons,
  Wifi,
  Cpu,
  Mem,
  Date,
  Light,
  Vol,
  Bat,
};
enum {
  Wired,
  Wireless,
  Temperature,
  Capacity,
  Charging,
  Plug,
};

#include "statusbar.h"

typedef unsigned long long ullong;

static char _icons[256] = "", _wifi[256] = "", _cpu[256] = "", _mem[256] = "",
            _date[256] = "", _light[256] = "", _vol[256] = "", _bat[256] = "";

static void refresh();
static void cron();
static void click(char *signal, char *button);
static void icons();
static void mem();
static void cpu();
static void wifi();
static void date();
static void light();
static void vol();
static void bat();
static int pscanf(const char *path, const char *fmt, ...);
static int cscanf(const char *cmd, const char *fmt, ...);

int pscanf(const char *path, const char *fmt, ...) {
  FILE *fp = NULL;
  va_list ap;
  int n;

  if (!(fp = fopen(path, "r"))) {
    return -1;
  }
  va_start(ap, fmt);
  n = vfscanf(fp, fmt, ap);
  va_end(ap);
  fclose(fp);

  return (n == EOF) ? -1 : n;
}

void icons() {
  char buffer[256];
  char icon[20] = "󰊠";

  FILE *fp = NULL;
  fp = popen("nmcli dev | grep 'ap0'", "r");
  if (fp == NULL) {
    return;
  }
  if (fgets(buffer, sizeof(buffer), fp) != NULL) {
    strcat(icon, " 󱜠");
  }
  pclose(fp);

  sprintf(_icons, "^sicons^%s %s ", colors[Icons][0], icon);
}

void wifi() {
  char buffer[256];
  char connected_network[20] = "";
  char icon[5] = "󰕡";
  int is_wired = 0, is_wireless = 0;

  FILE *fp = NULL;
  fp = popen("nmcli -t -f NAME,DEVICE,STATE connection show --active", "r");
  if (fp == NULL) {
    return;
  }
  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    if (devs[Wired] != NULL && strstr(buffer, devs[Wired]) != NULL) {
      is_wired = 1;
      char *token = strtok(buffer, ":");
      strncpy(connected_network, token, sizeof(connected_network) - 1);
      connected_network[sizeof(connected_network) - 1] = '\0';
    }
    if (devs[Wireless] != NULL && strstr(buffer, devs[Wireless]) != NULL) {
      is_wireless = 1;
      char *token = strtok(buffer, ":");
      strncat(connected_network, token, sizeof(connected_network) - 1);
      connected_network[sizeof(connected_network) - 1] = '\0';
    }
  }
  pclose(fp);

  if (is_wired) {
    strncpy(icon, "󰕡", sizeof(icon) - 1);
  } else if (is_wireless) {
    strncpy(icon, "󰖩", sizeof(icon) - 1);
  } else {
    strncpy(icon, "󱈨", sizeof(icon) - 1);
  }

  sprintf(_wifi, "^swifi^%s %s%s %s ", colors[Wifi][0], icon, colors[Wifi][1],
          connected_network);
}

void cpu() {
  char *icon = "󰍛";

  static long double a[7];
  long double b[7], sum;

  memcpy(b, a, sizeof(b));
  if (pscanf("/proc/stat", "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf", &a[0], &a[1],
             &a[2], &a[3], &a[4], &a[5], &a[6]) != 7)
    return;

  if (b[0] == 0)
    return;

  sum = (b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6]) -
        (a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6]);

  int usage = (int)(100.0 *
                    ((b[0] + b[1] + b[2] + b[5] + b[6]) -
                     (a[0] + a[1] + a[2] + a[5] + a[6])) /
                    sum);

  int temperature;
  if (pscanf(devs[Temperature], "%d", &temperature) != 1) {
    return;
  }
  temperature /= 1000;

  sprintf(_cpu, "^scpu^%s %s%s %02d%% %02d°C ", colors[Cpu][0], icon,
          colors[Cpu][1], usage, temperature);
}

void mem() {
  char buffer[256];
  char *icon = "󰟖";
  unsigned long mem_total = 0, mem_available = 0;

  FILE *fp = NULL;
  fp = fopen("/proc/meminfo", "r");
  if (fp == NULL) {
    return;
  }
  while (fgets(buffer, sizeof(buffer), fp)) {
    if (sscanf(buffer, "MemTotal:%lu kB", &mem_total) == 1) {
      continue;
    }
    if (sscanf(buffer, "MemAvailable:%lu kB", &mem_available) == 1) {
      continue;
    }
  }
  fclose(fp);

  int mem_persent = ((double)mem_total - mem_available) / mem_total * 100;

  sprintf(_mem, "^smem^%s %s%s %02d%% ", colors[Mem][0], icon, colors[Mem][1],
          mem_persent);
}

void date() {
  time_t time_now;
  time(&time_now);
  struct tm *time_info = localtime(&time_now);
  char time_n[6], icon[5] = "󱑖";
  int time_h;

  strftime(time_n, sizeof(time_n), "%H:%M", time_info);
  sscanf(time_n, "%d", &time_h);
  time_h %= 12;

  switch (time_h) {
  case 0:
    strncpy(icon, "󱑖", sizeof(icon) - 1);
    break;
  case 1:
    strncpy(icon, "󱑋", sizeof(icon) - 1);
    break;
  case 2:
    strncpy(icon, "󱑌", sizeof(icon) - 1);
    break;
  case 3:
    strncpy(icon, "󱑍", sizeof(icon) - 1);
    break;
  case 4:
    strncpy(icon, "󱑎", sizeof(icon) - 1);
    break;
  case 5:
    strncpy(icon, "󱑏", sizeof(icon) - 1);
    break;
  case 6:
    strncpy(icon, "󱑐", sizeof(icon) - 1);
    break;
  case 7:
    strncpy(icon, "󱑑", sizeof(icon) - 1);
    break;
  case 8:
    strncpy(icon, "󱑒", sizeof(icon) - 1);
    break;
  case 9:
    strncpy(icon, "󱑓", sizeof(icon) - 1);
    break;
  case 10:
    strncpy(icon, "󱑔", sizeof(icon) - 1);
    break;
  case 11:
    strncpy(icon, "󱑕", sizeof(icon) - 1);
    break;
  default:
    strncpy(icon, "󱑖", sizeof(icon) - 1);
  }

  sprintf(_date, "^sdate^%s %s%s %s ", colors[Date][0], icon, colors[Date][1],
          time_n);
}

void vol() {
  char buffer[256];
  char icon[5] = "󰕾";
  unsigned int vol = 0;
  int muted = 0;

  FILE *fp = NULL;
  fp = popen("amixer get Master", "r");
  if (fp == NULL) {
    return;
  }
  while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
    char *vol_pr = strstr(buffer, "[");
    if (vol_pr != NULL) {
      vol_pr++;
      vol = atoi(vol_pr);
    }
  }
  pclose(fp);

  if (strstr(buffer, "[off]") != NULL) {
    muted = 1;
  }
  if (muted) {
    strncpy(icon, "󰝟", sizeof(icon) - 1);
    sprintf(_vol, "^svol^%s %s%s -- ", colors[Vol][0], icon, colors[Vol][1]);
  } else {
    if (vol > 50) {
      strncpy(icon, "󰕾", sizeof(icon) - 1);
    } else if (vol > 0) {
      strncpy(icon, "󰖀", sizeof(icon) - 1);
    } else {
      strncpy(icon, "󰕿", sizeof(icon) - 1);
    }
    sprintf(_vol, "^svol^%s %s%s %d%% ", colors[Vol][0], icon, colors[Vol][1],
            vol);
  }
}

void bat() {
  char buffer[256];
  int capacity = -1;

  FILE *fp = NULL;
  fp = fopen(devs[Capacity], "r");
  if (fp == NULL) {
    return;
  }
  if (fgets(buffer, sizeof(buffer), fp) != NULL) {
    capacity = atoi(buffer);
  }
  fclose(fp);

  int charging = 0;
  fp = fopen(devs[Charging], "r");
  if (fp == NULL) {
    return;
  }
  if (fgets(buffer, sizeof(buffer), fp) != NULL) {
    size_t len = strlen(buffer);
    if (len > 1 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }
    if (!strcmp(buffer, "Charging")) {
      charging = 1;
    }
  }
  fclose(fp);

  int plugin = 0;
  fp = fopen(devs[Plug], "r");
  if (fp == NULL) {
    return;
  }
  if (fgets(buffer, sizeof(buffer), fp) != NULL) {
    if (buffer[0] == '1') {
      plugin = 1;
    }
  }
  fclose(fp);

  char icon[5] = "󰁹";
  if (plugin && !charging) {
    strncpy(icon, "󱘖", sizeof(icon) - 1);
  } else if (capacity >= 95) {
    strncpy(icon, charging ? "󰂅" : "󰁹", sizeof(icon) - 1);
  } else if (capacity >= 90) {
    strncpy(icon, charging ? "󰂋" : "󰂂", sizeof(icon) - 1);
  } else if (capacity >= 80) {
    strncpy(icon, charging ? "󰂊" : "󰂁", sizeof(icon) - 1);
  } else if (capacity >= 70) {
    strncpy(icon, charging ? "󰢞" : "󰂀", sizeof(icon) - 1);
  } else if (capacity >= 60) {
    strncpy(icon, charging ? "󰂉" : "󰁿", sizeof(icon) - 1);
  } else if (capacity >= 50) {
    strncpy(icon, charging ? "󰢝" : "󰁾", sizeof(icon) - 1);
  } else if (capacity >= 40) {
    strncpy(icon, charging ? "󰂈" : "󰁽", sizeof(icon) - 1);
  } else if (capacity >= 30) {
    strncpy(icon, charging ? "󰂇" : "󰁼", sizeof(icon) - 1);
  } else if (capacity >= 20) {
    strncpy(icon, charging ? "󰂆" : "󰁻", sizeof(icon) - 1);
  } else if (capacity >= 10) {
    strncpy(icon, charging ? "󰢜" : "󰁺", sizeof(icon) - 1);
  } else {
    strncpy(icon, charging ? "󰢟" : "󰂃", sizeof(icon) - 1);
  }

  sprintf(_bat, "^sbat^%s %s%s %2d%% ", colors[Bat][0], icon, colors[Bat][1],
          capacity);
}

void refresh() {
  char status[2048] = "";
  sprintf(status, "%s%s%s%s%s%s%s", _icons, _wifi, _cpu, _mem, _date, _vol,
          _bat);
  char cmd[2048] = "";
  snprintf(cmd, sizeof(cmd), "xsetroot -name \"%s\"", status);
  system(cmd);
}

void cron() {
  FILE *fp = NULL;
  int i = 0;
  while (1) {
    fp = fopen(tempfile, "r");
    if (fp != NULL || !i) {
      icons(), wifi(), vol(), cpu(), mem(), bat();
      if (fp != NULL) {
        fclose(fp);
        remove(tempfile);
      }
    }
    date();
    refresh();
    i = (i + 1) % 10;
    usleep(100000);
  }
}

void click(char *signal, char *button) {
  char script[100] = "";
  if (!strcmp(signal, "icons")) {
    sprintf(script, "bash %s/icons.sh click %s", packages, button);
  } else if (!strcmp(signal, "wifi")) {
    sprintf(script, "bash %s/wifi.sh click %s", packages, button);
  } else if (!strcmp(signal, "cpu")) {
    sprintf(script, "bash %s/cpu.sh click %s", packages, button);
  } else if (!strcmp(signal, "mem")) {
    sprintf(script, "bash %s/mem.sh click %s", packages, button);
  } else if (!strcmp(signal, "date")) {
    sprintf(script, "bash %s/date.sh click %s", packages, button);
  } else if (!strcmp(signal, "vol")) {
    sprintf(script, "bash %s/vol.sh click %s", packages, button);
  } else if (!strcmp(signal, "bat")) {
    sprintf(script, "bash %s/bat.sh click %s", packages, button);
  }
  system(script);
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (argc == 2 && strcmp(argv[1], "cron") == 0) {
      cron();
    } else if (argc == 3) {
      click(argv[1], argv[2]);
    }
  }

  return 0;
}
