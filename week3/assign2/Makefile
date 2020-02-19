ARMGNU ?= arm-none-eabi

ASFLAGS = --warn --fatal-warnings
CFLAGS = -g -Wall -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib

all : clock.bin

install: clock.bin
	rpi-install.py clock.bin

clock.bin : memmap start.o gpio.o clock.o cstart.o timer.o
	$(ARMGNU)-ld $(LDFLAGS) start.o cstart.o gpio.o timer.o clock.o -T memmap -o clock.elf
	$(ARMGNU)-objdump -D clock.elf > clock.list
	$(ARMGNU)-objcopy clock.elf -O binary clock.bin

start.o : start.s
	$(ARMGNU)-as $(ASFLAGS) start.s -o start.o

clock.o : apps/clock.c
	$(ARMGNU)-gcc $(CFLAGS) -c apps/clock.c -o clock.o

gpio.o : gpio.c 
	$(ARMGNU)-gcc $(CFLAGS) -c gpio.c -o gpio.o

cstart.o : cstart.c
	$(ARMGNU)-gcc $(CFLAGS) -c cstart.c -o cstart.o

timer.o : timer.c
	$(ARMGNU)-gcc $(CFLAGS) -c timer.c -o timer.o

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.srec
	rm -f *.elf
	rm -f *.list
	rm -f *.img
