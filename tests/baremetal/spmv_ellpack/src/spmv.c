/*
Based on algorithm described here:
http://www.cs.berkeley.edu/~mhoemmen/matrix-seminar/slides/UCB_sparse_tutorial_1.pdf
*/

#include "spmv.h"

//~ void ellpack(TYPE nzval[N*L], int32_t cols[N*L], TYPE vec[N], TYPE out[N])
void ellpack(TYPE nzval[N*L], int32_t cols[N*L], TYPE vec[N*L], TYPE out[N*L])
{
#pragma HLS interface s_axilite port=return
#pragma HLS INTERFACE m_axi depth=4940 port=nzval offset=slave
#pragma HLS INTERFACE m_axi depth=4940 port=cols offset=slave
#pragma HLS INTERFACE m_axi depth=494 port=vec offset=slave
#pragma HLS INTERFACE m_axi depth=494 port=out offset=slave
    int i, j;
    TYPE Si;

    ellpack_1 : for (i=0; i<N; i++) {
        TYPE sum = out[i];
        ellpack_2 : for (j=0; j<L; j++) {
                Si = nzval[j + i*L] * vec[cols[j + i*L]];
                sum += Si;
        }
        out[i] = sum;
    }
}
