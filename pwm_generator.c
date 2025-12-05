/*****************************************************************
Commands:
- F <frequency>  : Set PWM frequency (1-5000 Hz)
- D <duty>       : Set duty cycle (0-100%)
- S              : Start PWM output
- T              : Stop PWM output
- R              : Read current ADC value
- I              : Get current PWM info
- X              : Toggle test pin state

Author: Shabd Shrivastava
*****************************************************************/

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "debug.h"
#include "pid.h"

#define F_CPU 12000000UL
#define BUFFSIZE 50
#define MAX_TRAJECTORY 200

// PWM configuration
volatile uint16_t pwm_frequency = 1000;  // Default 1kHz
volatile uint8_t pwm_duty = 50;          // Default 50%
volatile uint8_t pwm_enabled = 0;
volatile uint8_t continuous_sampling = 0; // Continuous ADC sampling flag
PID_Controller matto;
volatile uint8_t pid_enabled = 0;

// Trajectory tracking
volatile uint16_t trajectory[MAX_TRAJECTORY];
volatile uint8_t trajectory_length = 0;
volatile uint8_t trajectory_index = 0;
volatile uint8_t trajectory_mode = 0;

// Function declarations
void update_pwm(void);
void init_adc(void);
uint16_t read_adc(void);
void process_command(char *cmd, int param);

void init_pwm(void) {
    // Configure Timer1 for PWM on OC1A (PD5)
    DDRD |= _BV(PD5);  // Set OC1A as output
    update_pwm();
}

void update_pwm(void) {
    if (pwm_enabled) {
        uint32_t top_value = (F_CPU / (uint32_t)pwm_frequency) - 1;
        
        if (!pid_enabled) {
            printf("[DEBUG] Target freq: %d Hz\n", pwm_frequency);
            printf("[DEBUG] Initial TOP: %lu\n", top_value);
        }
        
        if (top_value <= 65535UL) {
            TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
            if (!pid_enabled) printf("[DEBUG] Using prescaler: 1\n");
        } else {
            top_value = (F_CPU / (8UL * pwm_frequency)) - 1;
            TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);
            if (!pid_enabled) {
                printf("[DEBUG] Using prescaler: 8\n");
                printf("[DEBUG] Adjusted TOP: %lu\n", top_value);
            }
        }
        
        ICR1 = top_value;
        OCR1A = ((uint32_t)top_value * pwm_duty) / 100;
        TCCR1A = _BV(COM1A1) | _BV(WGM11);
        
        if (!pid_enabled) {
            printf("[DEBUG] ICR1 (TOP): %u\n", ICR1);
            printf("[DEBUG] OCR1A (duty): %u\n", OCR1A);
            printf("[DEBUG] TCCR1A: 0x%02X\n", TCCR1A);
            printf("[DEBUG] TCCR1B: 0x%02X\n", TCCR1B);
        }
    } else {
        TCCR1A = 0;
        TCCR1B = 0;
        PORTD &= ~_BV(PD5);
    }
}

void init_adc(void) {
    // Configure ADC for high-quality readings
    ADMUX = _BV(REFS0);  // AVCC reference, ADC0 channel
    
    // ADC prescaler = 64 for optimal accuracy at 12MHz (ADC clock = 187.5kHz)
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADIE);  // Enable ADC interrupt
    sei();  // Enable global interrupts
}



volatile uint16_t latest_adc = 0;
volatile uint8_t sample_counter = 0;
volatile uint32_t adc_accumulator = 0;
volatile uint8_t adc_avg_counter = 0;

ISR(ADC_vect) {
    uint16_t raw_adc = ADC;
    
    // Software averaging
    adc_accumulator += raw_adc;
    adc_avg_counter++;
    
    if (adc_avg_counter >= 100) {
        latest_adc = (uint16_t)(adc_accumulator / 100);
        adc_accumulator = 0;
        adc_avg_counter = 0;
        
        // Run PID after each averaged sample
        if (pid_enabled) {
            sample_counter++;
            if (sample_counter >= 3) {  // 3 * 100 samples * 70us ≈ 21ms
                sample_counter = 0;
                
                // Update setpoint if in trajectory mode
                if (trajectory_mode && trajectory_length > 0) {
                    matto.setpoint = trajectory[trajectory_index];
                    trajectory_index = (trajectory_index + 1) % trajectory_length;
                }
                
                pwm_duty = pid_compute(&matto, latest_adc, pwm_duty);
                update_pwm();
            }
        }
    }

    if (pid_enabled || continuous_sampling) {
        ADCSRA |= _BV(ADSC);
    }
}


