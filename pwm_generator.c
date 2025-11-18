/*****************************************************************

High-quality PWM generator for Il-Matto that accepts serial commands
and outputs corresponding PWM waves for ADC measurement.

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
#include "debug.h"

#define F_CPU 12000000UL
#define BUFFSIZE 50

// PWM configuration
volatile uint16_t pwm_frequency = 1000;  // Default 1kHz
volatile uint8_t pwm_duty = 50;          // Default 50%
volatile uint8_t pwm_enabled = 0;
volatile uint8_t continuous_sampling = 0; // Continuous ADC sampling flag

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
        // Variable frequency PWM using ICR1 as TOP
        uint32_t top_value = (F_CPU / (uint32_t)pwm_frequency) - 1;
        uint8_t prescaler;
        
        printf("[DEBUG] Target freq: %d Hz\n", pwm_frequency);
        printf("[DEBUG] Initial TOP: %lu\n", top_value);
        
        // Select appropriate prescaler
        if (top_value <= 65535UL) {

            prescaler = 1;
            TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);  // Prescaler = 1
            printf("[DEBUG] Using prescaler: 1\n");
        } else {
            top_value = (F_CPU / (8UL * pwm_frequency)) - 1;
            prescaler = 8;
            TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);  // Prescaler = 8
            printf("[DEBUG] Using prescaler: 8\n");
            printf("[DEBUG] Adjusted TOP: %lu\n", top_value);
        }
        
        ICR1 = top_value;
        OCR1A = ((uint32_t)top_value * pwm_duty) / 100;  // Set duty cycle
        TCCR1A = _BV(COM1A1) | _BV(WGM11);  // Fast PWM mode 14, clear on match
        
        printf("[DEBUG] ICR1 (TOP): %u\n", ICR1);
        printf("[DEBUG] OCR1A (duty): %u\n", OCR1A);
        printf("[DEBUG] TCCR1A: 0x%02X\n", TCCR1A);
        printf("[DEBUG] TCCR1B: 0x%02X\n", TCCR1B);
        
        uint32_t actual_freq = F_CPU / ((uint32_t)ICR1 + 1);
        if (prescaler == 8) actual_freq /= 8;
        printf("[DEBUG] Actual freq: %lu Hz\n", actual_freq);
    } else {
        printf("[DEBUG] PWM disabled\n");
        TCCR1A = 0;  // Disable PWM
        TCCR1B = 0;  // Stop timer
        PORTD &= ~_BV(PD5);  // Set output low
    }
}

void init_adc(void) {
    // Configure ADC for high-quality readings
    ADMUX = _BV(REFS0);  // AVCC reference, ADC0 channel
    
    // ADC prescaler = 64 for optimal accuracy at 12MHz (ADC clock = 187.5kHz)
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADIE);  // Enable ADC interrupt
    sei();  // Enable global interrupts
}

ISR(ADC_vect) {
    // ADC conversion complete
    if (continuous_sampling) {
        uint16_t adc_val = ADC;
        uint16_t millivolts = (adc_val * 3300UL) / 1024;
        printf("ADC: %d (%u.%02uV)\n", adc_val, millivolts/1000, (millivolts%1000)/10);
        _delay_ms(100);  // 100ms between samples
        ADCSRA |= _BV(ADSC);  // Start next conversion
    }
}

uint16_t read_adc(void) {
    ADCSRA |= _BV(ADSC);  // Start conversion
    while (ADCSRA & _BV(ADSC));  // Wait for completion
    return ADC;  // Return 10-bit result
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
            
        default:
            printf("Unknown command. Available:\n");
            printf("  F <freq>  - Set frequency (1-5000 Hz)\n");
            printf("  D <duty>  - Set duty cycle (0-100%%)\n");
            printf("  S         - Start PWM\n");
            printf("  T         - Stop PWM\n");
            printf("  R         - Read ADC once\n");
            printf("  M         - Start continuous sampling\n");
            printf("  N         - Stop continuous sampling\n");
            printf("  I         - Show info\n");
            break;
    }
}

int main(void) {
    char cmd[BUFFSIZE];
    int param;
    int res;
    
    init_debug_uart0();
    init_pwm();
    init_adc();
    
    printf("PWM Generator Ready\n");
    printf("Connect PWM output (PD5) to ADC input (PA0)\n");
    printf("Type 'I' for commands\n");
    
    while (1) {
        res = scanf("%s %d", cmd, &param);
        
        if (res == 1) {
            // Single command without parameter
            process_command(cmd, 0);
        } else if (res == 2) {
            // Command with parameter
            process_command(cmd, param);
        } else {
            scanf("%*s");  // Clear input buffer
        }
    }
    
    return 0;
}