# ==========================================
# CONFIGURATION
# ==========================================
MCU = atmega328p
F_CPU = 16000000UL
TARGET = avr_main
SRC = avr_main.c

# Programmer Settings
# Update the PORT to match your Windows COM port (e.g., COM3) if needed
PORT = COM3
BAUD = 19200
PROGRAMMER = usbasp

# Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
OBJDUMP = avr-objdump
NM = avr-nm
READELF = avr-readelf
SIZE = avr-size

# Compiler Flags
# -Os: Optimize for size
# -Wall: Show all warnings
# -save-temps: Saves the .i (preprocessed) and .s (assembly) files
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

# Rule to build the .elf file (Linking Stage)
$(TARGET).elf: $(TARGET).o
	$(CC) $(CFLAGS) $(TARGET).o -o $(TARGET).elf

# Rule to build the .o file (Compiling/Assembling Stage)
$(TARGET).o: $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(TARGET).o

# 2. 'make flash': Uploads the HEX file using USBasp
flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$(TARGET).hex:i

# 3. 'make assembler': Dumps assembly and symbol data from the .o file
assembler: $(TARGET).o
	@echo "--- Extracting Assembler Details ---"
	$(OBJDUMP) -d $(TARGET).o > $(TARGET)_asm_disassembly.txt
	$(NM) -n -S $(TARGET).o > $(TARGET)_asm_symbols.txt
	@echo "Done! Saved to $(TARGET)_asm_disassembly.txt and $(TARGET)_asm_symbols.txt"

# 4. 'make linker': Dumps memory map, sections, and the linker script
linker: $(TARGET).elf
	@echo "--- Extracting Linker Details ---"
	$(READELF) -S $(TARGET).elf > $(TARGET)_link_sections.txt
	$(CC) -mmcu=$(MCU) $(TARGET).o -o $(TARGET).elf "-Wl,--verbose" > $(TARGET)_link_script.ld
	@echo "--- Final Memory Usage ---"
	$(SIZE) $(TARGET).elf
	@echo "Done! Saved blueprint to $(TARGET)_link_sections.txt and $(TARGET)_link_script.ld"

# 5. 'make clean': Deletes all generated files to reset the folder
clean:
	-del /q /f *.i *.s *.o *.elf *.hex *.txt *.ld 2>nul
	@echo "All generated files removed."