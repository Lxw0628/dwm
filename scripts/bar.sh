#!/bin/bash

# ^c$var^ = fg color
# ^b$var^ = bg color

interval=0

# load colors
. ~/.config/dwm/statusbar/bar_themes/catppuccin

disk() {
  disk_can_use_kb="$(df | sed -n '5p' | awk '{print $4}')"
  disk_can_use_gb=$(echo "scale=2; $disk_can_use_kb/1024/1024" | bc)
  printf "^c$mauve^ /:"
  printf "^c$text^ $disk_can_use_gb GB"
}

cpu() {
  # cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)
  cpu_val=$(top -n 1 -b | sed -n '3p' | awk '{printf "%02d%", 100 - $8}')
  printf "^c$mauve^ CPU:"
  printf "^c$text^ $cpu_val%"
}

battery() {
  get_capacity="$(cat /sys/class/power_supply/BAT0/capacity)"
  printf "^c$text^   $get_capacity"
}

brightness() {
  printf "^c$mauve^ Bri:"
  # printf "^c$text^%.0f\n" $(cat /sys/class/backlight/*/brightness)
  printf "^c$text^%.0f\n" $(ddcutil getvcp 12 | awk '{print $9}' | awk -F, '{print $1}')
}

mem() {
  mem_total_kb=$(grep MemTotal /proc/meminfo | awk '{print $2}')
  mem_free_kb=$(grep MemAvailable /proc/meminfo | awk '{print $2}')
  mem_used_kb=$((mem_total_kb - mem_free_kb))
  mem_used_gb=$(echo "scale=2; $mem_used_kb/1024/1024" | bc)
  # printf "^c$text^^b$black^  "
  printf "^c$mauve^ Mem:"
  printf "^c$text^ $mem_used_gb GB"
}

wlan() {
	case "$(cat /sys/class/net/wl*/operstate 2>/dev/null)" in
	up) printf "^c$mauve^ 󰤨  ^d^%s" " ^c$text^Connected" ;;
	down) printf "^c$mauve^ 󰤭  ^d^%s" " ^c$text^Disconnected" ;;
	esac
}

clock() {
	# printf "^c$text^ 󱑆  "
	printf "^c$mauve^ $(date '+%Y-%m-%d %H:%M') "
}

while true; do

  [ $interval = 0 ] || [ $(($interval % 3600)) = 0 ] && updates=$(pkg_updates)
  interval=$((interval + 1))

  # sleep 1 && xsetroot -name "$updates $(battery) $(brightness) $(cpu) $(mem) $(wlan) $(clock)"
  sleep 1 && xsetroot -name "$(disk) $(cpu) $(mem) $(clock)"
done
