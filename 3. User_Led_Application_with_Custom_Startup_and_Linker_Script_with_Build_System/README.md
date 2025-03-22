# STM32 Project Build and Upload Guide

This guide provides instructions for building and uploading the STM32 project to a target board. The following steps will guide you through the build process, and upload using OpenOCD and GDB.

## Build Process

To compile and build the project, run the following command in the terminal:

```bash
make final
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

## Clean Process

To clean the build artifacts, run:

```bash
make clean
```
---

> **Note:**  
> Ensure that all the above commands are executed in the directory where the project's `Makefile` is located.

---
