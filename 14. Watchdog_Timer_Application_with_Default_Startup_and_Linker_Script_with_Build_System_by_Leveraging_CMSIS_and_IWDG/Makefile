# ============================
# Project Configuration
# ============================

# Name of the firmware project
PROJECT_NAME := stm32_app

# ============================
# Toolchain Configuration
# ============================

# Compiler and related toolchain binaries
CC      := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE    := arm-none-eabi-size

# ============================
# Directory Structure
# ============================

# Directories for source code, headers, libraries, build artifacts and other files
SRC_DIR     := Source
INC_DIR     := Include
LIB_DIR     := Libs
STARTUP_DIR := Startup
LINKER_DIR  := Ldscript
BUILD_DIR   := Builds

# ============================
# Startup and Linker Files
# ============================

# Startup assembly and linker script
STARTUP_S     := $(STARTUP_DIR)/startup_stm32f411retx.s
LINKER_SCRIPT := $(LINKER_DIR)/STM32F411RETX_FLASH.ld

# ============================
# Output Files
# ============================

# Target output files (ELF executable, binary, hex, listing files, and more)
OUTPUT_ELF   := $(BUILD_DIR)/$(PROJECT_NAME).elf
OUTPUT_BIN   := $(BUILD_DIR)/$(PROJECT_NAME).bin
OUTPUT_HEX   := $(BUILD_DIR)/$(PROJECT_NAME).hex
OUTPUT_LIST  := $(BUILD_DIR)/$(PROJECT_NAME).list
OUTPUT_MAP   := $(BUILD_DIR)/$(PROJECT_NAME).map
SIZE_FILE    := $(BUILD_DIR)/default.size.stdout
OBJECTS_LIST := $(BUILD_DIR)/objects.list

# ============================
# Compilation Flags
# ============================

# MCU-specific compilation flags
MCU_FLAGS := -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb

# Preprocessor flags (defines)
DEFS := -DDEBUG -DNUCLEO_F411RE -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE

# Preprocessor flags (include directories)
COMMON_INCLUDES := -I$(INC_DIR) \
				   -I$(LIB_DIR)/CMSIS/Include \
    			   -I$(LIB_DIR)/CMSIS/Device/ST/STM32F4xx/Include
    			   
# C preprocessor flags
CPPFLAGS := $(DEFS) $(COMMON_INCLUDES)

# C compiler flags
CFLAGS := -std=gnu11 -O0 -Wall -Wextra -g3 -ffunction-sections -fdata-sections \
          -fstack-usage -MMD -MP --specs=nano.specs \
          $(MCU_FLAGS) $(CPPFLAGS)

# Assembler flags
ASFLAGS := -x assembler-with-cpp -MMD -MP --specs=nano.specs \
           $(MCU_FLAGS) $(CPPFLAGS)

# Linker flags for controlling the linking process
LDFLAGS := -T$(LINKER_SCRIPT) $(MCU_FLAGS) \
           --specs=nosys.specs --specs=nano.specs \
           -Wl,-Map=$(OUTPUT_MAP) -Wl,--gc-sections -static -u _printf_float \
           -Wl,--start-group -lc -lm -Wl,--end-group

# ============================
# Source and Object Files
# ==========================

# C source and object files
C_SOURCES := $(wildcard $(SRC_DIR)/*.c)
C_OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))

# Assembly object file
ASM_OBJECT := $(patsubst $(STARTUP_DIR)/%.s, $(BUILD_DIR)/%.o, $(wildcard $(STARTUP_DIR)/*.s))

# All object files
OBJECTS := $(C_OBJECTS) $(ASM_OBJECT)

# ============================
# Build Targets
# ============================
.PHONY: all size disasm bin hex flash clean

# Default target: build the project, show size, and generate disassembly
all: $(OUTPUT_ELF) size disasm

# Create build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $@

# Compile individual C files into object files (for SRC_DIR files)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile individual assembly files into object files (for STARTUP_DIR files)
$(ASM_OBJECT): $(STARTUP_S) | $(BUILD_DIR)
	$(CC) $(ASFLAGS) -c $< -o $@

# Link object files to create the final ELF executable
$(OUTPUT_ELF): $(OBJECTS)
	@echo $(OBJECTS) > $(OBJECTS_LIST)
	$(CC) $(LDFLAGS) @$(OBJECTS_LIST) -o $@
	@echo "✔ Built ELF: $@"

# Display the size report of the ELF file (memory usage analysis)
size: $(OUTPUT_ELF)
	@echo
	$(SIZE) -A $< > $(SIZE_FILE)
	@echo "✔ Size info: $(SIZE_FILE)"
	$(SIZE) $<

# Generate disassembly listing from the ELF file
disasm: $(OUTPUT_ELF)
	@echo
	$(OBJDUMP) -h -S $< > $(OUTPUT_LIST)
	@echo "✔ Disassembly listing: $(OUTPUT_LIST)"

# # Convert the ELF file to a raw binary and Intel HEX file formats (optional)
bin: $(OUTPUT_BIN)
hex: $(OUTPUT_HEX)

$(OUTPUT_BIN): $(OUTPUT_ELF)
	$(OBJCOPY) -O binary $< $@

$(OUTPUT_HEX): $(OUTPUT_ELF)
	$(OBJCOPY) -O ihex $< $@

# Flash the firmware to the microcontroller using OpenOCD
flash: $(OUTPUT_ELF)
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $< verify reset exit"

# Clean the build directory (remove all build artifacts)
clean:
	@echo "🧹 Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

# ============================
# Dependency Management
# ============================

# Include automatically generated dependency files
-include $(C_OBJECTS:.o=.d)
-include $(ASM_OBJECT:.o=.d)