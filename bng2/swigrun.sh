#!/bin/bash
swig -python example.i
gcc -c -fPIC `pkg-config --cflags --libs python`  example.c example_wrap.c
 ld -shared example.o example_wrap.o  /usr/local/MATLAB/R2017a/bin/glnxa64/libsundials_cvode.so.1 /usr/local/MATLAB/R2017a/bin/glnxa64/libsundials_nvecserial.so.0 -lm -o _example.so
