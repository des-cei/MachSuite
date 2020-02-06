/*
Based on algorithm described here:
http://www.cs.berkeley.edu/~mhoemmen/matrix-seminar/slides/UCB_sparse_tutorial_1.pdf
*/

#include "spmv.h"

//~ void spmv(TYPE val[NNZ], int32_t cols[NNZ], int32_t rowDelimiters[N+1], TYPE vec[N], TYPE out[N]){
void spmv(TYPE val[NNZ], int32_t cols[NNZ], int32_t rowDelimiters[NNZ], TYPE vec[NNZ], TYPE out[NNZ]){
#pragma HLS interface s_axilite port=return
#pragma HLS INTERFACE m_axi depth=1666 port=val offset=slave
#pragma HLS INTERFACE m_axi depth=1666 port=cols offset=slave
#pragma HLS INTERFACE m_axi depth=495 port=rowDelimiters offset=slave
#pragma HLS INTERFACE m_axi depth=494 port=vec offset=slave
#pragma HLS INTERFACE m_axi depth=494 port=out offset=slave
    int i, j;
    TYPE sum, Si;

    spmv_1 : for(i = 0; i < N; i++){
        sum = 0; Si = 0;
        int tmp_begin = rowDelimiters[i];
        int tmp_end = rowDelimiters[i+1];
        spmv_2 : for (j = tmp_begin; j < tmp_end; j++){
            Si = val[j] * vec[cols[j]];
            sum = sum + Si;
        }
        out[i] = sum;
    }
}


