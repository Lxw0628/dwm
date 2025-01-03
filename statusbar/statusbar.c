#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <time.h>
#include <unistd.h>

#define CMD_BUF_SIZE 128
#define BAR_BUF_SIZE 256

// Function to get disk usage

void get_disk(const char *path, char *output) {
  struct statvfs stat;

  // 检查路径有效性并获取文件系统信息
  if (statvfs(path, &stat) != 0) {
    snprintf(output, CMD_BUF_SIZE, "N/A");
    return;
  }

  // 计算可用空间 (以 GB 为单位)
  unsigned long long disk_free_kb = stat.f_bavail * stat.f_frsize / 1024;
  float disk_free_gb = disk_free_kb / 1024.0 / 1024.0;

  snprintf(output, CMD_BUF_SIZE, "%.2f GB", disk_free_gb);
}

// Function to get CPU usage
void get_cpu(char *output) {
  FILE *fp =
      popen("top -bn1 | sed -n '3p' | awk '{printf \"%02d\", 100 - $8}'", "r");
  if (!fp) {
    snprintf(output, CMD_BUF_SIZE, "N/A");
    return;
  }

  char buffer[CMD_BUF_SIZE];
  if (fgets(buffer, CMD_BUF_SIZE, fp)) {
    snprintf(output, CMD_BUF_SIZE, "%s%%", buffer);
  } else {
    snprintf(output, CMD_BUF_SIZE, "N/A");
  }
  pclose(fp);
}

// Function to get memory usage
void get_memory(char *output) {
  FILE *fp = fopen("/proc/meminfo", "r");
  if (!fp) {
    snprintf(output, CMD_BUF_SIZE, "N/A");
    return;
  }

  long mem_total = 0, mem_free = 0;
  char line[CMD_BUF_SIZE];
  while (fgets(line, CMD_BUF_SIZE, fp)) {
    if (sscanf(line, "MemTotal: %ld kB", &mem_total) == 1 ||
        sscanf(line, "MemAvailable: %ld kB", &mem_free) == 1) {
      continue;
    }
  }
  fclose(fp);

  long mem_used = mem_total - mem_free;
  float mem_used_gb = mem_used / 1024.0 / 1024.0;
  snprintf(output, CMD_BUF_SIZE, "%.2f GB", mem_used_gb);
}

// Function to get current time
void get_time(char *output) {
  time_t now = time(NULL);
  struct tm *local = localtime(&now);
  if (local) {
    strftime(output, CMD_BUF_SIZE, "%Y-%m-%d %H:%M", local);
  } else {
    snprintf(output, CMD_BUF_SIZE, "Time: N/A");
  }
}

// Function to set X root window name
void set_status(const char *status) {
  Display *display = XOpenDisplay(NULL);
  if (!display) {
    fprintf(stderr, "Cannot open display\n");
    return;
  }

  Window root = DefaultRootWindow(display);
  XStoreName(display, root, status);
  XCloseDisplay(display);
}

int main() {
  char disk[CMD_BUF_SIZE], cpu[CMD_BUF_SIZE], mem[CMD_BUF_SIZE],
      time[CMD_BUF_SIZE];
  char status[BAR_BUF_SIZE];

  while (1) {
    get_disk("/", disk);
    get_cpu(cpu);
    get_memory(mem);
    get_time(time);

    snprintf(status, BAR_BUF_SIZE,
             "^c#f5c2e7^Disk: ^c#cdd6f4^%s "
             "^c#f5c2e7^CPU: ^c#cdd6f4^%s "
             "^c#f5c2e7^Mem: ^c#cdd6f4^%s "
             "^c#f5c2e7^%s",
             disk, cpu, mem, time);
    set_status(status);

    sleep(1); // Update every second
  }

  return 0;
}
