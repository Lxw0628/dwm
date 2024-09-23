#!/bin/bash

# xrandr --output HDMI-2 --mode 1920x1080 --rate 120.00 --output eDP-1 --off
xrandr --output HDMI-2 --mode 1920x1080 --rate 120.00 --output eDP-1 --mode 1920x1080 --rate 60.00 --right-of HDMI-2

# killall -q bar.sh
sh ~/.config/dwm/scripts/bar.sh &
