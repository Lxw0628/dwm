#!/bin/sh

# []= Tiled Layout	0
# ><> Floating Layout	1
# [M] Monocle Layout	2

cat <<EOF | xmenu | awk '{print $3}'
[]=  平铺             0
[M]  单窗口           1
[@]  螺旋             2
[\\]  递减螺旋         3
H[]  叠加             4
TTT  底部栈           5
===  水平底部栈       6
HHH  网格             7
###  行网格           8
---  水平网格         9
:::  无间隙网格       10
|M|  居中主窗口       11
>M>  居中浮动主窗口   12
><>  浮动             13
EOF
