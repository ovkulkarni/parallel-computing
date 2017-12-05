#
# Torbert, 29 September 2017
#
set terminal png
set output "graph-mpi.png"
set key autotitle columnheader
set grid ytics lt 0
set xtics nomirror
plot "graph-mpi.txt" u 1:2 w l
