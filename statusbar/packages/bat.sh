#!/usr/bin/env bash

notify() {
	bat_text=$(cat /sys/class/power_supply/BAT0/capacity)
	if [ "$charging" ]; then _time=""; fi
	notify-send "󰁹 Battery" "\n剩余: $bat_text%\n$_time" -r 9527
}

click() {
	case "$1" in
	L) notify ;;
	R) killall xfce4-power-manager-settings || setsid xfce4-power-manager-settings ;;
	esac
}

case "$1" in
click) click $2 ;;
notify) notify ;;
esac
