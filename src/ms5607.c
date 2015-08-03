
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
#include <math.h>
#include "ms5607.h"
#include "i2c_J.h"
MS5607_t MS5607;

#define MS5607_I2C_ADDR	0xEC>>1


unsigned char crc4(unsigned int n_prom[])
{
	int cnt;
	// simple counter
	unsigned int n_rem;
	// crc reminder
	unsigned int crc_read;
	// original value of the crc
	unsigned char n_bit;
	n_rem = 0x00;
	crc_read=n_prom[7];
	//save read CRC
	n_prom[7]=(0xFF00 & (n_prom[7]));
	//CRC byte is replaced by 0
	for (cnt = 0; cnt < 16; cnt++)
		// operation is performed on bytes
	{
		// choose LSB or MSB
		if (cnt%2==1) n_rem ^= (unsigned short) ((n_prom[cnt>>1]) & 0x00FF);
		else n_rem ^= (unsigned short) (n_prom[cnt>>1]>>8);
		for (n_bit = 8; n_bit > 0; n_bit--)
		{
			if (n_rem & (0x8000))
			{
				n_rem = (n_rem << 1) ^ 0x3000;
			}
			else
			{
				n_rem = (n_rem << 1);
			}
		}
	}
	n_rem= (0x000F & (n_rem >> 12)); // // final 4-bit reminder is CRC code
	n_prom[7]=crc_read;
	// restore the crc_read to its original place
	return (n_rem ^ 0x00);
}


extern int fd_sensor;
void MS5607Start(void)
{
	i2c_setSlave(DEV_MS);
	i2c_one_Write(fd_sensor,RESET);
	usleep(10000);
	uint8_t buf[10];
	i2c_read(DEV_MS,buf,2,READ_C1_H);
	MS5607.C1 = (buf[0]<<8)|buf[1];
	i2c_read(DEV_MS,buf,2,READ_C2_H);
	MS5607.C2 = (buf[0]<<8)|buf[1];
	i2c_read(DEV_MS,buf,2,READ_C3_H);
	MS5607.C3 = (buf[0]<<8)|buf[1];
	i2c_read(DEV_MS,buf,2,READ_C4_H);
	MS5607.C4 = (buf[0]<<8)|buf[1];
	i2c_read(DEV_MS,buf,2,READ_C5_H);
	MS5607.C5 = (buf[0]<<8)|buf[1];
	i2c_read(DEV_MS,buf,2,READ_C6_H);
	MS5607.C6 = (buf[0]<<8)|buf[1];
	i2c_read(DEV_MS,buf,2,READ_CRC_H);
	MS5607.CRC = (buf[0]<<8)|buf[1];

	printf("ms5607 \nC1 : %d\nC2 : %d\nC3 : %d\nC4 : %d \nC5 : %d\nC6 : %d\nCRC : %d\n",MS5607.C1,MS5607.C2,MS5607.C3,MS5607.C4,MS5607.C5,MS5607.C6,MS5607.CRC);


	/*
	   while(1)
	   {
	   MS5607SensorStartTemperatureSampling();
	   uint32_t temperature_un = MS5607ReadTemperatureWhenAvailable();
	   MS5607StartPressureSampling();
	   uint32_t pressure_un = MS5607ReadPressureWhenAvailable();

	   int32_t temperature = MS5607CompensateTemperature(temperature_un);
	   uint32_t pressure = MS5607CompensatePressure(pressure_un);


	   printf_len = 0;
	   printf_len += snprintf(printf_buff+printf_len, sizeof(printf_buff)-printf_len, "%lu,", pressure_un);
	   printf_len += snprintf(printf_buff+printf_len, sizeof(printf_buff)-printf_len, "%lu,", temperature_un);
	   printf_len += snprintf(printf_buff+printf_len, sizeof(printf_buff)-printf_len, "%lu,", pressure);
	   printf_len += snprintf(printf_buff+printf_len, sizeof(printf_buff)-printf_len, "%ld\r\n", temperature);
	   UartXmitString(printf_buff);
	   }
	   */
}



void MS5607SensorStartTemperatureSampling(void)
{	
	i2c_one_Write(fd_sensor,SENSOR_TEMPERATURE_OSR);
}
uint32_t MS5607ReadTemperatureWhenAvailable(void)
{
	uint32_t D2;
	uint8_t buf[3];
	i2c_read(DEV_MS,buf,3,ADC_READ_1);
	D2 = (buf[0]<<16)|(buf[1]<<8)|buf[2];
	return D2;
}

void MS5607StartPressureSampling (void)
{
	i2c_one_Write(fd_sensor,SENSOR_PRESSURE_OSR);
}

uint32_t MS5607ReadPressureWhenAvailable(void)
{
	uint32_t D1;
	uint8_t buf[3];	
	i2c_read(DEV_MS,buf,3,ADC_READ_1);
	D1 = (buf[0]<<16)|(buf[1]<<8)|buf[2];
	return D1;
}


int32_t MS5607CompensateTemperature(uint32_t D2)	 // Compensate temperature
{
	// Difference between actual and reference temperature
	MS5607.dT = (int32_t)D2 - ((int32_t)MS5607.C5 << 8);
	// Actual temperature (-40 unsigned long long 85°C with 0.01°C resolution)
	int32_t TEMP = 2000 + ((MS5607.dT * (int64_t)MS5607.C6) >> 23);
	return TEMP;
}

uint32_t MS5607CompensatePressure(uint32_t D1)	 // Compensate pressure
{
	// Offset at actual temperature
	int64_t OFF = ((int64_t)MS5607.C2 << 17) + (((int64_t)MS5607.C4 * MS5607.dT) >> 6);
	// Sensitivity at actual temperature
	int64_t SENS = ((int64_t)MS5607.C1 << 16) + (((int64_t)MS5607.C3 * MS5607.dT) >> 7);
	// Temperature compensated pressure (10 to 1200mbar with 0.01mbar resolution)
	uint32_t P = (((D1 * SENS) >> 21) - OFF) >> 15;
	return  P;
}
