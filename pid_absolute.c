#include "pid_absolute.h"

#define PID_SAMPLE_TIME 0.020f  // 20ms

void pid_absolute_init(PID_Absolute *pid, float kp, float ki, float kd, float setpoint) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->setpoint = setpoint;
    pid->output_min = 0.0f;
    pid->output_max = 100.0f;
    pid_absolute_reset(pid);
}

void pid_absolute_reset(PID_Absolute *pid) {
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
}

uint8_t pid_absolute_compute(PID_Absolute *pid, uint16_t input) {
    float error = pid->setpoint - (float)input;
    
    // Proportional
    float p_term = pid->kp * error;
    
    // Integral with anti-windup
    float i_term = pid->ki * pid->integral;
    
    // Derivative
    float derivative = (error - pid->prev_error) / PID_SAMPLE_TIME;
    float d_term = pid->kd * derivative;
    pid->prev_error = error;
    
    // Compute absolute output
    float output = p_term + i_term + d_term;
    
    // Clamp output and update integral with anti-windup
    if (output > pid->output_max) {
        output = pid->output_max;
    } else if (output < pid->output_min) {
        output = pid->output_min;
    } else {
        pid->integral += error * PID_SAMPLE_TIME;
        if (pid->integral > 1000.0f) pid->integral = 1000.0f;
        if (pid->integral < -1000.0f) pid->integral = -1000.0f;
    }
    
    return (uint8_t)output;
}
