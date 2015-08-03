#ifndef _HAL_SENSOR_J_h_
#define _HAL_SENSOR_J_h_

#include "mpu6050_J.h"
#include "define_J.h"
typedef float_j_t sensor_t;

typedef struct sensor_struct_t{
sensor_t roll;
sensor_t pitch;
sensor_t yaw;
sensor_t accX;
sensor_t accY;
sensor_t accZ;
sensor_t gyroX;
sensor_t gyroY;
sensor_t gyroZ;
sensor_t ICS_accX;
sensor_t ICS_accY;
sensor_t ICS_accZ;
sensor_t norm_a;
int32_t ms_temperature;
int32_t ms_pressure;
}sensor_struct_t;

typedef enum {
KALMAN,
COMPLEMANTARY,
}filter_t;


//void quat_to_euler(float *ro,float* pit, float* ya );

int sensor_init(sensor_struct_t* angle); 
int sensor_read(sensor_struct_t* angle,sensor_t dt); //read mpu data
int sensor_filtering(filter_t fil, sensor_struct_t* angle,float dt); //read filter


#endif
