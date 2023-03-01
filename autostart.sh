#! /usr/bin/sh

killall xfce-polkit
killall feh
killall picom 
killall flameshot
killall udiskie
killall fcitx5
killall cfw
killall blueman-applet
killall nm-applet
killall dunst

source ~/.zprofile

xrandr --output HDMI-2 --mode 1920x1080 --rate 120.00 --output eDP-1 --mode 1920x1080 --rate 60.00 --right-of HDMI-2

sh $DWM/statusbar/statusbar.sh cron

# 通知服务器
# /usr/lib/notification-daemon-1.0/notification-daemon &

# 适用于DWM的极简主义通知守护程序
/usr/bin/dunst &

xset r rate 250 50

nm-applet &

blueman-applet &
# bluetoothctl power off

$HOME/.local/share/cfw/cfw &

fcitx5 &

udiskie &

flameshot &

picom --experimental-backends --config $HOME/.config/picom/picom.conf &

# feh --bg-fill /run/media/lxw/数据/图片/电脑壁纸/code4k.png &
feh --bg-fill --randomize $HOME/Pictures/wallpapers/* &
# feh --bg-fill --randomize $HOME/Pictures/wallpapers2/* &

/usr/lib/xfce-polkit/xfce-polkit

xmodmap -e "keycode 108 = Super_L"  # reassign Alt_R to Super_L
xmodmap -e "remove mod1 = Super_L"  # make sure X keeps it out of the mod1 group

