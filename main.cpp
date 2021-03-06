/* Copyright (C) 2015 HAN Jae Seung, JAES Electronics. All rights reserved.
   |
   |    rolling spider 
   |     --Top view--
   |   .....up:- down:+  pitch,x.......
   |   2     head      0
   |    \     0|0     /
   |      \   0|0   /
   |        \  |  /
   |          \|/______> roll up:- down:+,y
   |          /|\
   |        /     \
   |      /         \
   |    /             \
   |   3                1
   */

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

#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#include <time.h>

#include <termios.h>

#include <sched.h>

#include "HAL_motor_J.h"
#include "HAL_pid_J.h"
#include "HAL_sensor_J.h"
#include "gpio_J.h"
#include "battery_J.h"
#include "define_J.h"
#include "ms5607.h"
#include "lpf_J.h"
#include "helper_3dmath.h"
#include "i2c_J.h"
#include "mpu6050_J.h"
#include "MPU6050.h"
#include "ms5607.h"

#define DEBUG(x)  //printf(x)
#define SET_BIT(y,x) (y|=(1<<x))
#define RESET_BIT(y,x) (y&=~(1<<x))

#define PRINTF_FIRST {printf("\033[1;1H");}
#define PRINTF_CLEAR {printf("\033[2J");}

void* t_function(void*);
void* main_function(void*);
void* press_function(void*);

extern MS5607_t MS5607;
/*
   typedef enum {
   EMERGENCY = 0, //system stop all motor init
   TAKEOFF = 1, // start timer and motor and pid
   SYSTEM_KONST = 2,// input = sys Kp Ki Kd 
   THROTT_INPUT = 3,
   TOTAL_SENSOR = 4,
//    MAHONY = 5,
CMD_TARGET = 7,
TARGET = 9,
DFACT = 10,
SETGAMMA = 91,
RPY_CALIB = 90,
}SYSTEM_CMD;


*/

void PRINT_NOTICE(){
    printf("*********** NOTICE ************\n"
            "0   : EMERGENCY\n"
            "1   : START CONTROL\n"
            "sensor  [ref,acc,gyro,rpy,battery,baro,dt,absacc,cancel] [1,0] : sensor on / off\n"
            "pid     [system] [kp] [ki] [kd] : pid control tuning [system = roll,pitch,yaw,x,y,z,press] \n"
            "target  [system] [value] : target be desired in control system\n"
            "gamma   [system] [value] : gamma is how many fast learning factor in adaptive control\n"
            "dfactor [system] [value] : non ideal pid control is used how many Difference factor\n"
            "fuzzy   [system] [Ye] [dYe_dt] : fuzzy divide factor\n"
            "fuzzytable [system] [arr FUZZY_TABLE] \n"
            "throttle [0~10000] : offset throttle value\n"
            "rpycalib : roll pitch yaw calibration \n"
            "bool [EKF,throttle,cancel,print,ics] [1,0] \n"
          );
}

void PRINT_STATE(){
    printf("*********** STATE ************\n"
          );

}

typedef enum{

    S_ACCEL=0,
    S_GYRO=1,
    S_RPY=2,
    S_BATT=3,
    S_BARO=4,
    S_dT=5,
    S_ABS_ACC = 7,
    S_REF_MODEL = 8,
    S_CANCEL = 9,

    //	S_KALMAN =  ,// motor value 0 1 2 3 input
    //	S_COMPLEMENTARY = 3,
    //	S_NEW = 4,
    //	S_QUAT = 5,
    //	S_QUAT_TO_EULER = 6,
    //	S_INTEGRAL = 7,
    //	S_MOTOR = 8,
}SENSRO_CMD;


float_j_t RPY_CALI[3];

uint32_t mask_bit =0;


bool B_throttle= true;
bool B_EKF=true;
bool B_cancel=true;
bool B_print=true;
bool B_ics=true;

timer_t timer1;
int TIMER_EMER =1;
int thrott;
uint32_t cnt;
sensor_struct_t sensor;
pid_control_t pid[MAX_SYSTEM];
float_j_t dt;
float_j_t dt_prev;

int32_t ground_press;
float_j_t press_err=0;
float_j_t target[MAX_SYSTEM];


float_j_t offset[MAX_SYSTEM];


static float_j_t sensor_alpha=0;



int battery_adc=900;
#define batt_fuzzy_num 12
float_j_t batt_fuzzy_table[2][12]={
    {845,840,830,820,810,800,790,780,770,760,740,710},  
    {0.0001633333,0.00015925,0.000157284,0.0001553659,0.000153494,0.0001516667,0.0001498824,0.0001481395,0.0001464368,0.0001447727,0.0001431461,0.0001415556}
};


