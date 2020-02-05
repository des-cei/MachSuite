/*
Based on algorithm described here:
http://www.cs.berkeley.edu/~mhoemmen/matrix-seminar/slides/UCB_sparse_tutorial_1.pdf
*/

#include "spmv.h"
#include "artico3.h"

//~ void ellpack(TYPE nzval[N*L], int32_t cols[N*L], TYPE vec[N], TYPE out[N])
//~ {
A3_KERNEL(a3inout_t pack) {

    /* ARTICo³ unpacking */
    a3data_t *nzval = &pack[0];
    a3data_t *cols = &pack[N*L];
    a3data_t *vec = &pack[2*N*L];
    a3data_t *out = &pack[2*N*L + N];
    /* END ARTICo³ unpacking */

    int i, j;
    TYPE Si;

    ellpack_1 : for (i=0; i<N; i++) {
        TYPE sum = a3tof(out[i]);
        ellpack_2 : for (j=0; j<L; j++) {
                Si = a3tof(nzval[j + i*L]) * a3tof(vec[cols[j + i*L]]);
                sum += Si;
        }
        out[i] = ftoa3(sum);
    }
}
