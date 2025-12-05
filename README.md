# PWM Generator with PID Control

AVR-based PWM generator with closed-loop PID control and Qt GUI interface.

## Hardware
- ATmega328P @ 12MHz
- PWM output: PD5 (OC1A)
- ADC input: PA0 (ADC0)
- UART: 9600 baud

## Build
```bash
make all
make upload
```

## Embedded Features
- Variable frequency PWM (1-5000 Hz)
- Adjustable duty cycle (0-100%)
- PID controller with configurable setpoint
- Continuous ADC sampling with averaging
- UART command interface

## GUI
Qt-based control panel in `gui/` with serial communication and real-time plotting.

## Commands
Type `I` in serial terminal for full command list.

## License
MIT
