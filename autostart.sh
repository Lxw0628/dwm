#!/bin/bash


export LANG=zh_CN.UTF-8
export LANGUAGE=zh_CN:en_US

xrandr --output HDMI-2 --mode 1920x1080 --rate 120.00 --output eDP-1 --off

numlockx on
# ulimit -n 打开的文件描述符的最大个数
ulimit -n 4096
xrdb merge ~/.Xresources
xset r rate 200 50
# xbacklight -set 10 &
# feh --bg-fill --no-fehbg ~/Pictures/wallpapers/osu_bg/BG.jpg
feh --bg-fill --no-fehbg ~/Pictures/wallpapers/catppuccin/mocha.png
picom --config ~/.config/picom/picom.conf &
# picom --experimental-backends --config ~/.config/picom/picom.conf &

pkill -f /home/lxw0628/.config/dwm/scripts/bar.sh
sh ~/.config/dwm/scripts/bar.sh &

fcitx5 -d &
blueman-applet &
pa-applet &
nm-applet &
birdtray &
udiskie --tray &
clash-verge &
/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1 &
/usr/bin/dunst &
thunar --daemon &
xsettingsd &
xfce4-power-manager &

Snipaste &
