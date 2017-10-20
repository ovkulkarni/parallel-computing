#
# Torbert, 29 September 2017
#
set terminal png
set output "overlay.png"
set key autotitle columnheader
set grid ytics lt 0
set xtics nomirror
plot "40x30.txt" u 1:2 w l, \
    "80x60.txt" u 1:2 w l, \
    "160x120.txt" u 1:2 w l
