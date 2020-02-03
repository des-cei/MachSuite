/*
Implementation based on algorithm described in:
The cache performance and optimizations of blocked algorithms
M. D. Lam, E. E. Rothberg, and M. E. Wolf
ASPLOS 1991
*/

#include "gemm.h"
#include "artico3.h"

//~ void bbgemm(TYPE m1[N], TYPE m2[N], TYPE prod[N]){
A3_KERNEL(a3in_t m1, a3in_t m2, a3out_t prod) {
    int i, k, j, jj, kk;
    int i_row, k_row;
    TYPE temp_x, mul;

    loopjj:for (jj = 0; jj < row_size; jj += block_size){
        loopkk:for (kk = 0; kk < row_size; kk += block_size){
            loopi:for ( i = 0; i < row_size; ++i){
                loopk:for (k = 0; k < block_size; ++k){
                    i_row = i * row_size;
                    k_row = (k  + kk) * row_size;
                    temp_x = a3tof(m1[i_row + k + kk]);
                    loopj:for (j = 0; j < block_size; ++j){
                        mul = temp_x * a3tof(m2[k_row + j + jj]);
                        //~ prod[i_row + j + jj] += ftoa3(mul); // Would not work due to data types
                        prod[i_row + j + jj] = ftoa3(a3tof(prod[i_row + j + jj]) + mul);
                    }
                }
            }
        }
    }
}
