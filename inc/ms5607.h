/*
 * MS5607.h
 *
 *  Created on: 21 janv. 2012
 *      Author: timothee
 */

#ifndef MS5607_H_
#define MS5607_H_

#define DEV_MS 0x77
typedef struct
{
	uint16_t Reserved;
	uint16_t C1;  // Pressure sensitivity  SENS
	uint16_t C2;  // Pressure offset  OFF
	uint16_t C3;  // Temperature coefficient of pressure sensitivity  TCS
	uint16_t C4;  // Temperature coefficient of pressure offset  TCO
	uint16_t C5;  // Reference temperature
	uint16_t C6;  // Temperature coefficient of the temperature
	uint16_t CRC;
	int32_t dT;   // Difference between actual and reference temperature
}MS5607_t;


extern MS5607_t MS5607;
enum
{
	RESET = 0x1E,

	CONV_C1_OSR_256 = 0x40,
	CONV_C1_OSR_512 = 0x42,
	CONV_C1_OSR_1024 = 0x4,
	CONV_C1_OSR_2048 = 0x46,
	CONV_C1_OSR_4096 = 0x48,
	//CONV_C1_OSR_4096_L = 0x49,

	CONV_C2_OSR_256 = 0x50,
	CONV_C2_OSR_512 = 0x52,
	CONV_C2_OSR_1024 = 0x54,
	CONV_C2_OSR_2048 = 0x56,
	CONV_C2_OSR_4096 = 0x58,
	//CONV_C2_OSR_4096_L = 0x59,

	ADC_READ_1 = 0x00,
	ADC_READ_2 = 0x01,
	ADC_READ_3 = 0x02,
	ADC_READ_4 = 0x03,

	READ_C1_H = 0xA2,
	READ_C1_L = 0xA3,
	READ_C2_H = 0xA4,
	READ_C2_L = 0xA5,
	READ_C3_H = 0xA6,
	READ_C3_L = 0xA7,
	READ_C4_H = 0xA8,
	READ_C4_L = 0xA9,
	READ_C5_H = 0xAA,
	READ_C5_L = 0xAB,
	READ_C6_H = 0xAC,
	READ_C6_L = 0xAD,
	READ_CRC_H = 0xAE,
	READ_CRC_L = 0xAF,

};
#define 		SENSOR_TEMPERATURE_OSR			CONV_C2_OSR_4096
//#define 		SENSOR_TEMPERATURE_OSR_L			CONV_C2_OSR_4096_L

#define 		SENSOR_PRESSURE_OSR				CONV_C1_OSR_4096

void MS5607Start(void);
void MS5607Stop(void);
void MS5607SensorStartTemperatureSampling(void);
uint32_t MS5607ReadTemperatureWhenAvailable(void);
void MS5607StartPressureSampling (void);
uint32_t MS5607ReadPressureWhenAvailable(void);
int32_t MS5607CompensateTemperature(uint32_t D2);
uint32_t MS5607CompensatePressure(uint32_t D1);




#endif /* MS5607_H_ */
