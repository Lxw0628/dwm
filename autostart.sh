#! /usr/bin/sh

killall xfce-polkit
killall feh
killall picom
killall flameshot
killall udiskie
killall fcitx5
# killall cfw
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

# /opt/clash-for-windows-chinese/cfw &
clash-verge &

fcitx5 &

thunar --daemon &

flameshot &

# udisksctl mount -b /dev/sda5 &

# picom --experimental-backends --config $HOME/.config/picom/picom.conf &

feh --bg-fill --randomize $HOME/Pictures/wallpapers/* &

/usr/lib/xfce-polkit/xfce-polkit &

xset r rate 250 50

udiskie -2 &
