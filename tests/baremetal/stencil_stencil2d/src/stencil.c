#include "stencil.h"

//void stencil (TYPE orig[row_size * col_size], TYPE sol[row_size * col_size], TYPE filter[f_size]){
void stencil (TYPE orig[row_size * col_size], TYPE sol[row_size * col_size], TYPE filter[row_size * col_size]){
#pragma HLS interface s_axilite port=return
#pragma HLS INTERFACE m_axi depth=4096 port=orig offset=slave
#pragma HLS INTERFACE m_axi depth=4096 port=sol offset=slave
#pragma HLS INTERFACE m_axi depth=9 port=filter offset=slave
    int r, c, k1, k2;
    TYPE temp, mul;

    stencil_label1:for (r=0; r<row_size-2; r++) {
        stencil_label2:for (c=0; c<col_size-2; c++) {
            temp = (TYPE)0;
            stencil_label3:for (k1=0;k1<3;k1++){
                stencil_label4:for (k2=0;k2<3;k2++){
                    mul = filter[k1*3 + k2] * orig[(r+k1)*col_size + c+k2];
                    temp += mul;
                }
            }
            sol[(r*col_size) + c] = temp;
        }
    }
}