static float_j_t throttle=1000;
float_j_t tar_pit=0,tar_rol=0;
void main_control_code  (int signum){
    GpioWrite(LED_RED_LEFT,1);
    struct timespec tp;
    if (clock_gettime (CLOCK_MONOTONIC, &tp) == -1)
        perror ("clock_gettime");

    //	sprintf (buffer, "%ld s %ld ns overrun %d", tp.tv_sec,
    //			tp.tv_nsec, timer_getoverrun (timer1));
    //	write (STDOUT_FILENO, buffer, strlen (buffer)); 

    float_j_t now= tp.tv_nsec*pow(10,-9);
    dt =  now - dt_prev;
    if(dt<0){
        dt += 1;
    }
    dt_prev = now;

    //	float_j_t gx = (gyroX*250/32768)*DEG_TO_RAD;
    //	float_j_t gy = (gyroY*250/32768)*DEG_TO_RAD;
    //	float_j_t gz = (gyroZ*250/32768)*DEG_TO_RAD;
    //	float_j_t ax = accX*GRAVITY*2/32768;
    //	float_j_t ay = accY*GRAVITY*2/32768;
    //	float_j_t az = accZ*GRAVITY*2/32768;
    //	MahonyAHRSupdateIMU(gx,gy,gz,ax,ay,az);	
    //	Yaw_m=atan2(2*q1*q2-2*q0*q3,2*q0*q0+2*q1*q1-1)*RAD_TO_DEG;
    //	Pitch_m=-1*asin(2*q1*q3+2*q0*q2)*RAD_TO_DEG;
    //	Roll_m=atan2(2*q2*q3-2*q0*q1,2*q0*q0+2*q3*q3-1)*RAD_TO_DEG;
#define WEIGHT_P_ERR 0.1
    press_err = WEIGHT_P_ERR * (ground_press - sensor.ms_pressure) + (1-WEIGHT_P_ERR)*press_err;
    //PRINT_BUG(9,"pree err %f\n",press_err);

    sensor_read(&sensor,dt);
    //   PRINT_BUG(9,"norm %f\n",norm_a);
    if(sensor.norm_a>=7){
        TIMER_EMER=1;
        motor_setDuty(0,0);
        motor_setDuty(1,0);
        motor_setDuty(2,0);
        motor_setDuty(3,0);
        printf("EMERGENCY\n");

    }


    if(TIMER_EMER==1){
        //    z_HDR = WEIGHT_Z_ERR * (aaWorld.z) + (1-WEIGHT_Z_ERR)*z_HDR;	
        return ;
    }
    //    sensor_filtering(KALMAN, &sensor,dt);

    //    float_j_t  temp_yaw = sensor.mpu.gyroZ*yaw_factor+(1-yaw_factor)*temp_prev;
    //    sensor.yaw += ((float_j_t)sensor.mpu.gyroZ-temp_yaw)/131*dt;
    //    temp_prev = temp_yaw;
    //printf("dt %.5f acc %.4f %.4f %.4f gyro %.4f %.4f %.4f\n",dt,sensor.accX,sensor.accY,sensor.accZ,sensor.gyroX,sensor.gyroY,sensor.gyroZ) ;
    //    printf("dt %f %f %f %f \n",dt,sensor.ICS_accX,sensor.ICS_accY,sensor.ICS_accZ);
#if 0
    pid_setErr(&pid[XAXIS],);
    pid_setErrD(&pid[XAXIS],,dt);
    pid_setErrI(&pid[XAXIS],,dt);
    int xForce = pid_control(&pid[XAXIS]);

    pid_setErr(&pid[YAXIS],);
    pid_setErrD(&pid[YAXIS],,dt);
    pid_setErrI(&pid[YAXIS],,dt);
    int yForce = pid_control(&pid[YAXIS]);

    pid_setErr(&pid[ZAXIS],);
    pid_setErrD(&pid[ZAXIS],,dt);
    pid_setErrI(&pid[ZAXIS],,dt);
    int zForce = pid_control(&pid[ZAXIS]);
#endif 



    //    float_j_t z_force = (float_j_t)(m_0+m_1+m_2+m_3)*z_factor/4+1;
    //    float_j_t xxx = aaWorld.x/z_force;
    //    float_j_t yyy = aaWorld.y/z_force;
    //    float_j_t tar_pit = asinf(xxx)*RAD_TO_DEG;
    //    float_j_t tar_rol = asinf(yyy)*RAD_TO_DEG;
    //    printf("z_f %d tp %f tr %f \n",z_force,tar_pit,tar_rol);


    /******************************************************************/


    /*
     *  acc feedback function
     */
    static int m_0,m_1,m_2,m_3;
    /*
     *
     *  theta pi  inverse estimate
     *
     * */
    //Thrust equation  = k w^2
#define mass 0.065

    static float_j_t thrust_eq_K=0.0001633333;
    /* thrust rotation abs force*/
    float_j_t x_dd,y_dd,z_dd;
    if(B_ics==true){
        x_dd= (sensor.ICS_accX+target[XAXIS])*mass/(thrust_eq_K*throttle);
        y_dd= (sensor.ICS_accY+target[YAXIS])*mass/(thrust_eq_K*throttle);
        //float_j_t z_dd= (-sensor.ICS_accZ+target[ZAXIS])*mass/(thrust_eq_K*throttle);
        z_dd= (sensor.ICS_accZ+GRAVITY)*mass/(thrust_eq_K*throttle);
    }else{
        x_dd= (target[XAXIS])*mass/(thrust_eq_K*throttle);
        y_dd= (target[YAXIS])*mass/(thrust_eq_K*throttle);
        //float_j_t z_dd= (-sensor.ICS_accZ+target[ZAXIS])*mass/(thrust_eq_K*throttle);
        z_dd= (GRAVITY*2)*mass/(thrust_eq_K*throttle);
    }

    battery_read(&battery_adc);
    /* fuzzy battery to k  */
    int i;
    for(i=0;i<batt_fuzzy_num;i++){
        if(battery_adc>batt_fuzzy_table[0][i]){
            break;
        }
    }
    if(i==0){
        thrust_eq_K = batt_fuzzy_table[1][0];
    }else if(i>=batt_fuzzy_num){
        thrust_eq_K = batt_fuzzy_table[1][batt_fuzzy_num-1];
    }else{
        thrust_eq_K = (batt_fuzzy_table[0][i-1]-battery_adc)/(batt_fuzzy_table[0][i-1]-batt_fuzzy_table[0][i])*batt_fuzzy_table[1][i]+(battery_adc-batt_fuzzy_table[0][i])/(batt_fuzzy_table[0][i-1]-batt_fuzzy_table[0][i])*batt_fuzzy_table[1][i-1];
    }


    //sensor_alpha = press_err*0.1;
    //throttle = sqrtf( powf(x_dd,2)+powf(sensor.ICS_accY,2)+powf(GRAVITY+GRAVITY+sensor_alpha+sensor.ICS_accZ,2) )*mass/thrust_eq_K;
    if(B_throttle==true){
        throttle = sqrtf( powf(sensor.ICS_accX,2)+powf(sensor.ICS_accY,2)+powf(GRAVITY+GRAVITY+sensor_alpha+target[ZAXIS]-sensor.ICS_accZ,2) )*mass/thrust_eq_K;
    }
    float_j_t pi = sensor.roll*DEG_TO_RAD ,theta = sensor.pitch*DEG_TO_RAD ,psi=0;


    //throttle = mass/thrust_eq_K*( x_dd*(sinf(theta)*cosf(psi)*cos(pi) + sinf(psi)*sin(pi)) + y_dd*(sinf(theta)*sinf(psi)*cosf(pi) - cos(psi)*sinf(pi)) +(GRAVITY*2+z_dd)*(cosf(theta)*cosf(pi)) );
    //throttle = mass/thrust_eq_K*( sensor.accX*(sinf(theta)*cosf(psi)*cos(pi) + sinf(psi)*sin(pi)) + sensor.accY*(sinf(theta)*sinf(psi)*cosf(pi) - cos(psi)*sinf(pi)) +(sensor.accZ)*(cosf(theta)*cosf(pi)) );



    float_j_t tar_r = asinf((x_dd*sinf(psi)-y_dd*cosf(psi))/sqrtf(powf(x_dd,2)+powf(y_dd,2)+powf(z_dd,2)))*RAD_TO_DEG;
    float_j_t tar_p = -atanf( (x_dd*cosf(psi)+y_dd*sinf(psi))/(z_dd) )*RAD_TO_DEG;

#if 0
    //b0 b1 b2 a0 a1 a2
    static float_j_t Coefficient[Section][N_th_order]={
        1,   -1.29632172693744 ,  1 ,  1 ,  -1.80515310279348 ,  0.887024494640021,
        1,   0.380515274317817 ,  1 ,  1 ,  -1.80676488212313 ,  0.826487086173650,
        1,   -1.55996982970591 ,  1 ,  1 ,  -1.82577026864564 ,  0.962742992695179


            /*
               1,   -1.80076771484253,   1,   1,   -1.92041654169673,   0.94144415753752,
               1,   -0.93913972362592,   1,   1,   -1.90464807479265,   0.90973233503983,
               1,   -1.88217035608364,   1,   1,   -1.94600151310549,   0.98086373952747
               */
    };
    static float_j_t Gain[Section+1]={  
        0.291999517481610
            ,0.233036368112730
            ,0.00392996161525334
            ,1
            /*0.26488320337131,
              0.13480587283977,
              0.00373540094367,
              1*/

    };

    static float_j_t state_x[Section][N_th_order/2]={0,}; //0 state is lastest
    static float_j_t state_y[Section][N_th_order/2]={0,}; //0 state is lastest
    //static float_j_t state_z[Section][N_th_order/2]={0,}; //0 state is lastest

    float_j_t ln;
    IIR_filter(&ln,tar_r,0,Coefficient,Gain,state_x);
    IIR_filter(&ln,ln,1,Coefficient,Gain,state_x);
    IIR_filter(&ln,ln,2,Coefficient,Gain,state_x);
    tar_rol = Gain[3]*ln;

    IIR_filter(&ln,tar_p,0,Coefficient,Gain,state_y);
    IIR_filter(&ln,ln,1,Coefficient,Gain,state_y);
    IIR_filter(&ln,ln,2,Coefficient,Gain,state_y);
    tar_pit = Gain[3]*ln;
#else
    if(B_cancel==true){
        tar_rol = tar_r ;
        tar_pit = tar_p;
    }
    /*
#define TAR_LPF 1
tar_rol = TAR_LPF*tar_r+(1-TAR_LPF)*tar_rol;
tar_pit = TAR_LPF*tar_p+(1-TAR_LPF)*tar_pit;
*/

#endif




#if 0
    pid_setErr(&pid[XAXIS],aaWorld.x);
    pid_setErrD(&pid[XAXIS],aaWorld.x-pid[XAXIS].err_prev,dt);
    pid_setErrI(&pid[XAXIS],pid[XAXIS].err,dt);
    pid_setErr_prev(&pid[XAXIS],pid[XAXIS].err);
    tar_pit=pid_control(&pid[XAXIS]);

    //   printf("tp %f ",tar_pit); 

    pid_setErr(&pid[YAXIS],aaWorld.y);
    pid_setErrD(&pid[YAXIS],aaWorld.y-pid[YAXIS].err_prev,dt);
    pid_setErrI(&pid[YAXIS],pid[YAXIS].err,dt);
    pid_setErr_prev(&pid[YAXIS],pid[YAXIS].err);
    tar_rol=pid_control(&pid[YAXIS]);
    //   printf("tr %f ",tar_rol); 
#endif

#if 1
    /* adaptive_control ROLL */

    //target[ROLL]+tar_rol;
    adaptive_ref_sys_model(&pid[ROLL],target[ROLL]+tar_rol,INIT_DT);
    adaptive_theta_calc(&pid[ROLL],sensor.roll,dt);
    pid_setErr(&pid[ROLL],target[ROLL]+tar_rol-sensor.roll);
    pid_setErrD(&pid[ROLL],sensor.gyroX,dt);
    //pid_setErrD(&pid[ROLL],gg[0]/16.4,dt);
    pid_setErrI(&pid[ROLL],pid[ROLL].err,dt);
    //int rForce=(int)adaptive_control(pid_control(&pid[ROLL]),theta);
    int rForce = (int)adaptive_pid_control(&pid[ROLL]);

#else
    int rForce=0;
#endif



#if 1  

    /*
#define YE_LPF_FACTOR 0.4f
#define DYE_DT_LPF_FACTOR 0.4f
pid_control_t *p = &pid[PITCH];
float_j_t Ye = ((cur_state-sensor.pitch)/p->Ye_div_factor)*YE_LPF_FACTOR+(1-YE_LPF_FACTOR)*p->Ye_prev;
float_j_t dYe_dt = ( (Ye - p->Ye_prev)/p->dYe_dt_div_factor /dt)*DYE_DT_LPF_FACTOR + (1-DYE_DT_LPF_FACTOR)*p->dYe_dt_prev  ;
p->Ye_prev = Ye;
p->dYe_dt_prev = dYe_dt;
*/


    adaptive_ref_sys_model(&pid[PITCH],target[PITCH]+tar_pit,INIT_DT);
    adaptive_theta_calc(&pid[PITCH],sensor.pitch,dt);
    pid_setErr(&pid[PITCH],target[PITCH]+tar_pit-sensor.pitch);
    pid_setErrD(&pid[PITCH],sensor.gyroY,dt);
    //pid_setErrD(&pid[PITCH],gg[1]/16.4,dt);
    pid_setErrI(&pid[PITCH],pid[PITCH].err,dt);
    //int pForce =(int)(adaptive_control( pid_control(&pid[PITCH]),theta));
    int pForce =(int)adaptive_pid_control(&pid[PITCH]);


    //    pid_setErr(&pid[YAW], sensor.gyroZ/16.4 );
    //    pid_setErrD(&pid[YAW],pid[YAW].err,dt);
    //    pid_setErr_prev(&pid[YAW],pid[YAW].err);
    //    pid_setErrI(&pid[YAW],pid[YAW].err,dt);
    //    int yawForce =(int) pid_control(&pid[YAW]);
#else
    cur_state= adaptive_ref_sys_model(&pid[PITCH],target[PITCH],INIT_DT);
    theta = adaptive_theta_calc(&pid[PITCH],cur_state,sensor.pitch,dt);
    pid_setErr(&pid[PITCH],target[PITCH]-sensor.pitch+tar_pit);
    pid_setErrD(&pid[PITCH],sensor.gyroY,dt);
    //pid_setErrD(&pid[PITCH],gg[1]/16.4,dt);
    pid_setErrI(&pid[PITCH],pid[PITCH].err,dt);
    //int pForce =(int)(adaptive_control( pid_control(&pid[PITCH]),theta));
    int pForce =(int)adaptive_pid_control(&pid[PITCH]);




#endif


#if 1  
    /*
     *
     *      YAW pid control
     *
     */

    pid_setErr(&pid[YAW], sensor.yaw);
    pid_setErrD(&pid[YAW],sensor.gyroZ+target[YAW],dt);
    //pid_setErrD(&pid[YAW],gg[2]/16.4,dt);
    pid_setErr_prev(&pid[YAW],pid[YAW].err);
    pid_setErrI(&pid[YAW],pid[YAW].err,dt);
    int yawForce =(int) pid_control(&pid[YAW]);

#else
    int yawForce=0;
#endif



#if 0
    /*
     *  Z acc  and  pressure 
     */

    z_HDR = WEIGHT_Z_ERR * (aaWorld.z) + (1-WEIGHT_Z_ERR)*z_HDR;	
    pid_setErr(&pid[ZAXIS],(aaWorld.z-z_HDR)*9.8/4096);
    pid_setErrD(&pid[ZAXIS],pid[ZAXIS].err-pid[ZAXIS].err_prev,dt);
    pid_setErrI(&pid[ZAXIS],pid[ZAXIS].err,dt);
    pid_setErr_prev(&pid[ZAXIS],pid[ZAXIS].err);
    Zthro = (int)pid_control(&pid[ZAXIS]);


    press_err = WEIGHT_P_ERR * (ground_press -sensor.ms_pressure -target[PRESS]) + (1-WEIGHT_P_ERR)*press_err;	
    pid_setErr(&pid[PRESS],press_err);
    pid_setErrD(&pid[PRESS],pid[PRESS].err-pid[PRESS].err_prev,dt);
    pid_setErrI(&pid[PRESS],pid[PRESS].err,dt);
    pid_setErr_prev(&pid[PRESS],pid[PRESS].err);
    Pthro = (int)pid_control(&pid[PRESS]);
#else
#endif



    //motor thrust force = k w^2 => motor saturation , w^2 force , inverse equation => k constant * (0~10000)/10000
    // k constant => included forward pid controller constant
    m_0 = pid_feedforward((rForce+pForce+yawForce)/2+thrott+throttle);
    m_1 = pid_feedforward((rForce-pForce-yawForce)/2+thrott+throttle);
    m_2 = pid_feedforward((-rForce+pForce-yawForce)/2+thrott+throttle);
    m_3 = pid_feedforward((-rForce-pForce+yawForce)/2+thrott+throttle);



    motor_setDuty(0,(m_0));
    motor_setDuty(2,(m_2));
    motor_setDuty(1,(m_1));
    motor_setDuty(3,(m_3));


#if 1
    /* if(cnt++>=5){
       cnt=0;
       char pbuf[40];
    //sprintf(pbuf,"Ye %f dYe %f Yp %f Ym %f theta %f\n",Ye,dYe_dt,sensor.pitch,cur_state,theta);
    //sprintf(pbuf,"WAC %f %f %f RPY %f %f %f BAT %d ",sensor.ICS_accX,sensor.ICS_accY,sensor.ICS_accZ,sensor.roll,sensor.pitch,sensor.yaw,battery_adc);
    sprintf(pbuf,"ACC %f %f %f GRYO %f %f %f DT %f \n",sensor.accX,sensor.accY,sensor.accZ,sensor.gyroX,sensor.gyroY,sensor.gyroZ,dt);
    write(STDOUT_FILENO,pbuf,strlen(pbuf));
    }*/
    //    float_j_t x2 = sensor.mpu.accX/2048;
    //    float_j_t y2 = sensor.mpu.accY/2048;
    //    float_j_t z2 = sensor.mpu.accZ/2048;
    //    float_j_t u = GRAVITY*sqrtf(x2*x2+y2*y2+z2*z2);
    //
    //    y_dd = - u * sin(sensor.pitch*DEG_TO_RAD);
    //    x_dd = u * cos(sensor.pitch*DEG_TO_RAD) * sin(sensor.roll*DEG_TO_RAD);
    //    z_dd = u * cos(sensor.pitch*DEG_TO_RAD) * cos(sensor.roll*DEG_TO_RAD);

#else
    //int batt;
    if((cnt++)%10==0){
        char pbuf[40];
        cnt=1;
        if(mask_bit){
            //printf("_J_ ");
        }
        if(mask_bit&(1<<S_REF_MODEL)){


            //sprintf(pbuf,"Yp %f Ym %f tar %f ",sensor.pitch,cur_state,tar_pit);
            sprintf(pbuf,"J %.2f %.2f %.4f %.4f ",sensor.pitch,cur_state,tar_pit,sensor.ICS_accX);
            write(STDOUT_FILENO,pbuf,strlen(pbuf));

            //printf("Yp %f Ym %f ",sensor.pitch,cur_state);
            //printf("tar  %f ",tar_pit);
        }
        if(mask_bit&(1<<S_RPY)){
            printf("RR %f PP %f YY %f ",sensor.roll-RPY_CALI[0],sensor.pitch-RPY_CALI[1],sensor.yaw-RPY_CALI[2]);
        }
        if(mask_bit&(1<<S_ACCEL)){
            printf("AX %f AY %f AZ %f ",sensor.accX,sensor.accY,sensor.accZ);
        }
        if(mask_bit&(1<<S_ABS_ACC)){
            //printf("WWZ %f ",sensor.accX*-sin(sensor.pitch*DEG_TO_RAD)+sensor.accY*sin(sensor.roll*DEG_TO_RAD)*cos(sensor.pitch*DEG_TO_RAD)+sensor.accZ*cos(sensor.roll*DEG_TO_RAD)*cos(sensor.pitch*DEG_TO_RAD));
            //printf("dd %f %f %f tar_rol %f tar_pit %f ",x_dd,y_dd,z_dd,tar_rol,tar_pit);
            printf("W %f %f %f ",sensor.ICS_accX,sensor.ICS_accY,sensor.ICS_accZ);
        }
        if(mask_bit&(1<<S_GYRO)){
            printf("GX %f GY %f GZ %f ",sensor.gyroX,sensor.gyroY,sensor.gyroZ);
            //printf("GX %d GY %d GZ %d ",gg[0],gg[1],gg[2]);
        }
        if(mask_bit&(1<<S_BARO)){
            printf("GND %d PRESS %d E %f ",ground_press,sensor.ms_pressure,press_err);
        }
        if(mask_bit&(1<<S_dT)){
            //sprintf(pbuf,"WAC %f %f %f RPY %f %f %f BAT %d ",sensor.ICS_accX,sensor.ICS_accY,sensor.ICS_accZ,sensor.roll,sensor.pitch,sensor.yaw,battery_adc);
            //sprintf(pbuf,"ACC %f %f %f GRYO %f %f %f DT %f ",sensor.accX,sensor.accY,sensor.accZ,sensor.gyroX,sensor.gyroY,sensor.gyroZ,dt);
            //write(STDOUT_FILENO,pbuf,strlen(pbuf));
            sprintf(pbuf,"dT %f ",dt);
            //sprintf(pbuf," \n");
            write(STDOUT_FILENO,pbuf,strlen(pbuf));
        }
        if(mask_bit&(1<<S_BATT)){
            printf("BAT %d ",battery_adc);
        }
        if(mask_bit){
            printf("\n");
        }
    }
#endif
#if 0
    float_j_t X = powf(((float_j_t)sensor.mpu.accX/2048),2);
    float_j_t Y = powf(((float_j_t)sensor.mpu.accY/2048),2);
    float_j_t Z = powf(((float_j_t)sensor.mpu.accZ/2048),2);

    float_j_t acc=sqrtf(X+Y+Z)*GRAVITY;
    //	printf("acc %f ",acc);	
    float_j_t z_dd = acc* cos(sensor.roll*DEG_TO_RAD)*cos(sensor.pitch*DEG_TO_RAD)-GRAVITY;
    //	printf("z_dd %f ",z_dd);
    pid_setErr(&pid[ZAXIS],z_dd);
    pid_setErrD(&pid[ZAXIS],z_dd-pid[ZAXIS].err_prev,dt);
    pid_setErrI(&pid[ZAXIS],z_dd,dt);
    pid_setErr_prev(&pid[ZAXIS],z_dd);
    int thro = pid_control(&pid[ZAXIS]);
    printf("thro %d ",thro);
#endif
#if 0
    float_j_t mass=65; //65g
    float_j_t alpha = 3.5;
    //float_j_t x_dd = - zForce/mass * sin(sensor.roll*DEG_TO_RAD);
    //float_j_t y_dd = zForce/mass * cos(sensor.roll*DEG_TO_RAD)*sin(sensor.pitch*DEG_TO_RAD);
    float_j_t z_dd = zForce/mass * cos(sensor.roll*DEG_TO_RAD)*cos(sensor.pitch*DEG_TO_RAD)-GRAVITY;

    pid_setErr(&pid[ZAXIS],z_dd);
    pid_setErrD(&pid[ZAXIS],pid[ZAXIS].err-pid[ZAXIS].err_prev,dt);
    pid_setErrI(&pid[ZAXIS],pid[ZAXIS].err,dt);	
    pid_setErr_prev(&pid[ZAXIS],z_dd);
    int omega = pid_control(&pid[ZAXIS]);
    zForce = omega*alpha;	
    int thro = omega*10000/328.625/(3.14*2);
#endif
    GpioWrite(LED_RED_LEFT,0);
}



