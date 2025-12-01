#ifndef PID_ABSOLUTE_H
#define PID_ABSOLUTE_H

#include <stdint.h>

typedef struct {
    float kp;
    float ki;
    float kd;
    float setpoint;
    float integral;
    float prev_error;
    float output_min;
    float output_max;
} PID_Absolute;

void pid_absolute_init(PID_Absolute *pid, float kp, float ki, float kd, float setpoint);
void pid_absolute_reset(PID_Absolute *pid);
uint8_t pid_absolute_compute(PID_Absolute *pid, uint16_t input);

#endif
