set terminal png size 1600,1280
set output "apollo-graphs.png"
set multiplot layout 2, 2 title "Apollo Free Return"
set tics font "Helvetica,9"

set size nosquare
plot "apollo.txt" u 1:2 w l title "|v|" 
#
set size nosquare
plot "apollo.txt" u 1:3 w l title "r"

set size nosquare
plot "apollo-noreturn.txt" u 1:2 w l title "|v|" 
#
set size nosquare
plot "apollo-noreturn.txt" u 1:3 w l title "r"
unset multiplot
