if (!exists("filename")) filename='orbit.txt'
if (!exists("outfilename")) outfilename='output.png'
set terminal png
set output outfilename
set tics font "Helvetica,9"
set size square
plot "earth.txt" w l notitle, filename u 2:3 w l notitle, filename u 4:5 w l notitle
