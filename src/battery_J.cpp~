
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <termios.h> 
#include <stdint.h>
#include "battery_J.h"
#include "gpio_J.h"
int fd_ultra;

int battery_init()
{
	//gpio 56 -d ho 1

	if((fd_ultra = open(DEV_ULTRA,O_RDWR))<0){
		perror("ultra open ");
		return -1;
	}
return 0;
}



int battery_read(int *val){
if(ioctl(fd_ultra,BATTERY,val)<0){
	return -1;
}
return 0;
}

		




/*void *batt_adc(void* d)
{
	printf("batt thread call\n");
	while(1){
		int bat_val=0;
		if((ioctl(fd_ultra,BATTERY,&bat_val))<0){
			perror("batt ioctl");
		}
		sprintf(buf,"batt = %d \n",bat_val);
		write(STDOUT_FILENO,buf,strlen(buf));

	}

}*/


