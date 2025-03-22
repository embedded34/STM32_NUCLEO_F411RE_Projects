# STM32 Project Build and Upload Guide

This guide provides instructions for building and uploading the STM32 project to a target board. The following steps will guide you through the build process, file size details, optional conversion, and upload using OpenOCD and GDB.

## Build Process

To compile and build the project, run the following commands in the terminal:

1. **Compile the main source file** (`main.c`):

   ```bash
   arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -std=gnu11 -Wall main.c -o main.o
   ```

2. **Compile the startup file** (`stm32f411_startup.c`):

   ```bash
   arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -std=gnu11 -Wall stm32f411_startup.c -o stm32f411_startup.o
   ```

3. **Link the object files to create the ELF file**:

   ```bash
   arm-none-eabi-gcc -nostdlib -T stm32f411_linker_script.ld main.o stm32f411_startup.o -o stm32_blink.elf -Wl,-Map=stm32_blink.map
   ```

## Size Details

To check the size of the generated output file, use the following command:

```bash
arm-none-eabi-size stm32_blink.elf
```

## Optional: Convert ELF to BIN Format

If you need to convert the ELF file to a binary `.bin` format, you can use the following command:

```bash
arm-none-eabi-objcopy -O binary stm32_blink.elf stm32_blink.bin
```

## Upload Process

Follow these steps to upload the compiled binary to the STM32 target board:

1. **Start OpenOCD**:

   In one terminal window, run:

   ```bash
   openocd -f board/st_nucleo_f4.cfg
   ```

2. **Connect to GDB**:

   In another terminal window, run:

   ```bash
   arm-none-eabi-gdb
   ```

3. **Connect to the target via OpenOCD**:

   Inside the GDB session, enter the following commands:

   ```bash
   target remote localhost:3333
   monitor reset init
   monitor flash write_image erase stm32_blink.elf
   monitor reset init
   monitor resume
   quit
   ```

4. **Exit GDB**:

   Type `y` to confirm the exit when prompted.

---

> **Note:**  
> Ensure that all the above commands are executed in the directory where the project's files are located.

---
