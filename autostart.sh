#! /usr/bin/sh

killall xfce-polkit
killall feh
killall picom
killall flameshot
killall udiskie
killall fcitx5
killall clash-verge
killall blueman-applet
killall nm-applet
killall dunst

source ~/.zprofile

sh $DWM/statusbar/statusbar.sh cron

# 通知服务器
# /usr/lib/notification-daemon-1.0/notification-daemon &

# 适用于DWM的极简主义通知守护程序
/usr/bin/dunst &

nm-applet &

blueman-applet &

clash-verge &

fcitx5 &

thunar --daemon &

flameshot &

# udisksctl mount -b /dev/sda5 &

picom --experimental-backends --config $HOME/.config/picom/picom.conf &

feh --bg-fill --randomize $HOME/Pictures/wallpapers/* &

/usr/lib/xfce-polkit/xfce-polkit &

xrandr --output HDMI-2 --mode 1920x1080 --rate 120.00 --output eDP-1 --off

xset r rate 250 50

bluetoothctl power off

udiskie -2 &
