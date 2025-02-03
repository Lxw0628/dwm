#!/bin/bash

# 单例启动函数：仅在进程不存在时运行命令
run_once() {
  if ! pgrep -x "$1" >/dev/null; then
    shift
    "$@" &
  fi
}

# === 系统级单例程序（全局只启动一次） ===
run_once xsettingsd xsettingsd
run_once fcitx5 fcitx5 -d
run_once polkit-gnome /usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1
run_once xfce4-power-manager xfce4-power-manager
run_once udiskie udiskie --tray
run_once dunst dunst
run_once warpd warpd

# === 显示器/硬件设置（无需重复执行） ===
xset -b off
numlockx on
xrdb merge ~/.Xresources
xset r rate 200 50
xrandr --output HDMI-2 --mode 1920x1080 --rate 120.00 --output eDP-1 --off
"$HOME/Scripts/wallpapers.sh" "autochange"

# === dwm 相关程序（需随 dwm 重启） ===
pkill dwm_statusbar  # 先终止旧状态栏
dwm_statusbar &      # 启动新状态栏

# 若 picom 已存在，先终止旧进程
pkill picom
picom --config ~/.config/picom/picom.conf &

# 托盘程序（确保只启动一次）
run_once nm-applet nm-applet
# run_once blueman-applet blueman-applet
run_once pa-applet pa-applet

# === 其他程序 ===
run_once clash-verge clash-verge
run_once Snipaste Snipaste
run_once otd-daemon otd-daemon
