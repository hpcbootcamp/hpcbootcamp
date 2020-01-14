#!/bin/bash

#SBATCH -N 1
#SBATCH -t 00:10
#SBATCH –-reservation=bootcamp

export OMP_NUM_THREADS=1

cd $HOME/hpcbootcamp

time ./pgm
