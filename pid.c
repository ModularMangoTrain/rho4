#include "pid.h"

void pid_init(PID_Controller *pid, float kp, float ki, float kd, uint16_t setpoint) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->setpoint = setpoint;
    pid->integral = 0;
    pid->differential = 0;
    pid->prev_error = 0;
    pid->output_min = 0;
    pid->output_max = 100;
}

uint8_t pid_compute(PID_Controller *pid, uint16_t input, uint8_t current_duty) {
    int16_t error = pid->setpoint - input;
    
    pid->integral = pid->integral + pid->ki * 0.1 / 2 * (error + pid->prev_error);
    if (pid->integral > 100) pid->integral = 100;
    if (pid->integral < -100) pid->integral = -100;

    pid->differential = (2*pid->kd)/(2*0.2+0.1) * (error - pid->prev_error) + (2*0.2 - 0.1) / (2*0.2 + 0.1) * pid->differential;
    
    float output = pid->kp * error + pid->integral + pid->differential;
    pid->prev_error = error;
    
    // Rate limiting: max ±10% change per update
    if (output > 10.0) output = 10.0;
    if (output < -10.0) output = -10.0;
    
    int result = current_duty + (int)output;
    if (result > pid->output_max) result = pid->output_max;
    if (result < pid->output_min) result = pid->output_min;
    
    return (uint8_t)result;
}

void pid_reset(PID_Controller *pid) {
    pid->integral = 0;
    pid->prev_error = 0;
}

void pid_set_limits(PID_Controller *pid, uint8_t min, uint8_t max) {
    pid->output_min = min;
    pid->output_max = max;
}