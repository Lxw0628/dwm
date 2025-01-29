#!/usr/bin/env bash
tempfile=~/.config/dwm/statusbar/temp
update() {
	light_text=$(xrandr --verbose | grep Brightness | awk '{print $2}' | head -n 1)
	light_text=$(awk "BEGIN {print $light_text * 100}")
	if [ "$light_text" -ge 95 ]; then
		light_icon="󰃠"
	elif [ "$light_text" -ge 75 ]; then
		light_icon="󰃝"
	elif [ "$light_text" -ge 50 ]; then
		light_icon="󰃟"
	elif [ "$light_text" -ge 25 ]; then
		light_icon="󰃞"
	else light_icon="󰃜"; fi
}

notify() {
	update
	notify-send -r 9527 -h int:value:$light_text -h string:hlcolor:#dddddd "$light_icon  Light"
}

toggle() {
	screen_names=($(xrandr --listmonitors | grep -oE '[^ ]+$' | tail -n +2))
	backlight=$(xrandr --verbose | grep Brightnes | awk '{print $2}' | head -n 1)
	backlight=$(awk -v n1=1.3 -v n2=$backlight 'BEGIN {print n1 - n2}')
	result=$(awk -v n1=$backlight -v n2=1.0 'BEGIN {if (n1 > n2) print 1; else if (n1 < n2) print -1; else print 0}')
	if [ $result -eq 1 ]; then
		backlight=1.0
	fi
	str=""
	for idx in ${screen_names[@]}; do
		str+=$(printf -- "--output %s --brightness %.2f " "$idx" "$backlight")
	done

	xrandr $str
}

click() {
	case "$1" in
	L) notify ;; # 仅通知
	R) toggle ;; # 切换亮度
	esac
	touch $tempfile
}

case "$1" in
click) click $2 ;;
notify) notify ;;
esac