uint16_t read_adc(void) {
    uint16_t result;
    cli();
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC));
    result = ADC;
    sei();
    return result;
}

void process_command(char *cmd, int param) {
    switch (cmd[0]) {
        case 'F':  // Set frequency
            if (param >= 1 && param <= 5000) {
                pwm_frequency = param;
                update_pwm();
                printf("Frequency set to %d Hz\n", param);
            } else {
                printf("Error: Frequency must be 1-5000 Hz\n");
            }
            break;
            
        case 'D':  // Set duty cycle
            if (param >= 0 && param <= 100) {
                pwm_duty = param;
                update_pwm();
                printf("Duty cycle set to %d%%\n", param);
            } else {
                printf("Error: Duty cycle must be 0-100%%\n");
            }
            break;
            
        case 'S':  // Start PWM
            pwm_enabled = 1;
            update_pwm();
            printf("PWM started: %d Hz, %d%% duty\n", pwm_frequency, pwm_duty);
            break;
            
        case 'T':  // Stop PWM
            pwm_enabled = 0;
            update_pwm();
            printf("PWM stopped\n");
            break;
            
        case 'R':  // Read ADC once
            {
                uint16_t adc_val = read_adc();
                uint16_t millivolts = (adc_val * 3300UL) / 1024;  // Convert to mV
                printf("ADC: %d (%u.%02uV)\n", adc_val, millivolts/1000, (millivolts%1000)/10);
            }
            break;
            
        case 'M':  // Start continuous monitoring
                pid_enabled = 0;
                sample_counter = 0;
                continuous_sampling = 1;
                printf("Continuous sampling started\n");
                ADCSRA |= _BV(ADSC);  // Start first conversion
            break;
            
        case 'N':  // Stop continuous monitoring
            continuous_sampling = 0;
            printf("Continuous sampling stopped\n");
            break;
            
        case 'X':  // Test pin toggle
            PORTD ^= _BV(PD5);  // Toggle PD5
            if (PORTD & _BV(PD5)) {
                printf("Pin PD5 is now HIGH\n");
            } else {
                printf("Pin PD5 is now LOW\n");
            }
            break;
            
        case 'I':  // Get info
            printf("PWM Info:\n");
            printf("  Status: %s\n", pwm_enabled ? "ON" : "OFF");
            printf("  Frequency: %d Hz (actual)\n", pwm_frequency);
            printf("  Duty Cycle: %d%%\n", pwm_duty);
            printf("  Output Pin: PD5 (OC1A)\n");
            printf("  ADC Channel: PA0 (ADC0)\n");
            if (pwm_enabled) {
                uint32_t actual_freq = F_CPU / ((uint32_t)ICR1 + 1);
                if (TCCR1B & _BV(CS11)) actual_freq /= 8;
                printf("  Actual Freq: %lu Hz\n", actual_freq);
            }
            break;
        case 'P':  // Enable PID
            continuous_sampling = 0;
            pid_enabled = 1;
            pwm_enabled = 1;
            pwm_duty = (matto.setpoint * 100UL) / 1023;
            matto.integral = pwm_duty;
            matto.prev_error = 0;
            printf("PID enabled\n");
            ADCSRA |= _BV(ADSC);
            break;
            
        case 'Q':  // Disable PID
            pid_enabled = 0;
            printf("PID disabled\n");
            break;
            
        case 'V':  // Set setpoint
            if (param >= 0 && param <= 1023) {
                cli();
                matto.setpoint = param;
                matto.prev_error = 0;
                matto.integral = pwm_duty;
                trajectory_mode = 0;  // Disable trajectory when manually setting
                sei();
                printf("Setpoint: %d\n", matto.setpoint);
            } else {
                printf("Error: Setpoint must be 0-1023\n");
            }
            break;
            
        case 'W':  // Start trajectory (W for waveform)
            trajectory_mode = 1;
            trajectory_index = 0;
            printf("Trajectory: %d points loaded\n", trajectory_length);
            break;
            
        case 'E':  // End trajectory mode
            trajectory_mode = 0;
            printf("Trajectory mode disabled\n");
            break;
            
        case 'A':  // Add trajectory point (A for add)
            if (param >= 0 && param <= 1023 && trajectory_length < MAX_TRAJECTORY) {
                trajectory[trajectory_length++] = param;
                printf(".");  // Simple ACK
            }
            break;
            
        case 'C':  // Clear trajectory
            trajectory_length = 0;
            trajectory_index = 0;
            trajectory_mode = 0;
            printf("Trajectory cleared\n");
            break;
            
        case 'Z':  // Reset/stop everything
            pid_enabled = 0;
            continuous_sampling = 0;
            pwm_enabled = 0;
            trajectory_mode = 0;
            update_pwm();
            printf("System reset\n");
            break;
            
        default:
            printf("Unknown command. Available:\n");
            printf("  F <freq>  - Set frequency (1-5000 Hz)\n");
            printf("  D <duty>  - Set duty cycle (0-100%%)\n");
            printf("  S         - Start PWM\n");
            printf("  T         - Stop PWM\n");
            printf("  R         - Read ADC once\n");
            printf("  M         - Start continuous sampling\n");
            printf("  N         - Stop continuous sampling\n");
            printf("  P         - Enable PID\n");
            printf("  Q         - Disable PID\n");
            printf("  V <setpt> - Set PID setpoint\n");
            printf("  A <value> - Add trajectory point\n");
            printf("  W         - Start trajectory tracking\n");
            printf("  E         - End trajectory tracking\n");
            printf("  C         - Clear trajectory\n");
            printf("  X         - Toggle test pin PD5\n");
            printf("  I         - Show info\n");
            break;
    }
}

