CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
STFLASH = st-flash

CFLAGS = -mcpu=cortex-m4 -mthumb -Os -g -Wall -Wextra
LDFLAGS = -nostartfiles -Tstm32f429.ld

TARGET = main
OBJS = main.o startup.o

all: $(TARGET).bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

flash: $(TARGET).bin
	$(STFLASH) write $< 0x8000000

clean:
	rm -f *.o *.elf *.bin
