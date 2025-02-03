#include <X11/Xlib.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>

// 常量定义
#define CMD_BUF_SIZE 64
#define BAR_BUF_SIZE 256
#define FOREGROUND_COLOR_ICON "^c#f5c2e7^"
#define FOREGROUND_COLOR_TEXT "^c#cdd6f4^"
#define DISK_MOUNT "/"

// X连接缓存
static Display *display = NULL;

void get_disk(char output[CMD_BUF_SIZE]) {
  struct statvfs stat;
  if (statvfs(DISK_MOUNT, &stat) == 0) {
    unsigned long long avail = stat.f_bavail * stat.f_frsize;
    snprintf(output, CMD_BUF_SIZE, "%.2fG", avail / (1024.0 * 1024 * 1024));
  } else {
    strncpy(output, "N/A", CMD_BUF_SIZE);
  }
}

void get_cpu(char output[CMD_BUF_SIZE]) {
  static unsigned long long last_total = 0, last_idle = 0;
  FILE *fp = fopen("/proc/stat", "r");

  if (fp) {
    unsigned long long user, nice, system, idle, iowait;
    fscanf(fp, "cpu  %llu %llu %llu %llu %llu", &user, &nice, &system, &idle,
           &iowait);
    fclose(fp);

    unsigned long long total = user + nice + system + idle + iowait;
    unsigned long long idle_total = idle + iowait;

    if (last_total > 0) {
      float usage = 100.0 * (total - last_total - (idle_total - last_idle)) /
                    (total - last_total);
      snprintf(output, CMD_BUF_SIZE, "%02d%%", (int)(usage + 0.5));
    }

    last_total = total;
    last_idle = idle_total;
  } else {
    strncpy(output, "N/A", CMD_BUF_SIZE);
  }
}

void get_memory(char output[CMD_BUF_SIZE]) {
  long total = 0, avail = 0;
  FILE *fp = fopen("/proc/meminfo", "r");

  if (fp) {
    char line[CMD_BUF_SIZE];
    while (fgets(line, sizeof(line), fp)) {
      if (!total)
        sscanf(line, "MemTotal: %ld kB", &total);
      if (sscanf(line, "MemAvailable: %ld kB", &avail) == 1)
        break;
    }
    fclose(fp);
  }

  if (total > 0 && avail > 0) {
    snprintf(output, CMD_BUF_SIZE, "%.2fGiB",
             (total - avail) / 1048576.0); // 1024*1024 = 1048576
  } else {
    strncpy(output, "N/A", CMD_BUF_SIZE);
  }
}

void get_time(char output[CMD_BUF_SIZE]) {
  time_t now = time(NULL);
  struct tm *tm = localtime(&now);
  strftime(output, CMD_BUF_SIZE, "%b-%d %a %H:%M", tm);
}

void update_status() {
  char disk[CMD_BUF_SIZE], cpu[CMD_BUF_SIZE], mem[CMD_BUF_SIZE],
      time[CMD_BUF_SIZE];
  static char status[BAR_BUF_SIZE];

  get_disk(disk);
  get_cpu(cpu);
  get_memory(mem);
  get_time(time);

  // 分割时间为日期和时分秒部分
  char *date_part = time;
  char *time_part = "";
  char *last_space = strrchr(time, ' '); // 查找最后一个空格
  if (last_space) {
    *last_space = '\0';         // 分割字符串
    date_part = time;           // 前半部分（日期）
    time_part = last_space + 1; // 后半部分（时分秒）
  }

  snprintf(status, BAR_BUF_SIZE,
           " %s  %s%s "
           " %s  %s%s "
           " %s  %s%s "
           " %s%s %s%s ",
           FOREGROUND_COLOR_ICON, FOREGROUND_COLOR_TEXT, disk,
           FOREGROUND_COLOR_ICON, FOREGROUND_COLOR_TEXT, cpu,
           FOREGROUND_COLOR_ICON, FOREGROUND_COLOR_TEXT, mem,
           FOREGROUND_COLOR_ICON, date_part,
           FOREGROUND_COLOR_TEXT, time_part);

  XStoreName(display, DefaultRootWindow(display), status);
  XFlush(display);
}

int main() {
  /* setlocale(LC_TIME, "zh_CN.UTF-8"); */
  if (!(display = XOpenDisplay(NULL))) {
    fprintf(stderr, "Failed to open X display\n");
    return 1;
  }

  for (;; sleep(1)) {
    update_status();
  }

  // 理论上不会执行到这里
  XCloseDisplay(display);
  return 0;
}
