#!/bin/sh

# []= Tiled Layout	0
# ><> Floating Layout	1
# [M] Monocle Layout	2

cat <<EOF | xmenu | awk '{print $3}'
[]=  tile                   0
[M]  monocle                1
[@]  spiral                 2
[\\]  dwindle                3
H[]  deck                   4
TTT  bstack                 5
===  bstackhoriz            6
HHH  grid                   7
###  nrowgrid               8
---  horizgrid              9
:::  gaplessgrid            10
|M|  centeredmaster         11
>M>  centeredfloatingmaster 12
><>  floating               13
EOF
