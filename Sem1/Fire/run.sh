#!/bin/bash

mpirun -np $1 -q -hostfile ~/Parallel/hosts ./$2 | sort
