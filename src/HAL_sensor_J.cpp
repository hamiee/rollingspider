

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

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "helper_3dmath.h"
#include "KalmanJ.h"
#include "HAL_sensor_J.h"
#include "mpu6050_J.h"
#include "define_J.h"
#include "ms5607.h"
#include "lpf_J.h"
#include "Kalman_J.h"
#include "Extended_Kalman.h"
#include "battery_J.h"



extern bool B_EKF;


//#ifdef __EKF__
#define KALMAN1

#ifdef KALMAN2
kalman_data pitch_data;
kalman_data roll_data;
#endif


#ifdef  KALMAN1

Kalman_t kalmanX; // Create the Kalman instances
Kalman_t kalmanY;

#endif


sensor_t ms5607_cnt=0;
int ms5607_trig=0;
int sensor_read(sensor_struct_t* sensor,sensor_t dt){
    mpu6050_struct_t mpu;
    mpu6050_read(&mpu);
   
    /*
     *
     *      RAW SENSOR DATA
     *
     * */
    //sensor->accX = mpu.accX*GRAVITY/ACC_LSB;
    //sensor->accY = mpu.accY*GRAVITY/ACC_LSB;
    //sensor->accZ = mpu.accZ*GRAVITY/ACC_LSB;
    //sensor->gyroX = mpu.gyroX/GYRO_LSB;
    //sensor->gyroY = mpu.gyroY/GYRO_LSB;
    //sensor->gyroZ = mpu.gyroZ/GYRO_LSB;

#if 1
    /*
     *
     *   HSR ( Heuristic Scale Reduction ) 
     *
     * */ 
    //accX -> HSR -> LPF -> ICS_accX
    //float_j_t norm_a;
#define HSR_ic 1.00002
    static float_j_t scale_factor =0.99;

    //sensor->norm_a = sqrt((mpu.accX*mpu.accX+mpu.accY*mpu.accY+mpu.accZ*mpu.accZ))/ACC_LSB*scale_factor;
    sensor->norm_a = sqrtf((powf(mpu.accX,2)+powf(mpu.accY,2)+powf(mpu.accZ,2)))/ACC_LSB*scale_factor;
    
    //PRINT_BUG(9,"norm %f\n",norm_a);
    if(sensor->norm_a<1){
        scale_factor*=HSR_ic;
    }else if(sensor->norm_a>1){
        scale_factor/=HSR_ic;
    }
    sensor->accX = scale_factor*mpu.accX*GRAVITY/ACC_LSB;
    sensor->accY = scale_factor*mpu.accY*GRAVITY/ACC_LSB;
    sensor->accZ = scale_factor*mpu.accZ*GRAVITY/ACC_LSB;
    //PRINT_BUG(10,"norm %f fact %f \n",sensor->norm_a,scale_factor);
#else 


    sensor->accX = mpu.accX*GRAVITY/ACC_LSB;
    sensor->accY = mpu.accY*GRAVITY/ACC_LSB;
    sensor->accZ = mpu.accZ*GRAVITY/ACC_LSB;


#endif
#if 1
    /*
     *
     *  HDR (Heuristic Drift Reduction)
     *
     * */
    //sensor->gyro  is  Z^-1 
    int sign;
    float_j_t HDR_I;
#define  HDR_ic  0.0001
    static float_j_t HDR_I_prev[3]={0,};
    //Binary I_controller
    if(sensor->gyroX>0){
        sign = 1;
    }else if(sensor->gyroX<0){
        sign = -1;
    }else{
        sign = 0;
    } 
    HDR_I = HDR_I_prev[0] - sign*HDR_ic;
    HDR_I_prev[0] = HDR_I;
    sensor->gyroX=mpu.gyroX/GYRO_LSB-0+HDR_I;
    //Binary I_controller
    if(sensor->gyroY>0){
        sign = 1;
    }else if(sensor->gyroY<0){
        sign = -1;
    }else{
        sign = 0;
    } 
    HDR_I = HDR_I_prev[1] - sign*HDR_ic;
    HDR_I_prev[1] = HDR_I;
    sensor->gyroY=mpu.gyroY/GYRO_LSB-0+HDR_I;
    //Binary I_controller
    if(sensor->gyroZ>0){
        sign = 1;
    }else if(sensor->gyroZ<0){
        sign = -1;
    }else{
        sign = 0;
    } 
    HDR_I = HDR_I_prev[2] - sign*HDR_ic;
    HDR_I_prev[2] = HDR_I;
    sensor->gyroZ=mpu.gyroZ/GYRO_LSB-0+HDR_I;


    sensor->yaw  += sensor->gyroZ*dt; 
    if(sensor->yaw>=360){
        sensor->yaw-=360;
    }else if(sensor->yaw<=-360){
        sensor->yaw += 360;
    }

    //PRINT_BUG(10,"HDR %f %f %f \n",HDR_I_prev[0],HDR_I_prev[1],HDR_I_prev[2]);



#else
    sensor->gyroX=mpu.gyroX/GYRO_LSB;
    sensor->gyroY=mpu.gyroY/GYRO_LSB;
    sensor->gyroZ=mpu.gyroZ/GYRO_LSB;



#endif
    sensor_filtering(KALMAN,  sensor, dt);


    /*
     *
     * inertial coordinate system
     *
     * */

    sensor_t pi,theta,psi;
    pi = sensor->roll*DEG_TO_RAD;
    theta = sensor->pitch*DEG_TO_RAD;
    psi = 0;

    sensor_t ax,ay,az;
    ax = sensor->accX;
    ay = sensor->accY;
    az = sensor->accZ;



    float_j_t obs_ICS_accZ = -sinf(theta)*ax + cosf(theta)*sinf(pi)*ay + cosf(theta)*cosf(pi)*az;
    float_j_t obs_ICS_accX = cosf(psi)*cosf(theta)*ax + (cosf(psi)*sinf(theta)*sinf(pi)-sinf(psi)*cosf(pi))*ay + (cosf(psi)*sinf(theta)*cosf(pi)+sinf(psi)*sinf(pi))*az;
    float_j_t obs_ICS_accY = sinf(psi)*cosf(theta)*ax + (sinf(psi)*sinf(theta)*sinf(pi)+cosf(psi)*cosf(pi))*ay + (sinf(psi)*sinf(theta)*cosf(pi)-cosf(psi)*sinf(pi))*az;

#if 0
//b0 b1 b2 a0 a1 a2
static float_j_t Coefficient[Section][N_th_order]={
    1,  -0.470159672848057,  1 ,  1 ,  -1.60139216542065 ,  0.821599598123523,
    1,  1.24019733069713 ,   1 ,  1  , -1.67043703583951 ,  0.723892294576350,
    1,  -0.944203012189495,  1 ,  1 ,  -1.57258216031643 ,  0.941143388393353


    /*
    1,   -1.80076771484253,   1,   1,   -1.92041654169673,   0.94144415753752,
1,   -0.93913972362592,   1,   1,   -1.90464807479265,   0.90973233503983,
1,   -1.88217035608364,   1,   1,   -1.94600151310549,   0.98086373952747
    */
};
static float_j_t Gain[Section+1]={  
   0.361251759328520
  ,    0.464042976551585
  ,   0.00440723185008874
  ,  1 
    /*0.26488320337131,
0.13480587283977,
0.00373540094367,
1*/

};

static float_j_t state_x[Section][N_th_order/2]={0,}; //0 state is lastest
static float_j_t state_y[Section][N_th_order/2]={0,}; //0 state is lastest
static float_j_t state_z[Section][N_th_order/2]={0,}; //0 state is lastest

    float_j_t ln;
	IIR_filter(&ln,obs_ICS_accX,0,Coefficient,Gain,state_x);
    IIR_filter(&ln,ln,1,Coefficient,Gain,state_x);
	IIR_filter(&ln,ln,2,Coefficient,Gain,state_x);
	sensor->ICS_accX = Gain[3]*ln;

    IIR_filter(&ln,obs_ICS_accY,0,Coefficient,Gain,state_y);
    IIR_filter(&ln,ln,1,Coefficient,Gain,state_y);
	IIR_filter(&ln,ln,2,Coefficient,Gain,state_y);
	sensor->ICS_accY = Gain[3]*ln;

    IIR_filter(&ln,obs_ICS_accZ,0,Coefficient,Gain,state_z);
    IIR_filter(&ln,ln,1,Coefficient,Gain,state_z);
	IIR_filter(&ln,ln,2,Coefficient,Gain,state_z);
	sensor->ICS_accZ = Gain[3]*ln;
#else 
#define ICS_LPF 0.3
    sensor->ICS_accX=  (1-ICS_LPF)*sensor->ICS_accX +  ICS_LPF*obs_ICS_accX;
    sensor->ICS_accY=  (1-ICS_LPF)*sensor->ICS_accY +  ICS_LPF*obs_ICS_accY;
    sensor->ICS_accZ=  (1-ICS_LPF)*sensor->ICS_accZ +  ICS_LPF*obs_ICS_accZ;

/*#define ICS_LPF 0.3
    sensor->ICS_accX=ICS_LPF*obs_ICS_accX+(1-ICS_LPF)*sensor->ICS_accX;
    sensor->ICS_accY=ICS_LPF*obs_ICS_accY+(1-ICS_LPF)*sensor->ICS_accY;
    sensor->ICS_accZ=ICS_LPF*obs_ICS_accZ+(1-ICS_LPF)*sensor->ICS_accZ;
*/
#endif





/*    sensor->ICS_accX = ISC_LPF*obs_ICS_accX+(1-ISC_LPF)*sensor->ICS_accX;
    sensor->ICS_accY = ISC_LPF*obs_ICS_accY+(1-ISC_LPF)*sensor->ICS_accY;
    sensor->ICS_accZ = ISC_LPF*obs_ICS_accZ+(1-ISC_LPF)*sensor->ICS_accZ;
*/

    // mpu6050_loop() ;


    //  sensor->yaw = ypr[0]*180/M_PI;
    //  sensor->pitch = ypr[1]*180/M_PI;
    //  sensor->roll = ypr[2]*180/M_PI;

    //    //ms5607 convesion time is typ 8.33 ms 
    //    if(ms5607_cnt>0.022){
    
    //        ms5607_cnt = 0;
    //    }
    //    if(ms5607_cnt==0){
    //        ms5607_trig=0;
    //        uint32_t pressure_un = MS5607ReadPressureWhenAvailable();
    //        //printf(" pre_un %d ",pressure_un);
    //        MS5607SensorStartTemperatureSampling();
    //        uint32_t pressure = MS5607CompensatePressure(pressure_un);
    //        if (pressure > 70000 && pressure < 120000 ){
    //            sensor->ms_pressure = pressure;       
    //        }
    //        //filter_move_average 
    //        filter_move_average(sensor->ms_pressure);
    //    }
    //    else if((ms5607_cnt>0.011)&&(ms5607_trig==0)){
    //        ms5607_trig=1;
    //        uint32_t temperature_un = MS5607ReadTemperatureWhenAvailable();
    //        //printf(" tem_un %d ",temperature_un);
    //        MS5607StartPressureSampling();
    //        int32_t temperature = MS5607CompensateTemperature(temperature_un);
    //        if (temperature > -4000 && temperature < 8500 ) 
    //            sensor->ms_temperature = temperature;
    //    }
    //    ms5607_cnt+=dt;	

    return 0;
}