/**********************************************************

  timer working function    High resolution timer + high priority process
  if endtime == 0 then infinite  wait for timer
  nperiod = nanoperiod,  nano end time;

 ************************************************************/
sigset_t set;
int TimerWorking(long nperiod, long nendtime){//if endtime == 0 then infinite  wait for timer 
    //printf("into TimerWorking \n");
    struct itimerspec new_value, old_value;
    struct sigaction action;
    struct sigevent sevent;

    //struct sched_param sp = { .sched_priority = 10 };
    struct sched_param sp ;
    sp.sched_priority = 10;
    int ret;
    /* process  priority high */
    ret = sched_setscheduler(0, SCHED_FIFO, &sp);
    if (ret == -1) {
        perror("sched_setscheduler");
        return 1;
    }

    /* SIGALRM for printing time */
    memset (&action, 0, sizeof (struct sigaction));
    action.sa_handler = main_control_code;
    if (sigaction (SIGALRM, &action, NULL) == -1)
        perror ("sigaction");

    /* for program completion */
    memset (&sevent, 0, sizeof (struct sigevent));
    sevent.sigev_notify = SIGEV_SIGNAL;
    sevent.sigev_signo = SIGRTMIN;

    if (timer_create (CLOCK_MONOTONIC, NULL, &timer1) == -1)
        //	if (timer_create (CLOCK_REALTIME, NULL, &timer1) == -1)
        perror ("timer_create");

    new_value.it_interval.tv_sec = 0;
    new_value.it_interval.tv_nsec = nperiod; /* 1ms */
    new_value.it_value.tv_sec = 0;
    new_value.it_value.tv_nsec = nperiod; /* 1ms */
    if (timer_settime (timer1, 0, &new_value, &old_value) == -1)
        perror ("timer_settime");

    if (sigemptyset (&set) == -1)
        perror ("sigemptyset");

    if (sigaddset (&set, SIGRTMIN) == -1)
        perror ("sigaddset");

    if (sigprocmask (SIG_BLOCK, &set, NULL) == -1)
        perror ("sigprocmask");
#if 0
    timer_t timer2;
    if (timer_create (CLOCK_MONOTONIC, &sevent, &timer2) == -1)
        perror ("timer_create");
    if(nendtime!=0){
        new_value.it_interval.tv_sec = 0;
        new_value.it_interval.tv_nsec = 0; /* one time timer, no reset */
        new_value.it_value.tv_sec = 0;
        new_value.it_value.tv_nsec = nendtime; /* endtime */
        if (timer_settime (timer2, 0, &new_value, &old_value) == -1)
            perror ("timer_settime");
    }
#endif
    return 0;
}

