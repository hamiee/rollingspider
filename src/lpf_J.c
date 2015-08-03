#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <stdint.h>
#include "lpf_J.h"
#include "HAL_sensor_J.h"


#define __IIR_DIRECT_FORM_2__


#ifdef __IIR_DIRECT_FORM_2__

#define N_th_order 6
#define NUMERATOR 3
#define DENOMINATOR 3
#define Section 3

int IIR_filter( float_j_t *output,float_j_t input,int sec,float_j_t Coefficient[][N_th_order],float_j_t Gain[],float_j_t state[][N_th_order/2] ){
/*
	float_j_t temp=0;
	for(i=0;i<NUMERATOR;i++){
		temp += state[sec][i]*Coefficient[sec][i];
	}
	float_j_t y = Gain[sec]*temp;
*/	

	memmove( &state[sec][1], &state[sec][0] ,2*sizeof(float_j_t)  );

	state[sec][0] = input*Coefficient[sec][3]-Coefficient[sec][4]*state[sec][1]-Coefficient[sec][5]*state[sec][2];

	*output = Gain[sec]*(state[sec][0]*Coefficient[sec][0]+state[sec][1]*Coefficient[sec][1]+state[sec][2]*Coefficient[sec][2]);


	return 0;
}


#endif


