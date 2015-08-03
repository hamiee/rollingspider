#ifndef _PWM_J_h_
#define _PWM_J_h_


#define PWM_DEV "/dev/pwm"

int pwm_open();
int pwm_powerOn();
int pwm_init();
int pwm_powerOff();
int pwm_SetWidth( unsigned int pwm,unsigned int width );



#endif