int TimerWait()
{
    int signum;
    /* wait for completion signal (1 ms) */
    if (sigwait (&set, &signum) == -1)
        perror ("sigwait");
    motor_setDuty(0,0);
    motor_setDuty(1,0);
    motor_setDuty(2,0);
    motor_setDuty(3,0);

    exit (EXIT_SUCCESS);
    return 0;
}
int Timer_check(){
    if(sigpending(&set)==0){
        if(sigismember(&set,SIGALRM)){
            motor_setDuty(0,0);
            motor_setDuty(1,0);
            motor_setDuty(2,0);
            motor_setDuty(3,0);
            printf("SIGALRM is pending!!\n");
            return -1;
        }
    }
    return 0;
}




int setecho(int fd, int onoff)
{
    struct termios t;

    if (tcgetattr(fd, &t) == -1) 
        return -1; 

    if (onoff == 0)
        t.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    else
        t.c_lflag |= (ECHO | ECHOE | ECHOK | ECHONL);

    if (tcsetattr(fd, TCSANOW, &t) == -1) 
        return -1; 
    return 0;
}





//struct timeval start_point, end_point;

    char buf[256];

void *print_thread(void* data){
    printf("t_state print thread\n");
//    double operating_time;
    // if((cnt++)%10==0){
    

    while(1){
        //    usleep(200000);
        usleep(50000);
        if((TIMER_EMER==1)||(B_print==false)){
            continue;

        }


//        gettimeofday(&end_point, NULL); 
//        operating_time = (double)(end_point.tv_sec)+(double)(end_point.tv_usec)/1000000.0-(double)(start_point.tv_sec)-(double)(start_point.tv_usec)/1000000.0;
        //printf("%.2f %.2f %.4f %.4f\n",sensor.pitch,pid[PITCH].state,tar_pit,sensor.ICS_accX);
            //printf("_J_: RR %f PP %f YY %f\n",sensor.roll,sensor.pitch,sensor.yaw);


        //printf("%f %f %f\n",sensor.pitch,pid[PITCH].state,pid[PITCH].adaptive_theta);
        //printf("%d %f %f %f \n",battery_adc,sensor.ICS_accX,sensor.ICS_accY,sensor.ICS_accZ);
        //printf("%f J %f %f %f %f %f %f \n",operating_time,throttle, sensor.ICS_accX,sensor.ICS_accY,sensor.ICS_accZ,tar_rol,tar_pit);
        //printf("dt %f Iac %f %f %f ac %f %f %f\n",dt,sensor.ICS_accX,sensor.ICS_accY,sensor.ICS_accZ,sensor.accX,sensor.accY,sensor.accZ);
        //printf("dt %f Iac %f %f %f tar %f %f \n",dt,sensor.ICS_accX,sensor.ICS_accY,sensor.ICS_accZ,tar_rol,tar_pit);


#if 1
        if(mask_bit){
            //operating_time = (double)(end_point.tv_sec)+(double)(end_point.tv_usec)/1000000.0-(double)(start_point.tv_sec)-(double)(start_point.tv_usec)/1000000.0;
            //printf("SDT %f ",operating_time);
            //printf("_J_: ");
            sprintf(buf,"_J_: ");

        }
        if(mask_bit&(1<<S_REF_MODEL)){

            
            //sprintf(pbuf,"Yp %f Ym %f tar %f ",sensor.pitch,cur_state,tar_pit);
            sprintf(buf,"%sJ %.2f %.2f %.4f %.4f ",buf,sensor.pitch,pid[PITCH].state,tar_pit,sensor.ICS_accX);
            
            //write(STDOUT_FILENO,pbuf,strlen(pbuf));

            //printf("Yp %f Ym %f ",sensor.pitch,cur_state);
            //printf("tar  %f ",tar_pit);
        }
        if(mask_bit&(1<<S_CANCEL)){
            sprintf(buf,"%sCAN %.2f %.2f ",buf,tar_rol,tar_pit);
        }
        if(mask_bit&(1<<S_RPY)){
            sprintf(buf,"%sRR %.2f PP %.2f YY %.2f ",buf,sensor.roll-RPY_CALI[0],sensor.pitch-RPY_CALI[1],sensor.yaw-RPY_CALI[2]);
        }
        if(mask_bit&(1<<S_ACCEL)){
            sprintf(buf,"%sACC %f %f %f ",buf,sensor.accX,sensor.accY,sensor.accZ);
        }
        if(mask_bit&(1<<S_ABS_ACC)){
            //printf("WWZ %f ",sensor.accX*-sin(sensor.pitch*DEG_TO_RAD)+sensor.accY*sin(sensor.roll*DEG_TO_RAD)*cos(sensor.pitch*DEG_TO_RAD)+sensor.accZ*cos(sensor.roll*DEG_TO_RAD)*cos(sensor.pitch*DEG_TO_RAD));
            //printf("dd %f %f %f tar_rol %f tar_pit %f ",x_dd,y_dd,z_dd,tar_rol,tar_pit);
            sprintf(buf,"%sWACC %f %f %f ",buf,sensor.ICS_accX,sensor.ICS_accY,sensor.ICS_accZ);
        }
        if(mask_bit&(1<<S_GYRO)){
            sprintf(buf,"%sGYRO %f %f %f ",buf,sensor.gyroX,sensor.gyroY,sensor.gyroZ);
            //printf("GX %d GY %d GZ %d ",gg[0],gg[1],gg[2]);
        }
        if(mask_bit&(1<<S_BARO)){
            sprintf(buf,"%sGND %d PRESS %d E %f ",buf,ground_press,sensor.ms_pressure,press_err);
        }
        if(mask_bit&(1<<S_dT)){
            //sprintf(pbuf,"WAC %f %f %f RPY %f %f %f BAT %d ",sensor.ICS_accX,sensor.ICS_accY,sensor.ICS_accZ,sensor.roll,sensor.pitch,sensor.yaw,battery_adc);
            //sprintf(pbuf,"ACC %f %f %f GRYO %f %f %f DT %f ",sensor.accX,sensor.accY,sensor.accZ,sensor.gyroX,sensor.gyroY,sensor.gyroZ,dt);
            //write(STDOUT_FILENO,pbuf,strlen(pbuf));
            sprintf(buf,"%sdT %f ",buf,dt);
            //sprintf(pbuf," \n");
        }
        if(mask_bit&(1<<S_BATT)){
            sprintf(buf,"%sBAT %d ",buf,battery_adc);
        }
        if(mask_bit){
            
            sprintf(buf,"%s\n",buf);
            write(1,buf,strlen(buf));
        }
#endif
        //gettimeofday(&start_point, NULL);
    }

    return 0;
}
//}


