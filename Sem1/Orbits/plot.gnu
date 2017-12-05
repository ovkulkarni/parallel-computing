if (!exists("filename")) filename='orbit.txt'
if (!exists("outfilename")) outfilename='output.png'
set terminal png size 1600,1280
set output outfilename
set tics font "Helvetica,9"
set size square
plot "earth.txt" w l notitle, filename u 2:3 w l notitle
#
set size nosquare
plot filename u 1:5 w l title "vx",filename u 1:6 w l title "vy",filename u 1:7 w l title "vmag",0 w l ls 0 notitle
#
set size nosquare
plot filename u 1:4 w l title "r", 6.371e+6 w l ls 0 title "R"
