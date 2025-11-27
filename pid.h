#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef struct {
    float kp;
    float ki;
    float kd;
    uint16_t setpoint;
    float integral;
    float differential;
    int16_t prev_error;
    uint8_t output_min;
    uint8_t output_max;
} PID_Controller;

void pid_init(PID_Controller *pid, float kp, float ki, float kd, uint16_t setpoint);
uint8_t pid_compute(PID_Controller *pid, uint16_t input, uint8_t current_duty);
void pid_reset(PID_Controller *pid);
void pid_set_limits(PID_Controller *pid, uint8_t min, uint8_t max);

#endif