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

#define PID_SAMPLE_TIME 0.005f  // 5ms in seconds

uint8_t pid_compute(PID_Controller *pid, uint16_t input, uint8_t current_duty) {
    int16_t error = pid->setpoint - input;
    float Ts = PID_SAMPLE_TIME;
    
    float output = pid->kp * error + pid->integral;
    int result = current_duty + (int)output;
    
    int result_sat = result;
    if (result_sat > pid->output_max) result_sat = pid->output_max;
    if (result_sat < pid->output_min) result_sat = pid->output_min;
    
    if (result_sat == result) {
        pid->integral += pid->ki * error;
    }
    
    if (pid->integral > 20) pid->integral = 20;
    if (pid->integral < -20) pid->integral = -20;
    
    pid->prev_error = error;
    
    return (uint8_t)result_sat;
}

void pid_reset(PID_Controller *pid) {
    pid->integral = 0;
    pid->prev_error = 0;
}

void pid_set_limits(PID_Controller *pid, uint8_t min, uint8_t max) {
    pid->output_min = min;
    pid->output_max = max;
}