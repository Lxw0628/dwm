static const char* tempfile = "/home/lxw0628/.config/dwm/statusbar/temp";
static const char* packages = "/home/lxw0628/.config/dwm/statusbar/packages";
static const char* colors[][2] = {
    [Icons] = { "^c#f2cdcd^^b#1e1e2e^", NULL },
    [Wifi] = { "^c#f2cdcd^^b#1e1e2e^", "^c#f2cdcd^^b#1e1e2e^" },
    [Cpu] = { "^c#f2cdcd^^b#1e1e2e^", "^c#f2cdcd^^b#1e1e2e^" },
    [Mem] = { "^c#f2cdcd^^b#1e1e2e^", "^c#f2cdcd^^b#1e1e2e^" },
    [Date] = { "^c#f2cdcd^^b#1e1e2e^", "^c#f2cdcd^^b#1e1e2e^" },
    [Light] = { "^c#f2cdcd^^b#1e1e2e^", "^c#f2cdcd^^b#1e1e2e^" },
    [Vol] = { "^c#f2cdcd^^b#1e1e2e^", "^c#f2cdcd^^b#1e1e2e^" },
    [Bat] = { "^c#f2cdcd^^b#1e1e2e^", "^c#f2cdcd^^b#1e1e2e^" },
};
static const char* devs[] = {
    [Wired] = ":enp7s0f1",
    [Wireless] = ":wlp0s20f3",
    [Temperature] = "/sys/class/hwmon/hwmon5/temp1_input",
    [Capacity] = "/sys/class/power_supply/BAT0/capacity",
    [Charging] = "/sys/class/power_supply/BAT0/status",
    [Plug] = "/sys/class/power_supply/AC/online",
};
