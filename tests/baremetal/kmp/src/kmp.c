/*
Implementation based on http://www-igm.univ-mlv.fr/~lecroq/string/node8.html
*/

#include "kmp.h"

void CPF(char pattern[PATTERN_SIZE], int32_t kmpNext[PATTERN_SIZE]) {
    int32_t k, q;
    k = 0;
    kmpNext[0] = 0;

    c1 : for(q = 1; q < PATTERN_SIZE; q++){
        c2 : while(k > 0 && pattern[k] != pattern[q]){
            k = kmpNext[q];
        }
        if(pattern[k] == pattern[q]){
            k++;
        }
        kmpNext[q] = k;
    }
}


//int kmp(char pattern[PATTERN_SIZE], char input[STRING_SIZE], int32_t kmpNext[PATTERN_SIZE], int32_t n_matches[1]) {
int kmp(char pattern[STRING_SIZE], char input[STRING_SIZE], int32_t kmpNext[STRING_SIZE], int32_t n_matches[STRING_SIZE]) {
#pragma HLS interface s_axilite port=return
#pragma HLS INTERFACE m_axi depth=4 port=pattern offset=slave
#pragma HLS INTERFACE m_axi depth=8192 port=input offset=slave
#pragma HLS INTERFACE m_axi depth=4 port=kmpNext offset=slave
#pragma HLS INTERFACE m_axi depth=1 port=n_matches offset=slave
    int32_t i, q;
    n_matches[0] = 0;

    CPF(pattern, kmpNext);

    q = 0;
    k1 : for(i = 0; i < STRING_SIZE; i++){
        k2 : while (q > 0 && pattern[q] != input[i]){
            q = kmpNext[q];
        }
        if (pattern[q] == input[i]){
            q++;
        }
        if (q >= PATTERN_SIZE){
            n_matches[0]++;
            q = kmpNext[q - 1];
        }
    }
    return 0;
}
