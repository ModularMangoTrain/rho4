# Makefile for PWM Generator Project
# Author: Amazon Q

# AVR Configuration
MCU = atmega644p
F_CPU = 12000000UL
AVRDUDE_MCU = m644p
AVRDUDE_PROGRAMMER = usbasp
AVRDUDE_PORT = usb

# Compiler settings
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Os -std=c99
LDFLAGS = -Wl,-u,vfprintf -lprintf_flt -lm

# Host compiler (Windows/Linux)
HOST_CC = gcc
HOST_CFLAGS = -Wall -O2

# Targets
all: pwm_generator.hex 

# AVR embedded program
pwm_generator.elf: pwm_generator.c pid.c
	$(CC) $(CFLAGS) -o $@ $^

pwm_generator.hex: pwm_generator.elf
	$(OBJCOPY) -O ihex $< $@
	avr-size pwm_generator.elf



# Upload to AVR
upload: pwm_generator.hex
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMER) -p $(AVRDUDE_MCU) -P $(AVRDUDE_PORT) -U flash:w:$<

# Clean build files
clean:
	rm -f *.elf *.hex *.exe *.o

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build both embedded and host programs"
	@echo "  upload   - Upload embedded program to AVR"
	@echo "  clean    - Remove build files"
	@echo "  help     - Show this help"

.PHONY: all upload clean help