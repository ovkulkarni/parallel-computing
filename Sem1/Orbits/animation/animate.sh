#!/bin/bash

for i in `seq 600 600 691200`
do
    head -n $i apollo.txt > apollo$i.txt
    ./plot.sh apollo$i.txt output/apollo$i.png
    rm apollo$i.txt
done

