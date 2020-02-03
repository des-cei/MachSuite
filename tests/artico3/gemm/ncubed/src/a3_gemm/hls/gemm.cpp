#include "gemm.h"
#include "artico3.h"

//~ void gemm( TYPE m1[N], TYPE m2[N], TYPE prod[N] ){
A3_KERNEL( a3in_t m1, a3in_t m2, a3out_t prod ){
    int i, j, k;
    int k_col, i_col;
    TYPE mult;

    outer:for(i=0;i<row_size;i++) {
        middle:for(j=0;j<col_size;j++) {
            i_col = i * col_size;
            TYPE sum = 0;
            inner:for(k=0;k<row_size;k++) {
                k_col = k * col_size;
                mult = a3tof(m1[i_col + k]) * a3tof(m2[k_col + j]);
                sum += mult;
            }
            prod[i_col + j]  = ftoa3(sum);
        }
    }
}
