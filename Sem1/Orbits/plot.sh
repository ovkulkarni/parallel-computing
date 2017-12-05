#!/bin/bash

gnuplot -e "filename='$1'" -e "outfilename='$2'" plot.gnu
