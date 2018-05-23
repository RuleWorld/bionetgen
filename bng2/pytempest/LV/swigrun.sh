#!/bin/bash
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib/
swig -python LV.i
gcc -c -fPIC -I/home/sag134/anaconda3/include/python3.6m  LV.c LV_wrap.c
ld -shared LV.o LV_wrap.o  /usr/local/MATLAB/R2017a/bin/glnxa64/libsundials_cvode.so.1 /usr/local/MATLAB/R2017a/bin/glnxa64/libsundials_nvecserial.so.0 -lm -o _LV.so
