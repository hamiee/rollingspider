#ifndef _MPU_6050_J_h_
#define _MPU_6050_J_h_

/*   Sensor  */
#define MPU6050_DEV 	0x68 
#define MPU6050_OFFSET 	0x3B

typedef float sensor_t;

typedef struct mpu6050_struct_t{
int16_t accX, accY, accZ;
int16_t gyroX, gyroY, gyroZ;
uint16_t temper;
}mpu6050_struct_t;

int mpu6050_read(mpu6050_struct_t* data );
int mpu6050_init();

#endif
