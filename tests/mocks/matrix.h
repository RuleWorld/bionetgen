#ifndef MATRIX_H
#define MATRIX_H
#include <stddef.h>
typedef struct mxArray_tag mxArray;
#ifdef __cplusplus
extern "C" {
#endif
double* mxGetPr(const mxArray *pm);
size_t mxGetM(const mxArray *pm);
size_t mxGetN(const mxArray *pm);
mxArray* mxCreateDoubleMatrix(size_t m, size_t n, int ComplexFlag);
#ifdef __cplusplus
}
#endif
#define mxREAL 0
#endif
