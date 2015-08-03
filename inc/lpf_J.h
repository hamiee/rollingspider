

#ifndef __IIR_DIRECT_FORM_2__
#define __IIR_DIRECT_FORM_2__

#include "define_J.h"


#define N_th_order 6
#define NUMERATOR 3
#define DENOMINATOR 3
#define Section 3

int IIR_filter( float_j_t *output,float_j_t input,int sec,float_j_t Coefficient[][N_th_order],float_j_t Gain[],float_j_t state[][N_th_order/2] );

#endif

