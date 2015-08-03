#ifndef _HAL_PID_J_h_
#define _HAL_PID_J_h_


/* MOTOR control */
#include "HAL_sensor_J.h"
#include "define_J.h"
#include <math.h>


#define WEIGHT 0.7
typedef float_j_t motor_t;

#define REF_COUNT 2
#define MAX_SYSTEM 7
typedef enum {
ROLL=0,
PITCH=1,
YAW=2,
XAXIS=3,
YAXIS=4,
ZAXIS=5,
PRESS=6,
}system_t;

#define FUZZY_TABLE 5

typedef struct pid_control_t{
	motor_t Kp;
	motor_t Ki;
	motor_t Kd;
	motor_t err;
	motor_t errD;
	motor_t errI;
	motor_t err_prev;
    motor_t non_ideal_D_factor; 
//	motor_t errD_prev;
  /*
   *  adaptive control
   */
    motor_t gamma_factor;
    motor_t adaptive_theta;
//    motor_t input_model_Z[REF_COUNT];

    motor_t x1_k;
    motor_t x2_k;
    motor_t state;

/*
 *  fuzzy theory
 */
    motor_t Ye_prev;
    motor_t dYe_dt_prev;
    motor_t Ye_div_factor;
    motor_t dYe_dt_div_factor;
    int fuzzy_ref_table[FUZZY_TABLE];
    int fuzzy_table[FUZZY_TABLE][FUZZY_TABLE];
}pid_control_t;

void pid_init(pid_control_t *p);
void pid_setDfactor(pid_control_t *p,motor_t val);
void pid_setErr(pid_control_t *p, motor_t val);
void pid_setErrD(pid_control_t *p, sensor_t val,sensor_t dt);
void pid_setErrI(pid_control_t *p,sensor_t val, motor_t dt);
void pid_setErr_prev(pid_control_t *p, motor_t val);


motor_t pid_control(pid_control_t* control);

uint32_t pid_feedforward(int m_val);


motor_t adaptive_pid_control(pid_control_t* control);
void adaptive_getRefModel();
void adaptive_init(pid_control_t* p);
void adaptive_setGamma(pid_control_t*p,motor_t val);
/* ref sys model  : input, dt  -> return current_state */
int adaptive_ref_sys_model(pid_control_t*p,motor_t input,motor_t dt);
// theta_calc  return theta 
int  adaptive_theta_calc(pid_control_t* p , motor_t ref, motor_t plant_feedback);
//  adaptive_control    pid control output,  theta , adaptive  return MV 
motor_t adaptive_control(motor_t pid_out, motor_t adaptive_theta);

/* fuzzy theory */
#define PB  2
#define PS  1
#define ZE  0
#define NS  -1
#define NB  -2

void adaptive_setFuzzy_factor(pid_control_t* p,float_j_t Ye, float_j_t dYe_dt);
void adaptive_setFuzzy_table(pid_control_t* p, int matrix[FUZZY_TABLE][FUZZY_TABLE]);
void adaptive_setFuzzy_ref_table(pid_control_t* p , int matrix[FUZZY_TABLE]);
//float_j_t fuzzy_membership_function(pid_control_t* pid, float_j_t now, int desire);
motor_t adaptive_theta_calc(pid_control_t* p , motor_t ref, motor_t plant_feedback,float_j_t dt);





#endif
