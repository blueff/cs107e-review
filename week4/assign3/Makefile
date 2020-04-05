CFLAGS = -I$(CS107E)/include -g -Wall -Wno-format -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -T memmap -L/usr/local/Cellar/arm-none-eabi/4.8.3-cs170e/lib/gcc/arm-none-eabi/4.8.3

.PRECIOUS: %.elf

test.bin = tests/test_strings_printf.bin
print.bin = apps/print_pinout.bin
disassemble_self.bin = apps/disassemble_self.bin
test_disassemble.bin = tests/test_disassemble.bin

all: $(print.bin)
test.bin: $(test.bin)
disassemble_self.bin: $(disassemble.bin)

disassemble_self: $(disassemble_self.bin)
	rpi-install.py -p $<

disassemble_self.gdb: $(disassemble_self.bin)
	arm-none-eabi-gdb apps/disassemble_self.elf

test_disassemble: $(test_disassemble.bin)
	rpi-install.py -p $<

test_disassemble.gdb: $(test_disassemble.bin)
	arm-none-eabi-gdb tests/test_disassemble.elf

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o printf.o strings.o gpio.o timer.o uart.o disassemble.o assert.o
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
