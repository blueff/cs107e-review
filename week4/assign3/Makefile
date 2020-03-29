CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -T memmap -L/usr/local/Cellar/arm-none-eabi/4.8.3-cs170e/lib/gcc/arm-none-eabi/4.8.3

.PRECIOUS: %.elf

test.bin = tests/test_strings_printf.bin
print.bin = apps/print_pinout.bin
disassemble.bin = apps/disassemble.bin

all: $(print.bin)
test.bin: $(test.bin)
disassemble.bin: $(disassemble.bin)

disassemble: $(disassemble.bin)
	rpi-install.py -p $<

disassemble.gdb: $(disassemble.bin)
	arm-none-eabi-gdb apps/disassemble.elf

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o printf.o strings.o gpio.o timer.o uart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ -lgcc -o $@
	arm-none-eabi-objdump -D $@ > $@.list

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

install: $(print.bin)
	rpi-install.py -p $<
.PHONY: install

test: $(test.bin)
	rpi-install.py $<
.PHONY: test

test.gdb: $(test.bin)
	arm-none-eabi-gdb tests/test_strings_printf.elf
.PHONY: gdb

clean:
	rm -f *.o *.bin *.elf
