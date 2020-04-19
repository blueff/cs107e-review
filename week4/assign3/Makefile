CFLAGS = -I$(CS107E)/include -g -Wall -Wno-format -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -T memmap -L/usr/local/Cellar/arm-none-eabi/4.8.3-cs170e/lib/gcc/arm-none-eabi/4.8.3

.PRECIOUS: %.elf

common_objects = start.o cstart.o strings.o gpio.o timer.o uart.o disassemble.o assert.o

test.bin = tests/test_strings_printf.bin
print.bin = apps/print_pinout.bin
disassemble_self.bin = apps/disassemble_self.bin
test_disassemble.bin = tests/test_disassemble.bin

all: $(print.bin)

install: $(print.bin)
	rpi-install.py -p $<
.PHONY: install

test.bin: $(test.bin)

test: $(test.bin)
	rpi-install.py $<
.PHONY: test

test.gdb: $(test.bin)
	arm-none-eabi-gdb tests/test_strings_printf.elf
.PHONY: test.gdb

disassemble_self.bin: $(disassemble_self.bin)

disassemble_self: $(disassemble_self.bin)
	rpi-install.py -p $<
.PHONY: disassemble_self

disassemble_self.gdb: $(subst .bin,.gdb.bin,$(disassemble_self.bin))
	arm-none-eabi-gdb $(subst .bin,.elf, $<)
.PHONY: disassemble_self.gdb

test_disassemble: $(test_disassemble.bin)
	rpi-install.py $<
.PHONY: test_disassemble

test_disassemble.gdb: $(test_disassemble.bin)
	arm-none-eabi-gdb $(subst .bin,.elf,$<)
.PHONY: test_disassemble.gdb

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.gdb.elf: %.o $(common_objects) printf.gdb.o
	arm-none-eabi-gcc $(LDFLAGS) $^ -lgcc -o $@
	arm-none-eabi-objdump -D $@ > $@.list

%.elf: %.o $(common_objects) printf.o
	arm-none-eabi-gcc $(LDFLAGS) $^ -lgcc -o $@
	arm-none-eabi-objdump -D $@ > $@.list

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.gdb.o: %.c
	arm-none-eabi-gcc -DGDB_DEBUG=1 $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

clean:
	rm -f *.o *.bin *.elf
.PHONY: clean
