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
#include "gpio_J.h"

int fd_gpio;

void GpioOpen(){
	if((fd_gpio = open(GPIO_DEV,O_RDWR))<0){
		perror("open err");
	}
}

void GpioDir(int pin,enum gpio_mode mode){
	struct gpio_direction dir;
	dir.pin=pin;
	dir.mode = mode;
	if(ioctl(fd_gpio,GPIO_DIRECTION,&dir)<0){
		perror("dir  err");
	}
}

void GpioWrite(int pin, int value){
	struct gpio_data data;
	data.pin = pin;
	data.value = value;
	ioctl(fd_gpio,GPIO_WRITE,&data);
}


