CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -T memmap -L/usr/local/Cellar/arm-none-eabi/4.8.3-cs170e/lib/gcc/arm-none-eabi/4.8.3

.PRECIOUS: %.elf

all: apps/print_pinout.bin

testbin: tests/test_strings_printf.bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o printf.o strings.o gpio.o timer.o uart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ -lgcc -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

install: apps/print_pinout.bin
	rpi-install.py -p $<
.PHONY: install

test: testbin
	rpi-install.py $<
.PHONY: test

gdb: tests/test_strings_printf.bin
	arm-none-eabi-gdb tests/test_strings_printf.elf
.PHONY: gdb

clean:
	rm -f *.o *.bin *.elf