extern uint32_t ground_press;

int sensor_init(sensor_struct_t* sensor){

    /* mpu6050_init */
    mpu6050_init();
    mpu6050_struct_t data;
    mpu6050_read(&data);

#ifdef KALMAN2
    kalman_init(&pitch_data);
    kalman_init(&roll_data);

#endif 

#ifdef KALMAN1

    /*value , kalman init */
    sensor_t roll,pitch;
    roll  = atan2(data.accY, data.accZ) * RAD_TO_DEG;
    pitch = atan(-data.accX / sqrt(data.accY * data.accY + data.accZ * data.accZ)) * RAD_TO_DEG;	

    KalmanInit(&kalmanX);
    KalmanInit(&kalmanY);

    KalmansetAngle(&kalmanX,roll); // Set starting sensor
    KalmansetAngle(&kalmanY,pitch);
    sensor->roll = roll;
    sensor->pitch = pitch;
#endif


    /* ms5607 init */
    MS5607Start();
    MS5607SensorStartTemperatureSampling();
    usleep(9300);
    uint32_t temperature_un = MS5607ReadTemperatureWhenAvailable();
    printf("temp _un %d \n",temperature_un);
    MS5607StartPressureSampling();
    usleep(9300);	
    uint32_t pressure_un = MS5607ReadPressureWhenAvailable();
    printf("press _un %d \n",pressure_un);
    int32_t temperature = MS5607CompensateTemperature(temperature_un);
    uint32_t pressure = MS5607CompensatePressure(pressure_un);

    printf( "%dPa ", pressure);
    if (pressure > 50000 && pressure < 120000 ){
        printf( "OK\n\r");

    }else
        printf( "FAIL\n\r");


    printf("Temperature: ");
    printf("%d.%d°C ", temperature/100,(uint32_t)temperature%100);
    if (temperature > -4000 && temperature < 8500 )
        printf( "OK\n\r");
    else
        printf( "FAIL\n\r");

    sensor->ms_temperature  = temperature;
    sensor->ms_pressure =  pressure;
    MS5607StartPressureSampling();

    // lpf_init();


    return 0;
}


