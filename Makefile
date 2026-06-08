# ==========================================
# CONFIGURATION
# ==========================================
MCU = atmega328p
F_CPU = 16000000UL
TARGET = avr_main

# Directory Structure
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build

# Find all .c files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Translate all .c filenames into .o filenames inside the build directory
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Programmer Settings
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
# -I$(INC_DIR): Tells the compiler where to find your custom .h files!
# -save-temps=obj: Saves intermediate files (.i, .s) right next to the .o files in the build dir
CFLAGS = -Os -Wall -DF_CPU=$(F_CPU) -mmcu=$(MCU) -I$(INC_DIR) -save-temps=obj

# Linker Flags
LDFLAGS = -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

# ==========================================
# BUILD TARGETS
# ==========================================

# Default target
all: compile

# 1. 'make compile': Generates ELF, HEX, and keeps all intermediate files
compile: $(BUILD_DIR)/$(TARGET).hex
	@echo "Compilation complete. Output placed in $(BUILD_DIR)/"

# Rule to build the .hex file from the .elf file
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Rule to build the .elf file (Linking Stage)
# Links ALL object files $(OBJS) together
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Rule to compile .c files to .o files
# The '| $(BUILD_DIR)' ensures the build folder is created before compiling
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist (Windows command)
$(BUILD_DIR):
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

# 2. 'make flash': Uploads the HEX file
flash: $(BUILD_DIR)/$(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$<:i

# 3. 'make assembler': Dumps assembly and symbol data from all .o files
assembler: $(OBJS) | $(BUILD_DIR)
	@echo "--- Extracting Assembler Details ---"
	$(OBJDUMP) -d $(OBJS) > $(BUILD_DIR)/$(TARGET)_asm_disassembly.txt
	$(NM) -n -S $(OBJS) > $(BUILD_DIR)/$(TARGET)_asm_symbols.txt
	@echo "Done! Saved to $(BUILD_DIR)/"

# 4. 'make linker': Dumps memory map, sections, and the linker script
linker: $(BUILD_DIR)/$(TARGET).elf | $(BUILD_DIR)
	@echo "--- Extracting Linker Details ---"
	$(READELF) -S $< > $(BUILD_DIR)/$(TARGET)_link_sections.txt
	$(CC) -mmcu=$(MCU) $(OBJS) -o $< "-Wl,--verbose" > $(BUILD_DIR)/$(TARGET)_link_script.ld
	@echo "--- Final Memory Usage ---"
	$(SIZE) $<
	@echo "Done! Saved blueprint to $(BUILD_DIR)/"

# 5. 'make clean': Deletes the entire build folder
clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@echo "Build directory completely removed."