CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding -mpoke-function-name -mapcs-frame -fno-omit-frame-pointer -fno-math-errno -fsingle-precision-constant -ffast-math
LDFLAGS = -nostdlib -T memmap -L/usr/local/Cellar/arm-none-eabi/4.8.3-cs170e/lib/gcc/arm-none-eabi/4.8.3 -L$(CS107E)/lib

MY_MODULES = timer.o gpio.o strings.o printf.o malloc.o backtrace.o keyboard.o shell.o fb.o gl.o console.o

.PRECIOUS: %.o

test.bin = tests/test_keyboard_interrupts.bin
app.bin = apps/console_shell.bin

app: $(app.bin)

install: $(app.bin)
	rpi-install.py -p $<

test: $(test.bin)

test.install: $(test.bin)
	rpi-install.py -p $<

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o $(MY_MODULES)
	arm-none-eabi-gcc $(LDFLAGS) $^ -lgcc -lpi -o $@

%.o: %.c Makefile
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s Makefile
	arm-none-eabi-as $< -o $@
