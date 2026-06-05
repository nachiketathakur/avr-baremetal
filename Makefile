# ==========================================
# CONFIGURATION
# ==========================================
MCU = atmega328p
F_CPU = 16000000UL
TARGET = avr_main
SRC = avr_main.c

# Programmer Settings (Arduino as ISP)
# Update the PORT to match your Windows COM port (e.g., COM3)
PORT = COM3
BAUD = 19200
PROGRAMMER = usbasp

# Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Compiler Flags
# -Os: Optimize for size
# -Wall: Show all warnings
# -save-temps: Saves the .i (preprocessed), .s (assembly), and .o (object) files
CFLAGS = -Os -Wall -DF_CPU=$(F_CPU) -mmcu=$(MCU) -save-temps

# ==========================================
# BUILD TARGETS
# ==========================================

# Default target if you just type 'make'
all: compile

# 1. 'make compile': Generates ELF, HEX, and keeps all intermediate files
compile: $(TARGET).hex
	@echo "Compilation complete. Intermediate files saved."

# Rule to build the .hex file from the .elf file
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex

# Rule to build the .elf file (and trigger -save-temps for .i, .s, .o)
$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET).elf

# 2. 'make flash': Uploads the HEX file using Arduino as ISP
flash: $(TARGET).hex
	 $(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$(TARGET).hex:i

# 3. 'make clean': Deletes all generated files to reset the folder
clean:
	rm -f *.i *.s *.o *.elf *.hex
	@echo "All generated files removed."