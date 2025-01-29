#!/usr/bin/env bash

wp_change() {
	killall rechange_wallpaper.sh
	$DWM/scripts/rechange_wallpaper.sh &
}

powermenu() {
	~/.config/rofi/powermenu.sh
}

click() {
	case "$1" in
	L) wp_change ;;
	R) powermenu ;;
	esac
}

case "$1" in
click) click $2 ;;
notify) notify ;;
esac