/*
   while(1){
//	sprintf(buffer,"observ %+f \n",observer);
//	write(STDOUT_FILENO,buffer,strlen(buffer));
if(mask_bit!=0){
printf("_J_DATA:");
}
if(mask_bit&(1<<S_SENSOR)){
printf("RR %+10.5f PP %+10.5f ",sensor.roll,sensor.pitch);
//write(STDOUT_FILENO,buf,strlen(buf));
}
if(mask_bit&(1<<S_ACCEL)){
printf("AX %+6d AY %+6d AZ %+6d ",sensor.accX,sensor.accY,sensor.accZ);
//write(STDOUT_FILENO,buf,strlen(buf));
}
if(mask_bit&(1<<S_GYRO)){
printf("GX %+6d GY %+6d GZ %+6d ",sensor.gyroX,sensor.gyroY,sensor.gyroZ);
// write(STDOUT_FILENO,buf,strlen(buf));
}
if(mask_bit&(1<<S_BARO)){
printf("GND %8d NOW %8d ",ground_press,sensor.ms_pressure);
}
if(mask_bit&(1<<S_BATT)){
int val;
if(battery_read(&val)==0){
printf("BAT %5d ",val);
}
}

if(mask_bit!=0){
printf("\n");
}
usleep(5000);
}
}
*/

