#! /bin/bash
# LIGHT 亮度

source ~/.zprofile

this=_light
icon_color="^c#FFFFFF^^b#3333330xc0^"
text_color="^c#FFFFFF^^b#3333330xc0^"
signal=$(echo "^s$this^" | sed 's/_//')

update() {
    light_icon="☀"
    light_text="$(xbacklight -get)"

    icon=" $light_icon "
    text="$light_text% "

    sed -i '/^export '$this'=.*$/d' $DWM/statusbar/temp
    printf "export %s='%s%s%s%s%s'\n" $this "$signal" "$icon_color" "$icon" "$text_color" "$text" >> $DWM/statusbar/temp
}

notify() {
    update
    notify-send -r 9527 "$light_icon 亮度" "\n屏幕亮度：$text"
}

click() {
    case "$1" in
        L) notify            ;;
        U) xbacklight -inc 1 ;;
        D) xbacklight -dec 1 ;;
    esac
}

case "$1" in
    click) click $2 ;;
    notify) notify ;;
    *) update ;;
esac
