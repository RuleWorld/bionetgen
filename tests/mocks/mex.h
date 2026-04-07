#ifndef MEX_H
#define MEX_H
#include <stdio.h>
#define mexPrintf printf
#define mexErrMsgTxt(msg) { printf("%s\n", msg); }
#endif
