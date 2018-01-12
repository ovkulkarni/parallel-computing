#!/bin/bash

gcc -o $1 $1.c -lm "${@:2}"
if [[ $? -eq 0 ]]
then
    ./$1
else
    echo "Compile Error!"
    exit 1
fi
