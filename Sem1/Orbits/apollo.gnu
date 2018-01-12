if (!exists("filename")) filename='orbit.txt'
set terminal png size 1600,1280
set output outfilename
set multiplot layout 2, 1 title "Apollo Free Return"
set tics font "Helvetica,9"

set size nosquare
plot filename u 1:2 w l title "|v|" 
#
set size nosquare
plot filename u 1:3 w l title "r"
unset multiplot