#define MOVEAVE_SIZE 10
uint32_t move_average[MOVEAVE_SIZE]={0,};
uint32_t filter_move_average(uint32_t input){
    int i;
    for(i=MOVEAVE_SIZE-1; i>0 ;i--){
        move_average[i]=move_average[i-1];
    }
    move_average[0]=input;
    uint32_t val=0;
    for(i=0;i<MOVEAVE_SIZE;i++){
        val += move_average[i];
    }
    return (val/MOVEAVE_SIZE);
}
pthread_mutex_t  mutex_press = PTHREAD_MUTEX_INITIALIZER;
//uint32_t sec,nsec;

void *press_thread(void* data){
    printf("press_state print thread\n");
    while(1){
        MS5607SensorStartTemperatureSampling();
        usleep(8330);
        int32_t temperature_un = MS5607ReadTemperatureWhenAvailable();
        int32_t temperature = MS5607CompensateTemperature(temperature_un);
        if (temperature > -4000 && temperature < 8500 ){ 
            pthread_mutex_lock(&mutex_press);
            sensor.ms_temperature = temperature;
            pthread_mutex_unlock(&mutex_press);
        }
        MS5607StartPressureSampling();
        usleep(8330);
        int32_t pressure_un = MS5607ReadPressureWhenAvailable();
        int32_t pressure = MS5607CompensatePressure(pressure_un);
        if (pressure > 70000 && pressure < 120000 ){
            pthread_mutex_lock(&mutex_press);
            //sensor.ms_pressure = filter_move_average(pressure-100000)+100000;
            sensor.ms_pressure = pressure;       
            pthread_mutex_unlock(&mutex_press);
        }
#if 0
        if(mask_bit!=0){
            printf("_J_DATA:");
        }

        if(mask_bit&(1<<S_dT)){
#if 1
            struct timespec tp;
            if (clock_gettime (CLOCK_MONOTONIC, &tp) == -1){
                perror("clock_gettime");
            }
            int val = tp.tv_nsec - nsec;
            if(val<0){
                val +=1000000000;
            }
            //printf("press time %d pres %d fil %d  \n",val,pressure,sensor.ms_pressure );
            printf("dT %d ",val);
            nsec = tp.tv_nsec;
#endif
        }
        if(mask_bit&(1<<S_SENSOR)){
            printf("RR %f PP %f YY %f LPF %f ",sensor.roll,sensor.pitch,sensor.yaw,temp_prev);
        }
        if(mask_bit&(1<<S_ACCEL)){
            printf("AX %d AY %d AZ %d ",sensor.mpu.accX,sensor.mpu.accY,sensor.mpu.accZ);
        }
        if(mask_bit&(1<<S_GYRO)){
            printf("GX %d GY %d GZ %d ",sensor.mpu.gyroX,sensor.mpu.gyroY,sensor.mpu.gyroZ);
        }
        if(mask_bit&(1<<S_BARO)){
            printf("GND %d NOW %d ",ground_press,sensor.ms_pressure);
        }
        if(mask_bit&(1<<S_BATT)){
            int val;
            if(battery_read(&val)==0){
                printf("BAT %d ",val);
            }	
        }

        if(mask_bit!=0){
            printf("\n");
        }
#endif
    }
    return NULL;
}

void ble_call(){

    uint8_t buf[14];
    i2c_open();
    //i2c_setSlave(DEV_MS);
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
    int32_t ble_temp;
    int32_t ble_press;

    while(1){
        i2c_open();
        i2c_read(MPU6050_DEV,buf,14,MPU6050_RA_ACCEL_XOUT_H);
        i2c_close();
        int16_t accX = ((int16_t)(buf[0]<<8|buf[1]));
        int16_t accY = ((int16_t)(buf[2]<<8|buf[3]));
        int16_t accZ = ((int16_t)(buf[4]<<8|buf[5]));
        int16_t temper = (buf[6]<<8|buf[7]);
        int16_t gyroX = ((int16_t)(buf[8]<<8|buf[9]));
        int16_t gyroY = ((int16_t)(buf[10]<<8|buf[11]));
        int16_t gyroZ = ((int16_t)(buf[12]<<8|buf[13]));
        struct timespec tp;
        if (clock_gettime (CLOCK_MONOTONIC, &tp) == -1)
            perror ("clock_gettime");

        float_j_t now= tp.tv_nsec*pow(10,-9);
        dt =  now - dt_prev;
        if(dt<0){
            dt += 1;
        }
        dt_prev = now;


        //        i2c_open();
        //        MS5607SensorStartTemperatureSampling();
        //        i2c_close();
        i2c_open();
        int32_t temperature_un = MS5607ReadTemperatureWhenAvailable();
        i2c_close();
        int32_t temperature = MS5607CompensateTemperature(temperature_un);
        if ((temperature > 2000 && temperature < 8500 )){ 
            ble_temp=temperature;
        }
        //        i2c_open();
        //        MS5607StartPressureSampling();
        //        i2c_close();
        i2c_open();
        int32_t pressure_un = MS5607ReadPressureWhenAvailable();
        i2c_close();
        int32_t pressure = MS5607CompensatePressure(pressure_un);
        if ((pressure > 90000 && pressure < 120000 )){
            ble_press =pressure;
        }



        printf("dT %f ax %+8d ay %+8d az %+8d temp %+8d gx %+8d gy %+8d gz %+8d p_t %10d pre %10d  \n",dt,accX,accY,accZ,temper,gyroX,gyroY,gyroZ,ble_temp,ble_press);


    }

}


/*

   CONTROLLER CODE

*/
typedef enum{
    C_Emergency=0,
    C_UP=1,
    C_Down=2,
    C_Forward=3,
    C_Back=4,
    C_Left=5,
    C_Right=6,
    C_YawL=7,
    C_YawR=8,

}CONTROL_CMD;

static inline int myclock()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (tv.tv_sec*1000 + tv.tv_usec / 1000);
}

int controller(void){
    char str[16];
    int timelimit=100;
    const char ON='1', OFF='0';
    int escape = 1;
    int t, dt;

    bool prev_C_EMER=1;
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    setecho(STDIN_FILENO,0);
    printf("--Rolling Spider Controller--\n");
    dt = 0;

    while(escape){
        if(!dt){
            t = myclock();
        }

        scanf("%s",str);
        if(strcmp(str,"escape")==0){
            /* escape controller */
            printf("********* escape *********\n");
            escape = 0;
        }else if(strlen(str)==16){
            /* Controller  */

            t = myclock();

            if(str[C_Emergency]==ON){
                TIMER_EMER=1;
                motor_setDuty(0,0);
                motor_setDuty(1,0);
                motor_setDuty(2,0);
                motor_setDuty(3,0);
                prev_C_EMER=1;
            }else if(str[C_Emergency]==OFF){
                if(prev_C_EMER==1){
                    pid[ROLL].errI=0;
                    pid[PITCH].errI=0;
                    pid[YAW].errI=0;
                    pid[YAXIS].errI=0;
                    pid[XAXIS].errI=0;
                    pid[ZAXIS].errI=0;
                    pid[PRESS].errI=0;
                    tar_pit=0;
                    tar_rol=0;
                    //pid[PITCH].adaptive_theta = 1;
                    sensor.yaw = 0;            
                    //RPY_CALI[2] = sensor.yaw;      

                    press_err = 0;
                    TIMER_EMER=0;

                }

                prev_C_EMER=0;
                //sleep_timer(0,500000000);
                //ground_press = sensor.ms_pressure;
            }

            if((str[C_UP]==ON)&&(str[C_Down]==OFF)){
                target[ZAXIS]=3;
            }else if((str[C_UP]==OFF)&&str[C_Down]==ON){
                target[ZAXIS]=-3;
            }else{
                target[ZAXIS]=0;
            }

            if((str[C_Forward]==ON)&&(str[C_Back]==OFF)){
                target[XAXIS]=-7;
            }else if((str[C_Forward]==OFF)&&str[C_Back]==ON){
                target[XAXIS]=7;
            }else{
                target[XAXIS]=0;
            }

            if((str[C_Left]==ON)&&(str[C_Right]==OFF)){
                target[YAXIS]=3;
            }else if((str[C_Left]==OFF)&&str[C_Right]==ON){
                target[YAXIS]=-3;
            }else {
                target[YAXIS]=0;
            }

            if((str[C_YawL]==ON)&&(str[C_YawR]==OFF)){
                target[YAW]=-100;
            }else if((str[C_YawL]==OFF)&&(str[C_YawR]==ON)){
                target[YAW]=100;
            }else{
                target[YAW]=0;
            }



            //if(str[C_Emergency]==ON);
            //if(str[C_Emergency]==ON);
            //if(str[C_Emergency]==ON);
            //if(str[C_Emergency]==ON);
            //if(str[C_Emergency]==ON);
            //if(str[C_Emergency]==ON);
            //if(str[C_Emergency]==ON);
        }else{
            //printf("Exception Error\n");
        }

        /* time over error */
        dt = myclock() - t;
        if(dt > timelimit){
                TIMER_EMER=1;
                motor_setDuty(0,0);
                motor_setDuty(1,0);
                motor_setDuty(2,0);
                motor_setDuty(3,0);
            escape = 0;
            printf("Time over\n");
            return -1;
        }
    }

    setecho(STDIN_FILENO,1);
    return (0);
}

