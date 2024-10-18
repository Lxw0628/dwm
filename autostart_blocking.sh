#!/bin/bash

xrandr --output HDMI-2 --mode 1920x1080 --rate 120.00 --output eDP-1 --off


# killall -q bar.sh
sh ~/.config/dwm/scripts/bar.sh &
