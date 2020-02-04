#include "fft.h"

void fft(float real[FFT_SIZE], float img[FFT_SIZE], float real_twid[FFT_SIZE], float img_twid[FFT_SIZE]){
#pragma HLS interface s_axilite port=return
#pragma HLS INTERFACE m_axi depth=1024 port=real offset=slave
#pragma HLS INTERFACE m_axi depth=1024 port=img offset=slave
#pragma HLS INTERFACE m_axi depth=1024 port=real_twid offset=slave
#pragma HLS INTERFACE m_axi depth=1024 port=img_twid offset=slave
    int even, odd, span, log, rootindex;
    float temp;
    log = 0;

    outer:for(span=FFT_SIZE>>1; span; span>>=1, log++){
        inner:for(odd=span; odd<FFT_SIZE; odd++){
            odd |= span;
            even = odd ^ span;

            temp = real[even] + real[odd];
            real[odd] = real[even] - real[odd];
            real[even] = temp;

            temp = img[even] + img[odd];
            img[odd] = img[even] - img[odd];
            img[even] = temp;

            rootindex = (even<<log) & (FFT_SIZE - 1);
            if(rootindex){
                temp = real_twid[rootindex] * real[odd] -
                    img_twid[rootindex]  * img[odd];
                img[odd] = real_twid[rootindex]*img[odd] +
                    img_twid[rootindex]*real[odd];
                real[odd] = temp;
            }
        }
    }
}
