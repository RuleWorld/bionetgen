#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib/
swig -python MM.i
gcc -c -fPIC `pkg-config --cflags --libs python`  MM.c MM_wrap.c
ld -shared MM.o MM_wrap.o  /usr/local/MATLAB/R2017a/bin/glnxa64/libsundials_cvode.so.1 /usr/local/MATLAB/R2017a/bin/glnxa64/libsundials_nvecserial.so.0 -lm -o _MM.so
