#include "pid.h"

void pid_init(PID_Controller *pid, float kp, float ki, float kd, uint16_t setpoint) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->setpoint = setpoint;
    pid->integral = 0.0f;
    pid->differential = 0.0f;
    pid->prev_error = 0;
    pid->output_min = 0;
    pid->output_max = 100;
}

#define PID_SAMPLE_TIME 0.021f  // 21ms in seconds

uint8_t pid_compute(PID_Controller *pid, uint16_t input, uint8_t current_duty) {
    int16_t error = pid->setpoint - input;
    float Ts = PID_SAMPLE_TIME;
    
    // Proportional term
    float p_term = pid->kp * error;
    
    // Derivative term  
    float d_term = pid->kd * (error - pid->prev_error) / Ts;
    
    // Integrate with clamping
    pid->integral += pid->ki * Ts * error;
    if (pid->integral > 80) pid->integral = 80;
    if (pid->integral < -80) pid->integral = -80;
    
    // PID output (absolute form)
    float output = p_term + pid->integral + d_term;
    
    // Apply saturation
    int result_sat;
    if (output > pid->output_max) {
        result_sat = pid->output_max;
        pid->integral = pid->output_max - p_term - d_term;
    } else if (output < pid->output_min) {
        result_sat = pid->output_min;
        pid->integral = pid->output_min - p_term - d_term;
    } else {
        result_sat = (int)output;
    }
    
    pid->prev_error = error;
    
    return (uint8_t)result_sat;
}

void pid_reset(PID_Controller *pid) {
    pid->integral = 0.0f;
    pid->differential = 0.0f;
    pid->prev_error = 0;
}

void pid_set_limits(PID_Controller *pid, uint8_t min, uint8_t max) {
    pid->output_min = min;
    pid->output_max = max;
}