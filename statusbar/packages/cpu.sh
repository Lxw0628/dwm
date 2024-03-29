#! /bin/bash
# CPU 获取CPU使用率和温度的脚本

source ~/.zprofile

this=_cpu
# icon_color="^c#FFFFFF^^b#333333^"
text_color="^c#FFFFFF^^b#333333^"
signal=$(echo "^s$this^" | sed 's/_//')

update() {
	# cpu_icon="閭"
	# cpu_text=$(top -n1 -b | sed -n '3p' | awk '{printf "%02d%", 100 - $8}')
	cpu_text=$(python3 -c 'import psutil;print(psutil.cpu_percent(1))')
	temp_text=$(sensors coretemp-isa-0000 | head -3 | tail -1 | awk -F [+°C] '{print int($2)}')
	# icon=" $cpu_icon "
	text="CPU:$cpu_text%|$temp_text°C "
	# text="$cpu_text "

	sed -i '/^export '$this'=.*$/d' $DWM/statusbar/temp
	# printf "export %s='%s%s%s%s%s'\n" $this "$signal" "$icon_color" "$icon" "$text_color" "$text" >> $DWM/statusbar/temp
	printf "export %s='%s%s%s'\n" $this "$signal" "$text_color" "$text" >>$DWM/statusbar/temp
}

notify() {
	notify-send "CPU tops" "\n$(ps axch -o cmd:15,%cpu --sort=-%cpu | head)\\n\\n(100% per core)" -r 9527
}

call_btop() {
	pid1=$(ps aux | grep 'st -t statusutil' | grep -v grep | awk '{print $2}')
	pid2=$(ps aux | grep 'st -t statusutil_cpu' | grep -v grep | awk '{print $2}')
	mx=$(xdotool getmouselocation --shell | grep X= | sed 's/X=//')
	my=$(xdotool getmouselocation --shell | grep Y= | sed 's/Y=//')
	kill $pid1 && kill $pid2 || st -t statusutil_cpu -g 82x25+$((mx - 328))+$((my + 20)) -c noborder -e btop
}

click() {
	case "$1" in
	L) notify ;;
	M) ;;
	R) call_btop ;;
	U) ;;
	D) ;;
	esac
}

case "$1" in
click) click $2 ;;
notify) notify ;;
*) update ;;
esac