int sensor_filtering(filter_t fil, sensor_struct_t* sensor, sensor_t dt){

if(B_EKF==true){
    kalman_function(sensor,dt);
    return 0;
}
#ifdef KALMAN2

    float_j_t acc_pitch = atan2(sensor->accX, -sensor->accZ);
    float_j_t acc_roll = -atan2(sensor->accY, -sensor->accZ);
    kalman_innovate(&pitch_data, acc_pitch, sensor->gyroX);
    kalman_innovate(&roll_data, acc_roll, sensor->gyroY);
    sensor->pitch = pitch_data.x1;
    sensor->roll = roll_data.x1;

    return 0;
#endif 

#ifdef KALMAN1
    sensor_t accX = sensor->accX;
    sensor_t accY = sensor->accY;
    sensor_t accZ = sensor->accZ;
    //	sensor_t compsensorX,compsensorY;
    sensor_t square = accY*accY + accZ*accZ;
    if(square<0){
        square=0;
    }
    sensor_t roll  = atan2f(accY, accZ) * RAD_TO_DEG;
    sensor_t pitch = atan2f(-accX, sqrt(square))*RAD_TO_DEG;

    //sensor_t gyroXrate = gyroX*250/32768; // Convert to deg/s
    //sensor_t gyroYrate = gyroY*250/32768; // Convert to deg/s
    sensor_t gyroXrate = sensor->gyroX; // Convert to deg/s
    sensor_t gyroYrate = sensor->gyroY; // Convert to deg/s
    // This fixes the transition problem when the accelerometer sensor jumps between -180 and 180 degrees
    sensor_t kalsensorX = sensor->roll;
    sensor_t kalsensorY = sensor->pitch;


    // This fixes the transition problem when the accelerometer sensor jumps between -180 and 180 degrees
    if ((roll < -90 && kalsensorX > 90) || (roll > 90 && kalsensorX < -90)) {
        KalmansetAngle(&kalmanX,roll);
        kalsensorX = roll;
    } else
        kalsensorX = KalmangetAngle(&kalmanX,roll, gyroXrate, dt); // Calculate the sensor using a Kalman filter

    kalsensorY = KalmangetAngle(&kalmanY,pitch, gyroYrate, dt);
    //gyroXsensor += gyroXrate * dt; // Calculate gyro sensor without any filter
    //gyroYsensor += gyroYrate * dt;
    //gyroXsensor += kalmanX.getRate() * dt; // Calculate gyro sensor using the unbiased rate
    //gyroYsensor += kalmanY.getRate() * dt;

    //	compsensorX = 0.93 * (compsensorX + gyroXrate * dt) + 0.07 * roll; // Calculate the sensor using a Complimentary filter
    //	compsensorY = 0.93 * (compsensorY + gyroYrate * dt) + 0.07 * pitch;

    // Reset the gyro sensor when it has drifted too much
    //	if (gyroXsensor < -180 || gyroXsensor > 180)
    //		gyroXsensor = kalsensorX;
    //	if (gyroYsensor < -180 || gyroYsensor > 180)
    //		gyroYsensor = kalsensorY;

    //if(fil==KALMAN){
    sensor->roll = kalsensorX;
    sensor->pitch = kalsensorY;
    //}else if(fil==COMPLEMANTARY){
    //sensor->roll = compsensorX;
    //sensor->pitch = compsensorY;
    //}
    return 0;
#endif
}

