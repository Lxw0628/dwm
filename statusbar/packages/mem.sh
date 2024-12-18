#!/usr/bin/env bash
# MEM

notify() {
	free_result=$(free -h)
	text="
	可用:\t $(echo "$free_result" | sed -n 2p | awk '{print $7}')
	用量:\t $(echo "$free_result" | sed -n 2p | awk '{print $3}')/$(echo "$free_result" | sed -n 2p | awk '{print $2}')
	swap:\t $(echo "$free_result" | sed -n 3p | awk '{print $3}')/$(echo "$free_result" | sed -n 3p | awk '{print $2}')
	"
	notify-send "󰟖 Memory" "$text" -r 9527
}

call_btop() {
	pid1=$(ps aux | grep 'st -t statusutil' | grep -v grep | awk '{print $2}')
	pid2=$(ps aux | grep 'st -t statusutil_mem' | grep -v grep | awk '{print $2}')
	mx=$(xdotool getmouselocation --shell | grep X= | sed 's/X=//')
	my=$(xdotool getmouselocation --shell | grep Y= | sed 's/Y=//')
	kill $pid1 && kill $pid2 || st -t statusutil_mem -g 82x25+$((mx - 328))+$((my + 20)) -c FGN -e btop
}

click() {
	case "$1" in
	L) notify ;;
	R) call_btop ;;
	esac
}

case "$1" in
click) click $2 ;;
notify) notify ;;
esac
