CROSS_COMPILER ?= arm-none-eabi-
CC:= $(CROSS_COMPILER)gcc
CFLAG = -std=gnu99 \
	 -Wall -Werror \
	 -mcpu=cortex-m4\
	 -g -mthumb\
	 -Wl,-T main.ld -nostartfiles
TARGET := ./main.bin
SOURCE := ./source/*.c
USB := ./usb/source/*.c
INCDIR := -Iusb/source/

all: $(TARGET)
$(TARGET):$(SOURCE) $(USB) 
	$(CC) $(INCDIR) $(CFLAG)  -o main.elf  $^
	$(CROSS_COMPILER)objcopy -Obinary main.elf $(TARGET)
	$(CROSS_COMPILER)objdump -x -S main.elf > main.list
	@echo "build complete"
dbg:
	.\debug.bat
clean:
	rm -f *.o *.bin *.elf *.list