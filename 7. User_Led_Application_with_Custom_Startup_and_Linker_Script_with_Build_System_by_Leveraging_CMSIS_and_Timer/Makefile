# Compiler and toolchain definitions
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

# Directories for source code, headers, libraries, build artifacts and other files
SRC_DIR = Source
INC_DIR = Include
LIB_DIR = Libs
BUILD_DIR = Builds
STARTUP_DIR = Startup
LINKER_DIR = Ldscript

# Compilation flags for the C compiler
CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11 -g -Wall -Wextra -O0

# Preprocessor flags (defines and include directories)
CPPFLAGS = -DSTM32F411xE \
           -I$(INC_DIR) \
           -I$(LIB_DIR)/CMSIS/Include \
           -I$(LIB_DIR)/CMSIS/Device/ST/STM32F4xx/Include

# Startup and linker files
STARTUP_FILE = $(STARTUP_DIR)/stm32f411_startup.c
LINKER_FILE = $(LINKER_DIR)/stm32f411_linker_script.ld

# Linker flags for controlling the linking process
LDFLAGS = -nostdlib -T $(LINKER_FILE) -Wl,-Map=$(BUILD_DIR)/stm32_app.map

# The source files for the project
SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(STARTUP_DIR)/*.c)

# Object files corresponding to the source files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c)) \
       $(patsubst $(STARTUP_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(STARTUP_DIR)/*.c))

# Programmer tool and options (using OpenOCD for STM32 flashing)
PROGRAMMER = openocd
PROGRAMMER_FLAGS = -f interface/stlink-v2.cfg -f target/stm32f4x.cfg

# Output files (ELF executable, binary, hex, and listing files)
OUTPUT_ELF = $(BUILD_DIR)/stm32_app.elf
OUTPUT_BIN = $(BUILD_DIR)/stm32_app.bin
OUTPUT_HEX = $(BUILD_DIR)/stm32_app.hex
OUTPUT_LIST = $(BUILD_DIR)/stm32_app.lst

# Targets section
.PHONY : all size disasm flash clean

# Default target (build the entire project)
all : $(OUTPUT_ELF)

# Link object files to create the ELF executable
$(OUTPUT_ELF) : $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $(OUTPUT_ELF)

# Compile individual C files into object files (for both SRC_DIR and STARTUP_DIR)
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

$(BUILD_DIR)/%.o : $(STARTUP_DIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@
	
# Convert the ELF file to a raw binary file for flashing
$(OUTPUT_BIN) : $(OUTPUT_ELF)
	$(OBJCOPY) -O binary $(OUTPUT_ELF) $(OUTPUT_BIN)

# Convert the ELF file to a Intel HEX file (useful for some flashing tools)
$(OUTPUT_HEX) : $(OUTPUT_ELF)
	$(OBJCOPY) -O ihex $(OUTPUT_ELF) $(OUTPUT_HEX)

# Display the size of the ELF file (memory usage analysis)
size : $(OUTPUT_ELF)
	$(SIZE) -A $(OUTPUT_ELF)

# Generate disassembly listing of the ELF file
disasm : $(OUTPUT_ELF)
	$(OBJDUMP) -d $(OUTPUT_ELF) > $(OUTPUT_LIST)

# Flash the program to the STM32 microcontroller using OpenOCD
flash : $(OUTPUT_ELF)
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program $< verify reset exit"

# Clean the build directory (remove object files and output files)
clean :
	@echo "Cleaning up the build directory..."
	rm -rf $(BUILD_DIR)/*		
