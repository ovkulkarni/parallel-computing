set terminal png
set output "cores.png"
set tics font "Helvetica,9"
set size square
plot "cores.txt" u 1:2 w l notitle
