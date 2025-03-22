# Define user-defined variables
CC = arm-none-eabi-gcc
CFLAGS = -c -mcpu=cortex-m4 -mthumb -std=gnu11 -Wall
LDFLAGS = -nostdlib -T stm32f411_linker_script.ld -Wl,-Map=stm32_blink.map

# Create the final target
final : stm32_blink.elf 

# Create an object file for main.c
main.o : main.c
	$(CC) $(CFLAGS) $^ -o $@
	# arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -std=gnu11 -Wall main.c -o main.o

# Create an object file for stm32f411_startup.c
stm32f411_startup.o : stm32f411_startup.c
	$(CC) $(CFLAGS) $^ -o $@
	# arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -std=gnu11 -Wall stm32f411_startup.c -o stm32f411_startup.o

# Link to create the final executable
stm32_blink.elf : main.o stm32f411_startup.o
	$(CC) $(LDFLAGS) $^ -o $@
	# arm-none-eabi-gcc -nostdlib -T stm32f411_linker_script.ld *.o -o stm32_blink.elf -Wl,-Map=stm32_blink.map

# Clean the target	
clean:
	rm -f *.o *.elf *.map