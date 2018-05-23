#!/bin/bash
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib/
swig -python LVwp.i
gcc -c -fPIC -I/home/sag134/anaconda3/include/python3.6m  LVwp.c LVwp_wrap.c
ld -shared LVwp.o LVwp_wrap.o  /usr/local/MATLAB/R2017a/bin/glnxa64/libsundials_cvode.so.1 /usr/local/MATLAB/R2017a/bin/glnxa64/libsundials_nvecserial.so.0 -lm -o _LVwp.so
