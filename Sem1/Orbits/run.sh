#!/bin/bash

gcc -o $1 $1.c -lm 
if [[ $? -eq 0 ]]
then
    ./$1 "${@:2}"
else
    echo "Compile Error!"
    exit 1
fi
