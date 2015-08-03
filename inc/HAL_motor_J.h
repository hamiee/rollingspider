#ifndef _HAL_MOTOR_J_h_
#define _HAL_MOTOR_J_h_


int motor_init();
int motor_setDuty(unsigned int pwm,unsigned int width );  //duty width 0~10000 


#endif