int main(void) {
    char cmd[BUFFSIZE];
    int param;
    static uint8_t cmd_idx = 0;
    
    init_debug_uart0();
    init_pwm();
    init_adc();
    pid_init(&matto, 0.05, 0.01, 0.0, 512);
    

    
    printf("PWM Generator Ready\n");
    printf("Connect PWM output (PD5) to ADC input (PA0)\n");
    printf("Type 'I' for commands\n");
    
    while (1) {
    // Handle continuous sampling display
    if (continuous_sampling) {
        uint16_t adc_copy;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            adc_copy = latest_adc;
        }
        uint16_t millivolts = (adc_copy * 3300UL) / 1024;
        printf("ADC: %d (%u.%02uV)\n", adc_copy, millivolts/1000, (millivolts%1000)/10);
        _delay_ms(50);
    } else if (pid_enabled) {
        uint16_t adc_copy;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            adc_copy = latest_adc;
        }
        uint16_t millivolts = (adc_copy * 3300UL) / 1024;
        printf("ADC: %d (%u.%02uV)\n", adc_copy, millivolts/1000, (millivolts%1000)/10);
        _delay_ms(50);
    }
    
    // Command processing (non-blocking check) - read all available chars
    while (UCSR0A & _BV(RXC0)) {
        char c = UDR0;
        if (c == '\n') {
            if (cmd_idx > 0 && cmd[0] >= 'A' && cmd[0] <= 'Z') {
                cmd[cmd_idx] = '\0';
                char *numStart = cmd + 1;
                while (*numStart == ' ' || *numStart == '\t') numStart++;
                param = atoi(numStart);
                process_command(cmd, param);
            }
            cmd_idx = 0;
        } else if (c == '\r') {
            // Ignore
        } else if (c >= ' ' && c <= '~') {
            if (cmd_idx < BUFFSIZE - 1) {
                cmd[cmd_idx++] = c;
            } else {
                cmd_idx = 0;
            }
        } else {
            cmd_idx = 0;  // Reset on invalid char
        }
    }
    }

    
    return 0;
}