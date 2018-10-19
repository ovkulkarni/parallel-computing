#!/bin/bash

gcc -o $1 $1.c -lm -fopenmp
if [[ $? -eq 0 ]]
then
    ./$1 "${@:2}"
    if [[ -f $1.ppm ]]
    then
        display $1.ppm
    fi
else
    echo "Compile Error!"
    exit 1
fi