int find_system(char* str){
    if(strcmp(str,"roll")==0){
        return 0;
    }else if(strcmp(str,"pitch")==0){
        return 1;
    }else if(strcmp(str,"yaw")==0){
        return 2;
    }else if(strcmp(str,"x")==0){
        return 3;
    }else if(strcmp(str,"y")==0){
        return 4;
    }else if(strcmp(str,"z")==0){
        return 5;
    }else if(strcmp(str,"press")==0){
        return 6;
    }else {
        return -1;
    }
}
/********************************************************
 *
 *		MAIN function
 *
 *********************************************************/

int sleep_timer(int sec,int nsec);
void clean_stdin(void);
void *main_thread(void* data){
    printf("main print thread\n");
    int i,j;




}



int main(int argc, char* argv[]){
    int i,j; 
    int ble_flag=0;
    int kill_flag=1;
    for(i=0;i<MAX_SYSTEM;i++){
        target[i]=0;
    }

    pid_init(&pid[ROLL]);
    pid_init(&pid[PITCH]);
    pid_init(&pid[YAW]);
    pid_init(&pid[ZAXIS]);
    pid_init(&pid[XAXIS]);
    pid_init(&pid[YAXIS]);
    pid_init(&pid[PRESS]);

    //while(1){
    //adaptive_theta_calc(&pid[PITCH],1,1,INIT_DT);

    //}


    for(i=1;i<argc;i++){

        if(strcmp(argv[i],"-nokill")==0){
            printf("nokill dragon\n");
            kill_flag=0;
        }else if(strcmp(argv[i],"-ble")==0){
            //system("killall dragon-prog");
            ble_flag = 1;        
        }else if(strcmp(argv[i],"-load")==0){

        }

    }

    if(kill_flag==1){
        system("killall dragon-prog");
    }

    if(ble_flag==1){
        ble_call();
    }


    sensor_init(&sensor);
    printf("sensor init done\n");
    motor_init();
    printf("motor init done\n");
    battery_init();
    printf("battery init done\n");

    //  yaw_factor = 0.0000001;

    /*GPIO init*/
    GpioDir(LED_RED_LEFT,GPIO_OUTPUT_LOW);
    GpioDir(LED_GREEN_LEFT,GPIO_OUTPUT_LOW);



    pthread_attr_t attr;

    size_t stacksize;

    pthread_attr_init(&attr);
    pthread_attr_getstacksize (&attr, &stacksize);
    printf("Default stack size = %d\n", stacksize);

    stacksize = 1024*1024*3; 
    pthread_attr_setstacksize (&attr, stacksize);



    /*Pthread init*/
    pthread_t p_thread[3];
    if(pthread_create(&p_thread[1], &attr,print_thread, NULL)!=0){
        perror("thread create error : ");
        exit(0);
    }


    if(pthread_create(&p_thread[2], &attr, press_thread, NULL)!=0){
        perror("thread create error : ");
        exit(0);
    }

    /*    if(pthread_create(&p_thread[0], &attr, main_thread, NULL)!=0){
          perror("thread create error : ");
          exit(0);
          }
          */

    printf("pthread create done\n");


    /*Timer init*/
    //adaptive_getRefModel();
    //printf("adaptive model done\n");

    TimerWorking((int)(INIT_DT*1000000000),0);
    printf("timer working done\n");

    PRINT_NOTICE();


    int arr[FUZZY_TABLE][FUZZY_TABLE]= {0,};
    /* fuzzy theory init */

    //adaptive_setGamma(&pid[PITCH],1);
    int arr2[FUZZY_TABLE]={-3,-1,0,1,3};
    adaptive_setFuzzy_ref_table(&pid[PITCH],arr2);
    adaptive_setFuzzy_ref_table(&pid[ROLL],arr2);

    char num[20];
    while(1){
        //        fflush(stdin);
        //        clean_stdin();
        //printf("teset main thread\n");

        //scanf("%s",num);

        if(scanf("%s",num)<=0){
            continue;
        }
        /*int len = read(STDIN_FILENO,num,sizeof(num));
          if(len<0){
          continue;
          }
          num[len]='\0';
          */
        printf("Num %s\n",num);
        if(strcmp(num,"0")==0){
            TIMER_EMER=1;
            motor_setDuty(0,0);
            motor_setDuty(1,0);
            motor_setDuty(2,0);
            motor_setDuty(3,0);
            printf("EMERGENCY\n");
            PRINT_NOTICE();   
        }else if(strcmp(num,"load")==0){

            pid[YAW].Kp = 0;
            pid[YAW].Ki = 0;
            pid[YAW].Kd = 0.3;
            pid[ROLL].Kp = -20;
            pid[ROLL].Ki = -10;
            pid[ROLL].Kd = 0.04;
            pid[PITCH].Kp = -15;
            pid[PITCH].Ki = -7;
            pid[PITCH].Kd = 0.04;
            //thrott = 1500;
            pid[PITCH].gamma_factor = 0.8;
            pid[ROLL].gamma_factor = 0.8;

            adaptive_setFuzzy_factor(&pid[PITCH],2,4);
            adaptive_setFuzzy_factor(&pid[ROLL],2,4);

            int arr[FUZZY_TABLE][FUZZY_TABLE]= {
                {4,2,0,0,1},
                {4,1,0,1,2},
                {3,1,0,1,3},
                {2,1,0,1,4},
                {1,0,0,2,4}
            };
            /* fuzzy theory init */

            //adaptive_setGamma(&pid[PITCH],1);
            adaptive_setFuzzy_table(&pid[PITCH],arr);
            adaptive_setFuzzy_table(&pid[ROLL],arr);

            target[PITCH]=-2;
            target[ROLL]=-0.4;
            sensor_alpha = -0.2;
            //pid[ZAXIS].Kp = -30;
            //pid[ZAXIS].Ki = -10;
            //pid[ZAXIS].Kd = -20;

            printf("************** load *****************\n");

        }else if(strcmp(num,"exit")==0){
            motor_setDuty(0,0);
            motor_setDuty(1,0);
            motor_setDuty(2,0);
            motor_setDuty(3,0);
            exit(1);
            return 0;
        }else if(strcmp(num,"1")==0){
            //	        MS5607StartPressureSampling();
            //	        usleep(20000);	
            //	        uint32_t pressure_un = MS5607ReadPressureWhenAvailable();
            //	        ground_press = MS5607CompensatePressure(pressure_un);
            //			printf("ground_press %d \n",ground_press);
            pid[ROLL].errI=0;
            pid[PITCH].errI=0;
            pid[YAW].errI=0;
            pid[YAXIS].errI=0;
            pid[XAXIS].errI=0;
            pid[ZAXIS].errI=0;
            pid[PRESS].errI=0;
            tar_pit=0;
            tar_rol=0;
            pid[PITCH].adaptive_theta = 1;
            sensor.yaw = 0;            
            //RPY_CALI[2] = sensor.yaw;      

            press_err = 0;
            TIMER_EMER=0;

            //sleep_timer(0,500000000);
            ground_press = sensor.ms_pressure;
            printf("START\n");
        }else if(strcmp(num,"sensor")==0){

            char str[10];            
            int dd, tru;
            scanf("%s %d",str,&tru);
            if(strcmp(str,"acc")==0){
                dd = S_ACCEL;
            }else if(strcmp(str,"gyro")==0){
                dd = S_GYRO;
            }else if(strcmp(str,"rpy")==0){
                dd = S_RPY;
            }else if(strcmp(str,"battery")==0){
                dd = S_BATT;
            }else if(strcmp(str,"baro")==0){
                dd = S_BARO;
            }else if(strcmp(str,"dt")==0){
                dd = S_dT;
            }else if(strcmp(str,"cancel")==0){
                dd = S_CANCEL;
            }else if(strcmp(str,"absacc")==0){
                dd = S_ABS_ACC;
            }else if(strcmp(str,"ref")==0){
                dd = S_REF_MODEL;
            }            
            if(tru==1){
                SET_BIT(mask_bit,dd);
            }else{
                RESET_BIT(mask_bit,dd);
            }
            printf("mask_bit %s %d\n",str,tru);

        }else if(strcmp(num,"pid")==0){
            int sys;
            char buf[10];
            scanf("%s",buf);
            if((sys = find_system(buf))<0){
                perror("not find_system number");
            }
            if(sys>=MAX_SYSTEM){
                perror("sys input over");
            }else{	
                scanf("%f %f %f",&pid[sys].Kp,&pid[sys].Ki,&pid[sys].Kd);
                printf("_J_DATA: %s  Kp %f Ki %f Kd %f \n",buf,pid[sys].Kp,pid[sys].Ki,pid[sys].Kd);
            }


        }else if(strcmp(num,"target")==0){
            int sys;
            char buf[10];
            scanf("%s",buf);
            if((sys = find_system(buf))<0){
                perror("not find_system number");
            }
            float_j_t data;
            scanf("%f",&data);
            target[sys]=data;
            printf("target %s %f \n",buf,target[sys]);

        }else if(strcmp(num,"gamma")==0){
            int sys;
            char buf[10];
            scanf("%s",buf);
            if((sys = find_system(buf))<0){
                perror("not find_system number");
            }
            float_j_t aa;
            scanf("%f",&aa);
            adaptive_setGamma(&pid[sys],aa);
            printf("SET gamma_factor %s %f \n",buf,aa);
        }else if(strcmp(num,"dfactor")==0){
            float_j_t ff;
            int sys;
            char buf[10];
            scanf("%s",buf);
            if((sys = find_system(buf))<0){
                perror("not find_system number");
            }
            scanf("%f",&ff);
            pid_setDfactor(&pid[sys],ff);
        }else if(strcmp(num,"fuzzy")==0){
            float_j_t a,b;
            int sys;
            char buf[10];
            scanf("%s",buf);
            if((sys=find_system(buf))<0){
                perror("not find_system number");
            }
            scanf("%f %f",&a,&b);
            adaptive_setFuzzy_factor(&pid[sys],a,b);
        }else if(strcmp(num,"fuzzytable")==0){
            int sys;
            char buf[10];
            scanf("%s",buf);
            if((sys=find_system(buf))<0){
                perror("not find_system number");
            }
            for(i=0;i<FUZZY_TABLE;i++)
                for(j=0;j<FUZZY_TABLE;j++)
                    scanf("%d",&arr[i][j]);

            adaptive_setFuzzy_table(&pid[PITCH],arr);
        }else if(strcmp(num,"rpycalib")==0){
            RPY_CALI[0] = sensor.roll;
            RPY_CALI[1] = sensor.pitch;
            RPY_CALI[2] = sensor.yaw;      
            printf("RPY CALIBRATION\n");
        }else if(strcmp(num,"throttle")==0){
            //scanf("%d %d %d %d",&thrott[0],&thrott[1],&thrott[2],&thrott[3]);
            int data;
            scanf("%d",&data);
            thrott=data;
            printf("throttle %d\n",data);
            //		}else if(num==BATT){
            //			scanf("%d",&Batt_enable);
    }else if(strcmp(num,"test")==0){
        int a[5];
        int i;
        scanf("%d %d %d %d %d",&a[0],&a[1],&a[2],&a[3],&a[4]);


        printf("play to test\n");


        target[PITCH] = 0;
        if(sleep_timer(a[0],0)<0){
            TIMER_EMER=1;
            motor_setDuty(0,0);
            motor_setDuty(1,0);
            motor_setDuty(2,0);
            motor_setDuty(3,0);
            printf("EMERGENCY\n");
            continue;
        }
        for(i=0;i<a[1];i++){
            target[PITCH] = 30;
            if(sleep_timer(a[2],0)<0){
                TIMER_EMER=1;
                motor_setDuty(0,0);
                motor_setDuty(1,0);
                motor_setDuty(2,0);
                motor_setDuty(3,0);
                printf("EMERGENCY\n");
                break;
                continue;
            }
            target[PITCH] = -30;
            if(sleep_timer(a[3],0)<0){
                TIMER_EMER=1;
                motor_setDuty(0,0);
                motor_setDuty(1,0);
                motor_setDuty(2,0);
                motor_setDuty(3,0);
                printf("EMERGENCY\n");
                break;
                continue;
            }
        }

        if(i<a[1]){
            TIMER_EMER=1;
            motor_setDuty(0,0);
            motor_setDuty(1,0);
            motor_setDuty(2,0);
            motor_setDuty(3,0);
            printf("EMERGENCY\n");
            continue;
        }
        target[PITCH] = 0;
        if(sleep_timer(a[4],0)<0){
            TIMER_EMER=1;
            motor_setDuty(0,0);
            motor_setDuty(1,0);
            motor_setDuty(2,0);
            motor_setDuty(3,0);
            printf("EMERGENCY\n");
            continue;
        }
        TIMER_EMER=1;
        motor_setDuty(0,0);
        motor_setDuty(1,0);
        motor_setDuty(2,0);
        motor_setDuty(3,0);





    }else if(strcmp(num,"bool")==0){
        char buf[10];
        int nn;
        scanf("%s %d",buf,&nn);
        if(strcmp(buf,"EKF")==0){
            if(nn==1){
                B_EKF=true;
            }else{
                B_EKF=false;
            }
        }else if(strcmp(buf,"print")==0){
            if(nn==1){
                B_print=true;
            }else{
                B_print=false;
            }

        }else if(strcmp(buf,"throttle")==0){
            if(nn==1){
                B_throttle=true;
            }else{
                B_throttle=false;
            }
        }else if(strcmp(buf,"cancel")==0){
            if(nn==1){
                B_cancel=true;
            }else{
                B_cancel=false;
            }
        }else if(strcmp(buf,"ics")==0){
            if(nn==1){
                B_ics=true;
            }else{
                B_ics=false;
            }
        }


    }else if(strcmp(num,"alpha")==0){
        float_j_t a;
        scanf("%f",&a);
        printf("input alpha %f \n",a);
        sensor_alpha = a;
    }else if(strcmp(num,"controller")==0){
        printf("******Mode change*******\n");
        if(controller()==-1){

            printf("auto exit\n");
            return 0;
        }
    }
    memset(num,0,sizeof(num));
    }
    TimerWait();
    return 0;
}


int  sleep_timer(int sec,int nsec){
    struct timespec tp;
    clock_gettime (CLOCK_MONOTONIC, &tp);
    int init_time = tp.tv_sec;
    int init_ntime = tp.tv_nsec;
    while(1){
        int num; 
        if(scanf("%d",&num)>0){
            if(num==0){
                return -1;
            }
        }
        clock_gettime (CLOCK_MONOTONIC, &tp);
        if((tp.tv_sec - init_time)>=sec){
            if((tp.tv_nsec - init_ntime)>=nsec){
                break;
            }
        }
    }
    return 0;
}


void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}
