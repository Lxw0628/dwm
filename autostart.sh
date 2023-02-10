#! /usr/bin/sh

sh $DWM/statusbar/statusbar.sh cron

# 通知服务器
# /usr/lib/notification-daemon-1.0/notification-daemon &

# 适用于DWM的极简主义通知守护程序
killall dunst
/usr/bin/dunst &

source ~/.zprofile

xset r rate 250 50

killall nm-applet
nm-applet &

killall blueman-applet
blueman-applet &

killall cfw
$HOME/cfw/cfw &

killall fcitx5
fcitx5 &

killall flameshot
flameshot &

killall picom 
picom --experimental-backends --config $HOME/.config/picom/picom.conf &

# feh --bg-fill /run/media/lxw/数据/图片/电脑壁纸/code4k.png &
killall feh
feh --bg-fill --randomize $HOME/Pictures/wallpapers/* &

killall xfce-polkit
/usr/lib/xfce-polkit/xfce-polkit

xmodmap -e "keycode 108 = Super_L"  # reassign Alt_R to Super_L
xmodmap -e "remove mod1 = Super_L"  # make sure X keeps it out of the mod1 group

