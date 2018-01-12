#!/bin/bash

gnuplot -e "filename='$1'" -e "filename2='$2'" -e "filename3='$3'" -e "outfilename='$4'" plot3.gnu
