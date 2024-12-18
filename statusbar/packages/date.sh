#!/usr/bin/env bash
# DATE 获取日期和时间的脚本

notify() {
	_cal=$(cal --color=always | sed 1,2d | sed 's/..7m/<b><span color="#ff79c6">/;s/..0m/<\/span><\/b>/')
	_todo=$(cat ~/.todo.md | sed 's/\(- \[x\] \)\(.*\)/<span color="#ff79c6">\1<s>\2<\/s><\/span>/' | sed 's/- \[[ |x]\] //')
	notify-send "󰃵 Calendar" "\n$_cal\n————————————————————\n$_todo" -r 9527
}

call_todo() {
	pid1=$(ps aux | grep 'st -t statusutil' | grep -v grep | awk '{print $2}')
	pid2=$(ps aux | grep 'st -t statusutil_todo' | grep -v grep | awk '{print $2}')
	mx=$(xdotool getmouselocation --shell | grep X= | sed 's/X=//')
	my=$(xdotool getmouselocation --shell | grep Y= | sed 's/Y=//')
	kill $pid1 && kill $pid2 || st -t statusutil_todo -g 50x15+$((mx - 200))+$((my + 20)) -c FGN -e nvim ~/.todo.md
}

click() {
	case "$1" in
	L) notify ;;
	R) call_todo ;;
	esac
}

case "$1" in
click) click $2 ;;
notify) notify ;;
esac
