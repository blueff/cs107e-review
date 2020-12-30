<div align="center">
  <h1>
    Computer Systems from the Ground Up
  </h1>
</div>

My study notes of the awesome [CS107E](http://cs107e.github.io/) (Winter 2020) course.

Since the course is keeping going and keeping changing, the notes here may not correspond with the latest course material.

To follow along the notes, you can use the bundled course version [winter 2020](http://cs107e.cjting.me) (based on `1fc22e4` of cs107e repo).

## Setup

```bash
$ git clone https://github.com/blueff/cs107e-review
$ cd cs107e-review
$ source ./setup.sh
$ # `doc` contains all materials of CS107E winter 2020
$ http-server -p 4000 doc
# Open browser at localhost:4000 and enjoy!
```

## Table of Contents

<!-- MarkdownTOC -->

- [Week 1: Introduction and Welcome](#week-1-introduction-and-welcome)
  - [Raspberry Pi](#raspberry-pi)
  - [ARM processor and architecture](#arm-processor-and-architecture)
  - [Assignment 0: Choose Lab Section, Learn Git](#assignment-0-choose-lab-section-learn-git)
- [Week 2: ARM assembly and machine code](#week-2-arm-assembly-and-machine-code)
  - [Lab 1: Setup your Raspberry Pi](#lab-1-setup-your-raspberry-pi)
    - [Blink](#blink)
    - [Button](#button)
  - [Assignment 1: Implement a Larson Scanner](#assignment-1-implement-a-larson-scanner)
    - [A simple Larson Scanner](#a-simple-larson-scanner)
    - [Extension: Different Levels of Brightness](#extension-different-levels-of-brightness)
  - [From Assembly to C](#from-assembly-to-c)
- [Week 3: C Pointers and Arrays](#week-3-c-pointers-and-arrays)
  - [Lab 2: Below C Level](#lab-2-below-c-level)
    - [C to assembly](#c-to-assembly)
    - [Makefiles](#makefiles)
    - [Testing](#testing)
    - [Wire up display breadborad](#wire-up-display-breadborad)
  - [Assignment 2: Implement a Clock](#assignment-2-implement-a-clock)
    - [A Clock](#a-clock)
    - [Extension: Set Time](#extension-set-time)
- [Week 4: Communication and the Serial Protocol](#week-4-communication-and-the-serial-protocol)
  - [Lab 3: Debugging and Testing](#lab-3-debugging-and-testing)
    - [Debugging with gdb](#debugging-with-gdb)
    - [Serial communication](#serial-communication)
    - [C-strings](#c-strings)
    - [Gdb and testing](#gdb-and-testing)
  - [Assignment 3: Implement a String Formatting Library](#assignment-3-implement-a-string-formatting-library)
    - [Strings module](#strings-module)
    - [Number to string conversion helper functions](#number-to-string-conversion-helper-functions)
    - [Implement snprintf, vsnprintf and printf](#implement-snprintf-vsnprintf-and-printf)
    - [Extension: Disassembler](#extension-disassembler)
  - [Modules, Libraries, and Linking](#modules-libraries-and-linking)
- [Week 5: Memory Management](#week-5-memory-management)
  - [Lab 4: Linked and Loaded](#lab-4-linked-and-loaded)
    - [Linking](#linking)
    - [Memory Map](#memory-map)
    - [Bootloader](#bootloader)
    - [Stack](#stack)
    - [Heap](#heap)
    - [Check-in Question](#check-in-question)
  - [Assignment 4: Backtrace and Malloc](#assignment-4-backtrace-and-malloc)
    - [Prepare starter files](#prepare-starter-files)
    - [Backtrace Module](#backtrace-module)
    - [Malloc Module](#malloc-module)
    - [Extension: Mini-Valgrind](#extension-mini-valgrind)
  - [C Mastery](#c-mastery)
- [Week 6: Keyboards and the PS/2 Protocol](#week-6-keyboards-and-the-ps2-protocol)
  - [Lab 5: Keyboard Surfin'](#lab-5-keyboard-surfin)
    - [Check-in questions](#check-in-questions)
  - [Assignment 5: Keyboard and Simple Shell](#assignment-5-keyboard-and-simple-shell)
    - [Prepare starter files](#prepare-starter-files-1)
    - [Keyboard driver](#keyboard-driver)
    - [Simple shell](#simple-shell)
    - [Extension: editing and history](#extension-editing-and-history)
  - [Graphics and the framebuffer](#graphics-and-the-framebuffer)
- [Week 7](#week-7)
  - [Lab 6: Drawing into the Framebuffer](#lab-6-drawing-into-the-framebuffer)
    - [Study fb and mailbox code](#study-fb-and-mailbox-code)
    - [Multidimensional pointers](#multidimensional-pointers)
    - [Fonts](#fonts)
    - [Check-in](#check-in)
  - [Assignment 6: Graphics Library and Console](#assignment-6-graphics-library-and-console)
    - [Prepare starter files](#prepare-starter-files-2)
    - [Framebuffer](#framebuffer)
    - [Graphics primitives](#graphics-primitives)
    - [Fonts and text-drawing](#fonts-and-text-drawing)
    - [Console](#console)
    - [Extension: Line and triangle drawing](#extension-line-and-triangle-drawing)
  - [Interrupts](#interrupts)
- [Week 8: Interrupts, continued](#week-8-interrupts-continued)
  - [Lab 7: Raspberry Pi, Interrupted](#lab-7-raspberry-pi-interrupted)
    - [Review interrupts code](#review-interrupts-code)
    - [Set up a button circuit](#set-up-a-button-circuit)
    - [Write an interrupt handler](#write-an-interrupt-handler)
    - [Use a ring buffer queue](#use-a-ring-buffer-queue)
    - [Need for speed](#need-for-speed)
  - [Assignment 7: System Monitor with Interrupts](#assignment-7-system-monitor-with-interrupts)
    - [Prepare Starter Files](#prepare-starter-files-3)
    - [Basic Part](#basic-part)
    - [Need for speed?](#need-for-speed-1)
    - [Extension: Profiler](#extension-profiler)
- [Raspberry Pi Tips](#raspberry-pi-tips)
- [ARM Tips](#arm-tips)
  - [Disassemble object file](#disassemble-object-file)
  - [Disassemble binary file](#disassemble-binary-file)
  - [Disassemble one single instruction](#disassemble-one-single-instruction)
- [???](#)
- [GCC Tips](#gcc-tips)
  - [`-mpoke-function-name`](#-mpoke-function-name)
  - [Inline Assembly](#inline-assembly)

<!-- /MarkdownTOC -->

## Week 1: Introduction and Welcome

[List of electronic parts we will use in this course](https://cs107e.github.io/guides/bom/).

### Raspberry Pi

- [Pinout.xyz](https://pinout.xyz/): online website
- [Pinout.pdf](http://cs107e.github.io/guides/images/pinout.pdf): PDF document

The Raspberry Pi model we are gonna use is [Raspberry Pi 1 Model A+](https://www.raspberrypi.org/products/raspberry-pi-1-model-a-plus/).

Must stick with this version otherwise the code might not work.

The CPU model of the Pi is [BCM2835](https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/README.md) (ARM11 series, ARMv6 architecture).

Check the manual here [BCM2835 Peripherals Specification](https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf).

Please note that this manual __has a lot of errors__, must read it with [BCM 2835 Datasheet Errata](https://elinux.org/BCM2835_datasheet_errata#p96).

NOTE: __The address in the manual `0x7E...` is the logic address. We will change it to `0x20...` in the code__.

### ARM processor and architecture

- [Intro to ARM assembly](http://www.toves.org/books/arm/)
- [Awesome ARM tutorail from CSIE@NTU](https://www.csie.ntu.edu.tw/~cyy/courses/assembly/09fall/lectures/handouts/lec09_ARMisa.pdf)
- [VisUAL](https://salmanarif.bitbucket.io/visual/index.html): Visual ARM emulator

First, we need to install the arm toolchain for our Pi (compiler, assembler, linker, etc..) and the CP2012 driver. Check the [Installation Guide](http://cs107e.github.io/guides/install/).

After installation, we should have a bunch of `arm-none-eabi` tools.

```bash
cs107e git:(master) ❯ whence -a -m 'arm-none-eabi*'
/usr/local/bin/arm-none-eabi-addr2line
/usr/local/bin/arm-none-eabi-ar
/usr/local/bin/arm-none-eabi-as
/usr/local/bin/arm-none-eabi-c++filt
/usr/local/bin/arm-none-eabi-cpp
/usr/local/bin/arm-none-eabi-elfedit
/usr/local/bin/arm-none-eabi-gcc
/usr/local/bin/arm-none-eabi-gcc-4.8.3
/usr/local/bin/arm-none-eabi-gcc-ar
/usr/local/bin/arm-none-eabi-gcc-nm
/usr/local/bin/arm-none-eabi-gcc-ranlib
/usr/local/bin/arm-none-eabi-gcov
/usr/local/bin/arm-none-eabi-gdb
/usr/local/bin/arm-none-eabi-gprof
/usr/local/bin/arm-none-eabi-ld
/usr/local/bin/arm-none-eabi-ld.bfd
/usr/local/bin/arm-none-eabi-nm
/usr/local/bin/arm-none-eabi-objcopy
/usr/local/bin/arm-none-eabi-objdump
/usr/local/bin/arm-none-eabi-ranlib
/usr/local/bin/arm-none-eabi-readelf
/usr/local/bin/arm-none-eabi-run
/usr/local/bin/arm-none-eabi-size
/usr/local/bin/arm-none-eabi-strings
/usr/local/bin/arm-none-eabi-strip
```

What is the meaning of `arm-none-eabi`? It turns out that:

> Unix cross compilers are loosely named using a convention of the form `arch[-vendor][-os]-abi`.

> The arch refers to the target architecture, which in our case is ARM. The vendor nominally refers to the
toolchain supplier. The os refers to the target operating system, if any, and is used to decide which libraries
(e.g. newlib, glibc, crt0, etc.) to link and which syscall conventions to employ. The abi specifies which
application binary interface convention is being employed, which ensures that binaries generated by different
tools can interoperate.

So `none` menas we are not targeting any operating system here, aka we are in "bare metal". And `eabi` means the ABI for the ARM architecture.

### Assignment 0: Choose Lab Section, Learn Git

This is for the official students, I have nothing to do here 😜.

## Week 2: ARM assembly and machine code

[ARM Instruction Set Architecture](http://cs107e.github.io/readings/armisa.pdf).

From this documentation, we can know that __every ARM instruction can be conditionally executed__. This is a big difference from X86.

[ARM immediate value encoding](https://alisdair.mcdiarmid.org/arm-immediate-value-encoding/#play-with-it).

> The ARM instruction set encodes immediate values in an unusual way. It's typical of the design of the processor architecture: elegant, pragmatic, and quirky. Despite only using 12 bits of instruction space, the immediate value can represent a useful set of 32-bit constants.

Basically, instead of using 12-bit to represent a number, ARM uses 8-bit for the number and 4-bit for rotation. By using this approach, it can represent a large set of useful 32-bit values.

### Lab 1: Setup your Raspberry Pi

#### Blink

Let's give the world some light!

![](./assets/blink.gif)

#### Button

Tada! 🎉 What a fun button ever! NOTE: We need to have a 10k pull-up resistor. If you are not familiar with this concept, this is a [fine explanation](https://learn.sparkfun.com/tutorials/pull-up-resistors/all).

![](./assets/button.gif)

### Assignment 1: Implement a Larson Scanner

#### A simple Larson Scanner

1. Configure GPIO 20 ~ 27 to output mode
2. Set current gpio pin for a while then clear it
3. Update current gpio pin, need to handle the edge case
4. Go back to step 2

Here is the code [larson.s](./week2/assign1/larson.s).

![](./assets/larson.gif)

#### Extension: Different Levels of Brightness

First, we need a way to control brightness. I thought about directly configuring the output voltage for a GPIO pin but it seems impossible.

So Let's do it the other way. We can control the time of the high level in a period to control the power supplied. The more power, the brighter LED. Actually this is a standard technique called [Pulse Width Modulation](https://en.wikipedia.org/wiki/Pulse-width_modulation).

Let's try that, implement a simple test in [brightness.s](./week2/assign1/brightness.s).

We decrease the duty cycle (time of high level in the period) for GPIO 20, 21 and 22 one by one and see if their brightness has any difference.

![](./assets/brightness.jpeg)

Yes they do! So the implementation of extended Larson scanner is obvious now.

Check the code [larson-extended.s](./week2/assign1/larson-extended.s).

![](./assets/larson-extended.gif)

### From Assembly to C

- [Compiler Explorer](https://gcc.godbolt.org): A neat interactive tool to see translation from C to assembly

After writing some ARM assembly code and switching to using C, this is the first time I ever think that C is so so so superior and powerful!

Long live C!

> C is quirky, flawed, and an enormous success — Dennis Ritchie

> C gives the programmer what the programmer wants; few
restrictions, few complaints — Herbert Schildt

> C: A language that combines all the elegance and power of assembly
language with all the readability and maintainability of assembly
language — Unknown

A sample Makefile for compiling base-metal C for ARM:

```makefile
NAME = myprogram
CFLAGS = -Og -Wall -std=c99 -ffreestanding
LDFLAGS = -nostdlib -e main

.PRECIOUS: %.elf %.o

all : $(NAME).bin

%.bin: %.elf
	 arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o
	 arm-none-eabi-ld $(LDFLAGS) $< -o $@

%.o: %.c
	 arm-none-eabi-gcc $(CFLAGS) -c $< -o $@
```

## Week 3: C Pointers and Arrays

About NULL-terminated strings, Poul-Henning Kamp wrote an essay [The Most Expensive One-byte Mistake](https://queue.acm.org/detail.cfm?id=2010365).

Does C do it wrong? I don't know. Using `(len, ptr)` will certainly cause other problems, like how long is the length field? Is it fixed or is it flexible?

Since the spirit of C is trying to be simple and close to the machine, maybe NULL-terminated string is the way to go.

If we want to implement the `(len, ptr)` pattern, it's trivially easy. Check [sds in Redis](https://github.com/antirez/redis/blob/unstable/src/sds.h#L51).

### Lab 2: Below C Level

#### C to assembly

`make codegen.list` and see what compiler does! Let your curiosity be your guide!

#### Makefiles

I've noticed that the loop counter variable `c` in `blink.c` is not `volatile`.

```c
for (int c = DELAY; c ! = 0; c--); // wait
```

Is that OK? We can find out.

Disassemble the code and we can see the loop is there, it hasn't been optimized out.

But if we change the gcc flag `-Og` to `-O2`, the loop is gone.

So as a conclusion: __If your busy loop doesn't work, check the optimization level and whether or not the loop counter is volatile__.

**Disquss**

**Q:** What is the purpose for each of the CFLAGS?

**A:** `arm-none-eabi-gcc -v --help` is our best friend.

**Q:** What happens if you just type make? Which commands will execute?

**A:** It will make the first target just as you have typed `make all`.

**Q:** If you modify `blink.c` and run make again, which commands will rerun? What part of each target indicates the prerequisites?

**A:** All three commands, two `arm-none-eabi-gcc` and one `arm-none-eabi-objcopy`. The right part after the colon indicates the prerequisites.

**Q:** What do the symbols `$<` and `$@` mean?

**A:** `$@` refers to the left part of the rule, before the `:`. `$<` refers to the first element in the right part of the rule, after the `:`. And by the way, `$^` refers to all elements in the right part of the rule, after the `:`.

**Check-in question**

**Q:** Using the sample Makefile in the lab writeup, trace what happens when you issue make with no command-line arguments: what does make try to build? How does it determine what actions to take?

**A:** make will try to build the first target. Every build target has a "formula". make build target by building its dependencies and executing the formula.

**Q:** Got that “Green->success, Red->fail”, but are there any other outcomes to watch for? What happens when executing a test case that itself is in error, such as the final test case of testing.c? What happens if the bug in count_bits caused it to enter an infinite loop– what then would you expect to be the observed behavior when testing?

**A:** Yes, we get neither green nor red flashing. If a test case is incorrect, we will see the red LED flashing and think that our code is wrong. If the program enters an infinite loop, we will observe neither green lit nor red flashing. The green LED is off and the red LED is on.

**Q:** Which pins must be connected to show "3" on the first digit? Which additional pins much be connected to show "3333" across all four digits? Why is it impossible to simultaneously show "3" on the first digit and "4" on the second? (The trick to doing so, as you will see in the assignment, is to quickly switch between digits to create the illusion of displaying "34")

**A:** In order to show "3" on the first digit, we have to connect pin 12 (digit 1), pin 11 (A), pin 7 (B), pin 4 (C), pin 2 (D) and pin 5 (G). To show "3333", we have to connect pin 9 (digit 2), pin 8 (digit 3) and pin 6 (digit 4). Because all segements on four digits are connected together. If segement A on digit 1 is on, then segement A on digit 2, 3, 4 are all on.

#### Testing

Using two built-in LEDs to indicate whether something is wrong.

- GPIO 47: greet act LED
- GPIO 35: red power LED

Always test, test and test the program!

#### Wire up display breadborad

Just follow the instructions and remember: small step with tests!

Since I don't have short jumpers, it looks kind of messy. But the structure is clear.

![](./assets/display-breadboard.jpeg)

### Assignment 2: Implement a Clock

#### A Clock

This [Interactive demo of segment display](http://www.uize.com/examples/seven-segment-display.html) can be very helpful when constructing bit patterns.

Note: For the input pin, we are not going to implement the pull-up by ourselves. __It turns out that Raspberry Pi has built-in support for pull-up and pull-down__. Check the manual on page 100.

```c
static volatile u32 *gp_pud = (u32 *)0x20200094;
static volatile u32 *gp_pud_clk = (u32 *)0x20200098;
static void gpio_set_pullup(uint pin)
{
  uint bank = pin / 32;
  uint shift = pin % 32;

  *gp_pud = 2;

  for (volatile int i = 0; i < 150; i++) ;

  *(gp_pud_clk + bank) = 1 << shift;

  for (volatile int i = 0; i < 150; i++) ;

  *gp_pud = 0;
  *(gp_pud_clk + bank) = 0;
}
```

Much more convenient now! 😄

Here comes our cute home-made clock.

![](./assets/clock.gif)

#### Extension: Set Time

First, we need to design the _interface_. It's actually very straight forward.

Out clock has three modes.

- Running mode: it just ticks every second
- Setting mode: configure each digit in this mode
- Halt mode: it stops and displays fixed numbers

Red button for `start/stop` and blue button for switching from setting mode.

Let's see the actions and transitions from each mode:

- First, the clock shows the initial '----'
- Press red button, it starts running
  - Press red button again, it stops
  - Press blue button, it enters setting mode, see detail below
- Press blue button, it enters setting mode
  - Press blue button to move the "focus". Exiting to halt mode if the focus reaches to the end.
  - Press red button to increase the number of the current digit

What a cool clock!

When we try to capture the button press event, we will soon found a problem.

> When reading button presses on the Pi, you will quickly realize that pressing the button once may cause the value on the GPIO pin to change multiple times. This is due to physical characteristics of the button mechanism which cause the button circuit to open and close multiple times during a press.

Let's see the demo:

![](./assets/button-unstable.gif)

We can clearly see that on button press triggers multiple increments.

This can be solved by __debouncing__.

> To address this issue, implement debouncing by checking whether the value change on the GPIO pin corresponds to an actual button press or one of these spurious events. This can be done by checking that the GPIO pin reads the button press value for a long enough time (that is, these spurious events will change the GPIO value quickly, meaning if you check the value of the pin as pressed, then wait a bit longer and see it as unpressed, that means it was a spurious event).

The main idea is that we check the input level, wait a little bit time (we have to experiment to find out the time we need), then we check again. If both checks give us a low level, the button is pressed.

Our little button works now!

![](./assets/button-debounced.gif)

This is our final clock, check the full code [week3/assign2/apps/clocl.c](./week3/assign2/apps/clock.c).

![](./assets/clock-extended.gif)

NOTE: If you encounter this error: _undefined reference to `__aeabi_idivmod`_, that means you have used some division operations and you need to link to `libgcc.a`.

## Week 4: Communication and the Serial Protocol

`serial-code/serial` demonstrate how to implement a seril communication by ourselves. Basically, write bit, wait some time, write bit again.

### Lab 3: Debugging and Testing

#### Debugging with gdb

We can use `arm-none-eabi-gdb` to debut the elf file.

```bash
$ arm-none-eabi-gdb simple.elf
(gdb) target sim
(gdb) load
```

**Check-in question**

**Q:** Explain how the `lr` register is used as part of making a function call. Which instruction writes to the `lr` register? Which instruction reads from it? What commands could you use in gdb to observe the changes to the `lr` register during execution of a function call?

**A:** When we call a function, the address of the next instruction after the function is written to `lr`. After calling, set `pc` to the value `lr` has recorded to get us back. `bl` (branch and link) writes to the `lr` register. `bx` (branch and exchange) reads from the `lr` register. Use `display/x $lr` to show the content of the `lr` register after every step.

**Q:** Why is it necessary to plug in both TX and RX for loopback mode to work?

**A:** We need TX to send data to Pi and RX to receive data from Pi.

**Q:** On a hosted system, executing an incorrect call to strlen (e.g. argument is an invalid address or unterminated string) can result in a runtime error (crash/halt). But when running bare metal on the Pi, every call to strlen (whether well-formed or not) will complete “successfully” and return a value. Explain the difference in behavior. What is the return value for an erroneous call?

**A:** On a hosted system, if we pass an invalid address to strlen, strlen function may access unaccessable memory and cause the famous segement fault. That is because the OS is "watching" our program, make sure we "behave" and we don't read from/write to somewhere we shouldn't. But on the Pi, we are on our own. If we pass an invalid address to strlen, strlen will read memory from that address until it find a null byte. I don't know what would happen in Pi if we access beyond the phsical available memory (512MB). For an erroneous call, the return value varies depends on the contents in the memory and is meanless.

#### Serial communication

Use `screen` to communicate to our Pi over a tty device.

```bash
$ screen /dev/your-tty-device-here 115200
```

To close the connection, type `Control-a` followed by `k`.

Having been debugging our program with blinking LEDs since the very beginning, now I just realized how powerful `printf` is.

#### C-strings

`strcpy` is easy and **dangerous**. The good old C always trusts programmers. But I don't trust myself 😉.

In real projects, we should never use `strcpy`. Use `strncpy` instead.

#### Gdb and testing

Fun debugging!

### Assignment 3: Implement a String Formatting Library

Since I don't have the starter files, I have to build them by myself. Actually, It's kind of fun.

`timer.o`, `gpio.o` and `uart.o` are extracted from the official `libpi.a`.

```bash
$ arm-none-eabi-ar -xv libpi.a timer.o gpio.o uart.o
```

#### Strings module

The function prototypes in `strings.h` are very clear and helpful. So this is a fairly easy task.

We can utilize _TDD_ here. Write the tests first, then the implementation.

Check the code [strings.c](./week4/assign3/strings.c).

#### Number to string conversion helper functions

Test, test and test! I thought this was an easy task, but it turned out I made tons of bugs. Luckily, I wrote plenty test cases and they just saved me.

One thing to note: **Be careful with unsigned int**. They will overflow and cause subtle bugs if you are careless.

```c
// limit is an unsigned inteer
for(int i = 0; i < limit - 1; i++) {
  ...
}
```

You are out of luck if `limit` is 0 😢.

#### Implement snprintf, vsnprintf and printf

`vsnprintf` does all the heavy lifting. Stick to the awesome *strategies for success* and implement format conversions one by one with complete test cases.

The code is not hard at all, you just need to be very careful about every operations you do.

Check the code [printf.c](./week4/assign3/printf.c).

NOTE: The function's behavior for an invalid format conversion is undefined. You can define your own behavior. In my code, all the invalid format conversions will be copy as is. e.g. `printf("%a") == "%a"`.

#### Extension: Disassembler

This task is kind of challenging and need to take a while to finish.

The most challenging part is not the code writing but the documentation digging.

[ARM Architecture Reference Manual](https://www.scss.tcd.ie/~waldroj/3d1/arm_arm.pdf) is the best resource to nail this task down.

The following one liner is very helpful to get disassembling output from a single instruction.

```bash
$ perl -e 'print pack "H*", "e3a0d302"' > a.out && arm-none-eabi-objdump -D -b binary -marm -EB
a.out:     file format binary


Disassembly of section .data:

00000000 <.data>:
   0:   e3a0d302        mov     sp, #134217728  ; 0x8000000
```

To make the debug less painful which requires constantly rebooting the Pi and loading the code to see the output, we can use macro to conditionally compile the `printf` function and see the output in GDB.

```c
#if GDB_DEBUG
char __stdout[1024*1024] = {};
#endif

int printf(const char *format, ...) {
  char buf[1024];
  va_list ap;
  va_start(ap, format);
  int result = vsnprintf(buf, sizeof(buf), format, ap);

#if GDB_DEBUG
  strlcat(__stdout, buf, sizeof(__stdout));
#else
  uart_putstring(buf);
#endif

  return result;
}
```

Run the code in GDB and see the result with following command:

```
(gdb) printf "%s", __stdout
```

Check [Makefile](./week4/assign3/Makefile) to see the details.

**Remember to write the test! Every time you want to implement some function, write the test first!**

Implementing a complete disassembler is a huge task, I am not gonna do it. The final disassembler manages to successfully disassemble the first 100 instructions of itself. Cool enough 😎 Check the code [disassemble_self.c](./week4/assign3/apps/disassemble_self.c).

### Modules, Libraries, and Linking

**gcc**

`gcc` is actually a front-end of a bunch of tools and it's very powerful!

- `gcc -E`: preprocess, expand all macros and transform `.c` -> `.i`
- `gcc -S`: compile, transform `.c` -> `.s`
- `gcc -c`: assemble, transform `.s` -> `.o`

You can use `gcc -save-temps` to get all the intermediate files.

**Some useful tools**

- `arm-none-eabi-nm`: list symbols in the object file
- `arm-none-eabi-size`: list size of sections in the object file
- `arm-none-eabi-ar`: manage archive files

**Symbols in the object file**

- T/t - text
- D/d - read-write data
- R/r - read-only data
- B/b - bss (Block Started by Symbol)
- C - common
- lower-case letter means **static/local**

Pay attention to the `COMMON` symbol. They are very confusing at the first glance. Why do we need them? What's the difference from the BSS?

Simply put, COMMON only appears before the linking stage. Check [.bss vs COMMON: what goes where?](https://stackoverflow.com/questions/16835716/bss-vs-common-what-goes-where).

## Week 5: Memory Management

just malloc is easy 😎
malloc with free is hard 🤔
Efficient malloc with free ….Yikes! 😥

### Lab 4: Linked and Loaded

#### Linking

What type and size of symbol would correspond to an array definition such as `const int arr[5]`?

Because `arr` is uninitialized, it is a common symbol and has size of 5\*4 = 20. We can use `arm-none-eabi -S` to inspect the symbol size.

Resolution and relocation:

> The compiler processes only a single module (file) at a time and thus it can only resolve references to symbols that appear within the module currently being compiled. The linker runs in a subsequent pass to perform tasks that require joining across modules. The process of filling in the missing placeholder addresses with the final symbol locations is known as resolution.
>
> The linker is given a list of object files to process and it will combine the files together and arrange symbols into their final locations (relocation) and resolve cross-module references (resolution).

Note the difference between object files (.o) and archive files (.a).

> The linker treats objects files (.o) and libraries (.a) a little bit differently. When linking object files, all the files are combined. When linking libraries, only files containing definitions of undefined symbols are added to the executable. This makes it possible to make libraries with lots of useful modules, and only link the ones that you actually use in the final executable.

#### Memory Map

We can use a `linker script` to tell the linker how to lay out the sections in the final executable file.

Here is [documentation on linker scripts](https://sourceware.org/binutils/docs-2.21/ld/Scripts.html).

#### Bootloader

Our laptop and the bootloader communicate over the serial line via the Pi's UART. They use a simple file transfer protocal called XMODEM.

**Xmodem file transfer protocol**

**Q:** Why can’t the bootloader code also be placed at 0x8000?

**A:** Because bootloader will place data it received at 0x8000. If bootloader code is also at 0x8000, then it will destroy its own code.


**Q:** Where does the bootloader copy the received program and how does it begin executing it? Which instruction is executed first?

**A:** The bootloader will copy the received program to 0x8000 and use `BRANCHTO` function to begin executing it. `bx r0` instruction is executed first to jump to the received program.

**Q:** How does the bootloader use the green ACT led to signal to the user?

**A:** Flashing LED means bootloader is waiting for the data. Lit LED means it's receiving the data. Off LED means data has been received and it's being executed.

**Q:** In which circumstances does the bootloader respond with a NAK? When does the bootloader give up altogether on a transmission?

**A:** Bootloader responds with a NAK under these circumstances:

- It doesn't receive SOH as the first byte.
- It received SOH but the next byte is not the corresponding sequence number.
- It received SOH and the corresponding sequence number, but the next byte is not the complement of the sequence number.
- It received the whole 128 bytes packet data, but the checksum byte is wrong.

The bootloader will give up transmission when it didn't receive any data in half sec or it has encountered 5 errors.

**Q:** How/why does the bootloader use the timer peripheral?

**A:** Simply put, it need the timer to wait some time:

- When flash active LED, toggle it on, wait some time and toggle it off.
- When read data from UART, if there is no data, wait some time and check again.
- When branch to newly received program, wait some time and delay its execution so we can see potential output in our PC console.

**Q:** How will the bootloader respond if you unplug the USB-serial in the middle of transmission?

**A:** It will time out and start waiting for the new transmission.

#### Stack

**Function prolog:**

**Q:** Which four registers are pushed to the stack to set up the APCS frame?

**A:** `r11(fp)`, `r12`, `lr(r14)`, `pc(r15)`

**Q:** Three of the four registers in the APCS frame are caller-owned registers whose values are preserved in the prolog and later restored in the epilog. Which three are they? Which register is the fourth? Why is that register handled differently? What is even the purpose of pushing that register as part of the APCS frame?

**A:** Three registers are `r11`, `r12` and `lr`.

`pc` is the fourth register. We can't just restore pc to what it held before, because that won't make us out of the current function. We need to move value of lr to pc.

??? I don't know why we have to store the value of `pc`. Maybe because `pc - 12` is also the entry point of the current function, so we can use it to do someting?

**Q:** What instruction in the prolog anchors fp to point to the current frame?

**A:** `sub r11, r12, #4`

**Q:** To what location in the stack does the fp point?

**A:** The first word (from high address to low address) of the current stack frame (pc).

**Q:** The first instruction of the prolog copies sp to r12 and then uses r12 in the push that follows. This dance may seems roundabout, but is unavoidable. Do you have an idea why?

**A:** We can't use a value in a instruction which is simultaneously changing the value.

**Function epilog:**

**Q:** To what location in the stack does the sp point during the body of the function?

**A:** sp always point to the last word in current stack frame.

**Q:** he first instruction of the epilog changes the sp. To what location does it pointer after executing that instruction?

**A:** Point to the fourth word in current stack frame.

**Q:** The ldm instruction (“load multiple”) reads a sequence of words starting at a base address in memory and stores the words into the named registers. pop is a specific variant of ldm that additionally adjusts the base address as a side effect (i.e. changes stack pointer to “remove” those words) . The ldm instruction used in the epilog of abs copies three words in memory starting at the stack pointer into the registers r11, sp, and lr. This effectively restores the registers to the value they had at time of function entry. The instruction does not similarly restore the value of the pc register. Why not?

**A:** Because we dont' want to do that. The pc value in the stack points to the first instruction of our function (excepts for prolog). If we restore the pc, we will jump back to our function. That's not what we want.

**Q:** Which registers/memory values are updated by the bx instruction?

**A:** pc.

**Make Array:**

**Q:** After the instructions for the standard prolog, what additional instruction makes space for the array local variable?

**A:** `sub sp, sp #24`

**Q:** How are the contents for the array initialized (or not)?

**A:** They are not initialized at all (except for the first one which we initialize it with `array[0] = 1;`).

**Q:** In the body of the function, the array elements stored on the stack are accessed fp-relative. What is the relative offset from the fp to the base of the array? How can you determine that from reading the assembly instructions?

**A:** The relative offset is 36. We can tell it from this instruction: `str r3, [r11, #-36]`. It's used for initializing the first element of the array.

**Q:** The prolog has an additional instruction to allocate space for the array, but the epilog does not seem to have a corresponding instruction to deallocate the space. How then is the stack pointer adjusted to remove any local variables on function exit?

**A:** Just restore its value to what it stored previously. This is the benefit of using a stack: we don't have to deallocate, it's automatic.

#### Heap

**Q:** What would be the consequence if strndup mistakenly returns a pointer to memory contained within its to-be-deallocated stack frame?

**A:** We will read wrong data afterwards.

#### Check-in Question

**Q:** Compare the symbol list from nm linking.o to nm linking.elf. How do the symbol addresses from an object file differ from the addresses in the linked executable? How does the instruction bl \_cstart change after linking?

**A:** For non-local symbols, their addresses will be zero in object file. Before linking, `bl _cstart` actually is `bl 0`. After linking, the placeholder zero will be filled with actual address.

**Q:** On a sheet of paper, with address 0x0 at the bottom of the page and 0x8000000 at the top, sketch a diagram of the program memory for code/linking. Include the contents of the stack at the point where execution has just entered the main function. Use the memory diagram for simple as a guide.

**A:**

```
+------------+ 0x8000000
|  _cstart   |
|            |
+------------+
|    main    |
|            |
+-----+------+
|     |      |
|     |      |
|     |      |
|     v      |
|   stack    |
|            |
|            |
|            |
|            |
|            |
|            |
|    heap    |
|     ^      |
|     |      |
|     |      |
|     |      |
+-----+------+
|    bss     |
|            |
+------------+
|   rodata   |
|            |
+------------+
|    data    |
|            |
+------------+
|    text    |
|            |
+------------+ 0x8000
|            |
|            |
+------------+ 0x0
```

**Q:** How does the saved pc in a function’s stack frame relate to the address of the first instruction for the function? If you take the saved pc in the stack frame for `main` and subtract it from the saved lr in the stack frame for `diff`, what do you get? Express your answer as a number, then explain conceptually what that computation represents. The memory diagram for simple is a helpful reference.

**A:** Saved pc = first instruction + 12.

Use abs stack frame for example, `main` = 0x0000801c and `diff` = 0xbc. It represents the offset between caller and callee. But I don't see this any useful ???

**Q:** How should the number of free calls relate to the number of malloc calls in a correctly-written program that uses heap allocation?

**A:** They should be **exactly** same.

### Assignment 4: Backtrace and Malloc

#### Prepare starter files

`gpio.o`, `timer.o`, `uart.o` and `strings.o` are copied from official `libpi.a`.

```bash
$ arm-none-eabi-ar -xv libpi.a gpio.o timer.o uart.o strings.o
```

`backtrace.h` and `malloc.h` are copied from `_resource/cs107e/include`.

`printf.h`, `printf_internal.h`, `printf.c`, `cstart.c` and `start.s` are copied from assignment 3.

`Makefile` is written from scratch.

Now we are ready to tackle the challenges 🥊.

#### Backtrace Module

Have you ever wondered how stack tracking is implemented? No matter what programming language we are using, every time we messed up, we got a helpful stack tracking log.

Now let's implement this essential feature for ourself!

This task is not hard but time-consuming. We need a lot of patience to debug what things go wrong.

[stack_abs.html](./docs/labs/lab4/images/stack_abs.html) is very helpful.

Check the final code [backtrace.c](./week5/assign4/backtrace.c).

#### Malloc Module

Actually I think this task is very straight forward. Do some drawing with a pen and paper. We can figure out what to do for each function pretty easily.

NOTE: `sbrk` should check the up bound. We surely don't want our heap corrupt our stack.

We can get current stack pointer using inline asm.

```c
char *cur_sp = NULL;
__asm__("mov %0, sp" : "=r"(cur_sp));
```

Check the final code [malloc.c](./week5/assign4/malloc.c) and [test_backtrace_malloc.c](./week5/assign4/tests/test_backtrace_malloc.c). Always write the test!

#### Extension: Mini-Valgrind

NOTE: Take care to keep the payload aligned on the 8-byte boundary, which means the pointer returned by `malloc` should always be divisible by 8.

Add four bytes to our header as the leading red zone and four bytes to payload as the trailing red zone.

Whenever we are freeing a block, check the red zones. If they have incorrect content, then we know something bad is happening!

We can call `memory_report` in our `_cstart` function so that every program now has a neat memory report.

### C Mastery

Struct is a convenient and readable way to allocate memory and name offsets from a pointer.

NOTE: In C, struct has a "padding" process. Check this stack overflow thread [Structure padding and packing](https://stackoverflow.com/questions/4306186/structure-padding-and-packing) for details.

**What you need to write good software?**

- Productive development process
- Effective testing
- Proficient debugging strategy
- Priority on good design/readability/maintainability

**What is different about systems software?**

- Terse and unforgiving, details matter
- All depend on it, bugs have consequences
- Not enough to know what code does, but also how/why

**The value of code reading:**

- Is it clear what the code intends to do?
- Are you confident of the authors's understanding?
- Would you want to maintain this code?

> There are two ways of constructing a software design. One way is to make it so simple that there are obviously no deficiencies. And the other way is to make it so complicated that there are no obvious deficiencies. - C.A.R. Hoare

**Recommended development process (I can't agree more):**

- Write the high-quality version first (and only!)
- Decompose problems, not programs
- Implement from bottom up, each step should be testable
- Unifying common code means less code to write, test,
debug, and maintain!
- Don’t depend on comments to make up for lack of
readability in the code itself
- One-step build

**Engineering best practices:**

- Test, test, test, and test some more
- Start from a known working state, take small steps
- Make things visible (printf, logic analyzer, gdb)
- Methodical, systematic. Form hypotheses and perform experiments to confirm.
- Fast prototyping, embrace automation, one-click build, source control
- Don’t let bugs get you down, natural part of the work, relish the challenge you will learn something new!
- Wellness important! ergonomics, healthy sleep/fuel, maintain perspective

## Week 6: Keyboards and the PS/2 Protocol

PS/2 is the original serial protocol for keyboards and mouse (replaced by USB now).

[The PS/2 Mouse/Keyboard Protocol](https://web.archive.org/web/20180302005138/http://computer-engineering.org/ps2protocol/).

It has a 6-pin mini-DIN connector.

```
       ++
   +-+ || +-+
   |5| || |6|
   +-+ ++ +-+
+-+          +-+
|3|          |4|
+-+          +-+
   +-+    +-+
   |1|    |2|
   +-+    +-+
```

- Pin 1: Keyboard Data
- ~Pin 2: Unused~
- Pin 3: Ground
- Pin 4: +5 Volts
- Pin 5: Clock
- ~PIn 6: Unused~

Keyboard scancodes:

![](./assets/scancode.png)

Make (press) and Break (release) codes:

| Key | Action | Scan Code |
| :-: | :-: | :-: |
| A | Make (down) | `0x1C` |
| A | Break (up) | `0xF0 0x1C` |
| Shift L | Make (down) | `0x12` |
| Shift L | Break (up) | `0xF0 0x12` |
| Alt R | Make (down) | `0xE0 0x69` |
| Alt R | Break (up) | `0xE0 0xF0 0x69` |

We can see that **when we release some key, keyboard sends `0xF0` plus the scancode of the key**.

PS/2 is a **synchronous** protocol, which means that the keyboard sends clock.

- Data changes when clock lines is high
- Host reads data when clock is low

Payload: 1 start bit + 8 data bits (lsb-first) + 1 parity bit + 1 stop bit = 11 total bits.

Parity = XOR of all of the data bits

MIDI: Musical Instrument Digital Interface

### Lab 5: Keyboard Surfin'

the USB protocol is **complicated**. It’s approximately 2,000 lines of code to interface with a USB keyboard. OMG.

If you press a key and hold it down, the keyboard enters auto-repeat or typematic mode where it repeatedly generates key press actions until you release the key.

The code of this lab is fairly easy. Check [keyboard.c](./week6/lab5/my_keyboard/keyboard.c).

#### Check-in questions

**Q:** Back in lab1, we estimated how many instructions the Pi was executing (\~20 million/second). Today you measured the timing of PS/2 clock cycle. Calculate how many instructions the Pi can execute in that time. Now consider a call to printf. Make a ballpark estimate of how many instructions must execute to process and output a single char, now multiply by length of the format string for rough total count. If your keyboard driver makes a call to printf in between reading bits that overruns your budget, what will be the consequence?

**A:** We will miss scancode.

**Q:** What sequence of codes is sent when typing capital `A`?

**A:** `0x59(right shift) 0x1C` or `0x12(left shift) 0x1c`.

**Q:** In a PS/2 scancode, does the least significant or most significant data bit arrive first?

**A:** Least significant bit arrives first.

### Assignment 5: Keyboard and Simple Shell

#### Prepare starter files

Copy `cstart.c`, `memmap`, `start.s` from assignment 4.

Copy `keyboard.c` from lab5.

Create `shell.c` with stub functions based on `shell.h`.

Create `app/uart_shell.c` with following content:

```c
#include <uart.h>
#include <keyboard.h>
#include <printf.h>
#include <shell.h>

void
main(void) {
  uart_init();
  keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
  shell_init(printf);

  shell_run();
}
```

Create `tests/test_keyboard.c` with an empty main function.

Create `Makefile` to compile our app and test.

#### Keyboard driver

The keyboard module is well designed. We have 4 functions to handle 4 different level tasks:

- `keyboard_read_scancode`: the lowest level function to read raw scancode.
- `keyboard_read_sequence`: combine 1, 2 or 3 scancodes to a sequence. A sequence consists of what the action is (press or release) and what the scancode of the key is. In curent design, it doesn't track whether the key is normal or extended. That is, we can't tell the difference of left alt `0x11` and right alt (`0xE0 0x11`).
- `keyboard_read_event`: compared to a sequence, a key event contains modifiers status.
- `keyboard_read_next`: maybe `keyboard_read_char` is a better name? This function returns an ASCII character of current key press.

At the end, our keyboard driver need to handle this two siturations.

- Typing an ordinary key produces an ASCII character. The ordinary keys are:
  - All letters, digits, and punctuation keys
  - Whitespace keys (Space, Tab, Return)
- Typing a special key produces its designated value. These values are greater than 0x90 to distinguish from ASCII values. The special keys are:
  - Escape
  - Function keys F1-F12
  - Backspace key (sometimes marked with ← or ⌫)

Our keyboard driver does not produce modified characters based on state of Alt or Control, only for Shift and Caps Lock.

If Shift and Caps Lock applied together, Shift “wins”. Caps Lock and Shift together do not invert letters to lowercase. (Why design it this way???)

#### Simple shell

Thanks for all the previous efforts we made. Now Implementing a shell is a very eask task.

Check the code [shell.c](./week6/assign5/shell.c).

#### Extension: editing and history

**Command-line editing:**

- Left and right arrow keys
- `ctrl-a` and `ctrl-e` to move the cursor to the first and last
- `ctr-u` to delete the entire line

To support `ctrl-*` keys, we need to modify our `keyboard.c` so that it can produce character representing these keys. Currently, it can't recognize any ctrl keys.

We can use `\b` to move the cursor left, but for moving the cursor right, I use `Esc[C` based on the [ASCII Escape Sequences](http://ascii-table.com/ansi-escape-sequences.php).

**Command history:**

- Change the prompt to include the command number of the current line.
- Implement `history` command.
- Support key up and key down to browse histories.

Implementing history browsing with key up and key down is a little bit tricky, we have to consider all kinds of edge cases.

Consider this case: we have three history items and we press key up to browse the latest one. Then we type something, what should happen when we press key up again?

In bash or zsh or other mature shells, they filter histories using the current content. It's the reasonable thing to do but I don't plan to implement this feature in this assignment. We just ring the bell and do nothing.

Check the final code [shell.c](./week6/assign5/shell.c).

### Graphics and the framebuffer

CPU and GPU use [mailbox](https://github.com/raspberrypi/firmware/wiki/Mailboxes) to communicate.

CPU sends `fb_config_t` structure to GPU and gets framebuffer address back.

| Field | CPU | GPU | Description |
| :-: | :-: | :-: | :-: |
| width | write | read | Width of physical screen |
| height | write | read | Height of physical screen |
| virtual width | write | read | Width of framebuffer |
| virtual height | write | read | Height of framebuffer |
| pitch | read | **write** | Number of bytes in a scanline |
| depth | write | read | Bits per pixel |
| x_offset | write | read | X offset of screen in framebuffer |
| y_offset | write | read | Y ofset of screen in framebuffer |
| pointer | read | **write** | Pointer to framebuffer |
| size | read | **write** | Size of framebuffer in bytes |

NOTE: **The byte sequence of pixel is BGRA in framebuffer**.

```c
uint32_t fb[WIDTH * HEIGHT];
fb[0] = 0xAARRGGBB; // y=0, x=0
fb[1] = 0xAAGGRRBB; // y=0, x=1
```

Framebuffer overview:

- GPU continuously refreshes the display by sending the pixels in the framebuffer out the HDMI port
- The size of the image sent to the monitor is called the physical size
- The size of the framebuffer image in memory is called the virtual size
- The CPU and GPU share the memory, and hence the framebuffer
- The CPU and GPU exchange messages using a mailbox

NOTE: The default resolution in Pi GPU is not 1920x1080. If you have a 1920x1080 screen, in order to change the GPU resolution, create a file named `config.txt` with following content:

```text
hdmi_group=2
hdmi_mode=82
```

And put the file inside the root of sd card.

Now we have graphics! Check the code [gradient.c](./week6/gradient/gradient.c).

![](./assets/fb-gradient.jpeg)

## Week 7

### Lab 6: Drawing into the Framebuffer

#### Study fb and mailbox code

**Q:** What is the difference between physical size and virtual size? What is the difference between width and pitch?

**A:** The size of the image sent to the monitor is called the physical size and the size of the framebuffer image in memory is called the virtual size.

**Q:** What typecast could you apply to the framebuffer address to access the pixel data as a one-dimensional array of 8-bit bytes? As a two-dimensional array of 32-bit pixels? (Be sure to take into account the difference between width and pitch!)

**A:**

```c
// as one-dimensional array of 8-bit bytes
unsigned char *arr = fb.framebuffer;

// as two-dimensional array of 32-bit pixels
int row_len = fb.pitch / (fb.bit_depth / 8);
unsigned int (*arr)[row_len] = fb.framebuffer;
```

**Q:** Why does the code need each of the checks for whether the mailbox is EMPTY or FULL? What might go wrong if these checks weren’t there?

**A:** Because the CPU and CPU are asynchronous, they need to wait for each other by checking the mailbox status. We may read wrong data or corruput the data if there is no such check.

**Q:** Why can we add the `addr` and `channel` in `mailbox_write`? Could we also `|` them together? Which bit positions are used for the `addr` and which are used for the `channel`?

**A:** `addr` is 16-byte aligned which means the lower 4 bits are always zero and `channel` is less than 16 so they can be added together.

We can also get same result by `|` them.

Top 28 bits are used for `addr` and lower 4 bits are used for `channel`.

**Q:** Do `fb`, `mailbox`, and the `framebuffer` live in GPU or CPU memory? Which of these data structures can we choose where to allocate, and which are given to us?

**A:** `fb` apparently lives in CPU memory. `mailbox` address is `0x2000B880` and it's a peripheral register. I have printed `framebuffer` address and it's `0x4eace000`. It's way bigger than 512MB so I don't what memory it lives in???

We can only choose where to allocate `fb`.

**Q:** What happens to the loop that waits until not full/empty? What would be the observed behavior of executing the code that doesn’t use `volatile` ?

**A:** The loop will not read the value in the memory every time. It reads once and stores the value and uses that value for computing the loop condition.

It will loop forever if the first time loop condition checking is failed.

#### Multidimensional pointers

**Q:** What is the difference between the following two declarations? Think about what operations are valid on `a` versus `b`.

```c
char *a  = "Hello, world\n";
char b[] = "Hello, world\n";
```

**A:** `a` is a pointer to a char which lives in the read only memory. `b` is an array of char which lives in the stack.

We can't modify the data pointed by `a` but we can modify the data in `b`.

**Q:** What is the difference between the following two declarations?

```c
int *p[2];
int (*c)[2];
```

**A:** `p` is an array of two int pointers. `c` is a pointer pointing to an array of two int elements.

#### Fonts

**Q:** why does pixel_data have size `95 * 14 * 16 / 8`?

**A:** We have 95 characters, each character has 14 * 16 pixels, each pixel uses one bit. So we have `95 * 14 * 16` bits and that is `95 * 14 * 16 /8` bytes.

**Q:** Trace the operation of `font_get_char` for ASCII character `&` (hex 0x26)? At what location in pixel_data does it look to find the appropriate bits?

**A:** Bit index of first row: `0 * 1330 + 5*14 = 70`. Related byte indexes are 8, 9, 10. Bit value is `00001100000000`.

Bit index of second row: `1 * 1330 + 5*14 = 1400`. Related byte indexes are 175, 176. Bit value is `00001100000000`.

Bit index of third row: `2 * 1330 + 5*14 = 2730`. Related byte indexes are 341, 342. Bit value is `00110011000000`.

...

Following is the complete bit value for `&`, space means zero.

```text
    11
    11
  11  11
  11  11
  11  11
  11  11
    11
    11
  11  11  11
  11  11  11
  11    11
  11    11
    1111  11
    1111  11
```

#### Check-in

**Q:** How does your checkerboard look? Show a TA your crisp looking squares!

**A:**

![](./assets/fb-chessboard.jpeg)

**Q:** What happens if `mailbox_t mailbox` is not tagged as volatile?

**A:** The statuc checking while loop will run forever.

**Q:** Why is it valid to use `|` to combine the `addr` and `channel` in `mailbox_write`?

**A:** The lower four bits of `addr` is always zero and `channel` only occupies four bits at maximum.

**Q:** Show off your memory map diagram! Where does the stack sit, relative to the framebuffer? Where do the GPIO registers sit relative to the mailbox?

**A:** To be completed.

**Q:** What is the difference between the following two lines of code?
**Q:** Why does pixel_data have size 95 * 14 * 16 / 8?

**A:** Duplicated questions. Check my answers above.

### Assignment 6: Graphics Library and Console

#### Prepare starter files

Copy `memmap`, `keyboard.c`, `shell.c`, `start.s` and `cstart.c` from assignment 5.

Create `apps/console_shell.c` with following content:

```c
#include <gpio.h>
#include <keyboard.h>
#include <console.h>
#include <shell.h>

#define NUM_ROWS 20
#define NUM_COLS 40

void
main(void)
{
  gpio_init();
  console_init(NUM_ROWS, NUM_COLS);
  keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);

  shell_init(console_printf);
  shell_run();
}
```

Create `tests/test_gl_console.c` with an empty main function.

Create `console.c`, `fb.c` and `gl.c` with stub functions based on their header files.

Finally create `Makefile` to compile our app and test.

#### Framebuffer

NOTE:

> After each call to `mailbox_write`, follow up with a call to `mailbox_read` to acknowledge the GPU’s response. If you forget this, the mailbox queue eventually fills up and the system will hang.

> To implement `fb_swap_buffer` you change which half of the virtual framebuffer is frontmost (displayed) by changing the Y offset from 0 to the physical height (or vice versa). To make this change, set the `y_offset` in the fb struct and write to the mailbox to inform the GPU to update.

This indicates that the frame buffer address won't change once we get it.

#### Graphics primitives

NOTE:

> Take care to compute the location of a pixel’s data in the framebuffer based on **pitch**, not width, because the GPU may have made each row a little wider than requested for reasons of alignment.

These functions are fairly easy, just pay very attention to always check the array bounds.

Let's draw a mandelbrot set:

![](./assets/mandelbrot_set.jpeg)

#### Fonts and text-drawing

Make sure that we clip all text drawing to the bounds of the framebuffer.

#### Console

There are four special characters that require unique processing:

- `\b`: backspace (move cursor backwards one position)
- `\r`: carriage return (move cursor to first column in current row)
- `\n`: newline (move cursor down to first column of next row)
- `\f`: form feed (clear contents and move cursor to home position in upper left)

The console should also handle the operations for:

- Horizontal wrapping
- Vertical scrolling

For working with our shell, console need also handle following keys:

- `\a`: ring the bell
- `Esc[C`: move the cursor right

**Focus on the data structure**. Think about what your data structure is and how you are gonna implement all the features with this data structure?

I use this data structure:

```c
// A simple 2-element vector
typedef union {
  struct {
    int x;
    int y;
  };
  struct {
    int width;
    int height;
  };
} v2;

typedef struct {
  // Our console may not occupy the whole screen
  // This is the left and top padding in pixels
  v2 padding;

  // Console dimensions in characters
  // e.g. 40x20
  v2 dim;

  // Console size in pixels
  // size.width = font_width * dim.width
  // size.height = font_height * dim.height
  v2 size;

  // Cursor position in character space
  // e.g. (0, 0) means our cursor is at the row 0 and column 0
  // of the console.
  // Based on the `first_row_index`, row 0 can be any row on the screen
  v2 cursor;

  // Last character position of current line
  // Because we can move the cursor, we need to track
  // where the last character is
  // e.g. (0, 10) means we are currently at row 0 and
  // we have entered 10 characters
  v2 end;

  // It's a 2D array representing all characters on our console
  // char content[dim.height][dim.width + 1],
  // extra one character for \0
  void *content;

  // Determine which row in our `content` is the first row on the screen
  // We use this index to implement scrolling feature
  int first_row_index;
}
```

`shell` uses **characters** to control our `console`. The job of `console` is to display characters and the cursor.

Check the final code [console.c](./week7/assign6/console.c).

#### Extension: Line and triangle drawing

Line drawing is actually a very complex topic. It took me several days to get into this field.

There are many algorithmes for this topic and understanding them is not an easy task.

we have

- Bresenham's line algorithm
- Samping
- Super sampling
- SDF

If you are interested in this topic and want to see some real code I highly recommend this repo [miloyip/line](https://github.com/miloyip/line).

In the end, I use a method called *AABB SDF (Axix Aligned Bounding Box Signed Distance Field)* to draw anti-aliased lines with **arbitrary width**.

The basic idea of this method is that for each line we are going to draw, first we get the axis aligned bounding box of the line. Then for each pixel in the bounding pixel we calculate whether the pixel is in the line or not and based on that information we can decide the color of the pixel.

For drawing triangle, the critical part is to check whether a point is in the triangle. We use a simple fact that:

> If the point P is in the triagnle ABC, then the area of three sub triangles PAB, PBC, PAC must be equal to the area of ABC.

Check all the details in the [gl.c](./week7/assign6/gl.c).

Now we can use our two methods to draw a beautiful hexagram:

![](./assets/hexagram.jpeg)

### Interrupts

Interrupts cause processor to pause what it’s doing and instead
execute interrupt code, return to original code when done.

Two types of events:

- External events (peripherals, timer)
- Internal events (bad memory access, software trigger)

Interrupts are essential and powerful, but getting them
right requires using everything we've learned: Architecture, assembly, linking, memory, C, peripherals, …

NOTE: In `code/button-blocking` and `code/button-interrupt`, we can see that it didn't configure pull state for the button. That's because GPIO pins all have default pull state applied on power-on.

For GPIO pin 21, the default pull state is pull-up. So we the other side of the button should be connected to a 3.3v power pin, like pin 1.

Interrupt mechanics is somewhat analogous to function call:

- Suspend currently executing code, save state
- Jump to handler code, process interrupt
- When finished, restore state and resume

Hardware support for interrupts:

- Processor executing in a particular "mode"
  - `supervisor`, `interrupt`, `user`, `abort`, ..
  - Reset starts in `supervisor` mode (that's us!)
  - Processor switches mode in response to interrupt
- `CPSR` register tracks current mode, processor state
  - Special instructions copy val to regular register to read/write
- Banked registers
  - unique sp and lr per-mode (sometimes others, too)
- Interrupt vector
  - fixed location in memory jumped to on interrupt

ARM processor modes:

- `User`: unprivileged
- `IRQ`: interrupt
- `FIQ`: fast interrupt
- `Supervisor`: privileged, entered on reset (this is us)
- `Abort`: memory access violation
- `Undefined`: undefined instruction
- `System`: privileged mode that shares user regs

ARM interrupt table has just one instruction per interrupt type. Use that instruction to **vector** to code elsewhere.

ARM Interrupts:

| Address | Exception | Mode |
| :-: | :-: | :-: |
| 0x00000000 | Reset | Supervisor |
| 0x00000004 | Undefined Instruction | Undefined |
| 0x00000008 | Software Interrupt (SWI) | Supervisor |
| 0x0000000C | Prefetch Abort | Abort |
| 0x00000010 | Data Abort | Abort |
| 0x00000014 | ... | ... |
| 0x00000018 | **IRQ (Interrupt)** | IRQ |
| 0x0000001C | FIQ (Fast Interrupt) | IRQ |

When an external event triggers a hardware interrupt, processor responses:

- Complete current instruction
- Change processor mode, save return address (PC+8) into LR of new mode, save CPSR into SPSR (Tricky! Needs to happen "simultaneously"…)
- Further interrupts disabled until exit this mode
- Force pc address 0x18 (index 6 in vector table, IRQ)
- Software takes over

## Week 8: Interrupts, continued

Interrupt checklist:

- Initialize interrupt module
- Enable detection of desired kind of event
- Write handler function to process event
- Attach handler to interrupt source
- Globally enable interrupts

Every interrupt starts with same actions: **Executes instruction at vectors[IRQ] which jumps to `interrupt_asm`, which calls C function `interrupt_dispatch`**.

Preemption and safety is very hard, has lots of bugs.

Two simple answers

1. Use simple, safe data structures - write once, but not always possible
2. Otherwise, temporarily disable interrupts - always works, but easy to forget

### Lab 7: Raspberry Pi, Interrupted

#### Review interrupts code

**Q:** When installing the vector table, `interrupts_init` copies eight instructions plus two additional words of data. What are those additional words and why is it essential that they are copied along with the table? The existing code copies the information using an explicit loop. This could instead be a single call to memcpy. What would be the correct arguments to memcpy to do the equivalent copy task?

**A:** Those additional words store memory address of interrupt handlers, `abort_asm` and `interrupt_asm`.

They are essential because interrupt vectors need to load handler address from them.

Use `memcpy` to do the cope: `memcpy(_PRI_INTERRUPT_VECTOR_BASE, _vectors, 8 * 10)`

**Q:** How does the private helper `vector_table_is_installed` verify that initialization was successful? Find where this helper is used. What client oversight is it attempting to defend against?

**A:** By checking whether some interrupt vector has correct value.

Everytime when we want to attach a handler, it will check whether the vector table has been installed.

**Q:** What piece of state needs to change to globally enable/disable interrupts?

**A:** Bit 7 in CPSR register. Set to 0 to enable interrupt, 1 to disable interrupt.

**Q:** The supervisor stack is located at 0x8000000 and configured as one of the first instructions executed in `_start`. Where is the interrupt stack located and when it is configured? A different approach would be to configure the interrupt stack at program start along with the supervisor stack, but doing so then would require temporarily changing to interrupt mode – why is that switch needed?

**A:** `interrupt_asm` function configure interrupt stack to `0x8000`.

Because `sp` in a banked register, it refers to different underlying register in different modes.

**Q:** How is a function “attached” as a handler for an interrupt source? If multiple handlers are attached to the same source, how does the dispatch operation determine which one processes the interrupt? What is the consequence if no handler is found to process it?

**A:** When we add a handler, we simply store the handelr and its source into the `handlers` array.

If a handler returns true, this indicates interrupt has been processed.

Nothing happens if no handler is found to process an interrupt.

#### Set up a button circuit

**Q:** If you click the button multiple times in quick succession, some of the presses are missed. You get neither a printed + nor a screen redraw. Why does that happen?

**A:** Because CPU is busy drawing our screen so the button checking code doesn't have chance to run and this makes us miss some button presses.

#### Write an interrupt handler

**Q:** The variable gCount must be declared `volatile`. Why? Can the compiler tell, by looking at only this file, how control flows between main and the interrupt handler? Will the compiler generate different code if volatile than without it? Will the program behave differently? Test it both ways and find out!

**A:** If gCount is not volatile, the generated code will not evaluate the if condition every time in the loop. Actually, if the first time evaluation of the loop condition gets false, it will never evaluate it again and this will cause an infinite empty loop.

Pay attention to the assembly code at `0x82cc`.

```
00008274 <main>:
    8274: e1a0c00d  mov ip, sp
    8278: e92dd830  push  {r4, r5, fp, ip, lr, pc}
    827c: e24cb004  sub fp, ip, #4
    8280: eb0003f3  bl  9254 <gpio_init>
    8284: eb000bfd  bl  b280 <uart_init>
    8288: e3a00d0a  mov r0, #640  ; 0x280
    828c: e3a01e1e  mov r1, #480  ; 0x1e0
    8290: e3a02000  mov r2, #0
    8294: eb00002b  bl  8348 <gl_init>
    8298: eb000648  bl  9bc0 <interrupts_init>
    829c: ebffffb4  bl  8174 <configure_button>
    82a0: eb0006b9  bl  9d8c <interrupts_global_enable>
    82a4: ebffff59  bl  8010 <rb_new>
    82a8: e59f5020  ldr r5, [pc, #32] ; 82d0 <main+0x5c>
    82ac: e3a03000  mov r3, #0
    82b0: e5850008  str r0, [r5, #8]
    82b4: ea000001  b 82c0 <main+0x4c>
    82b8: ebffffbe  bl  81b8 <redraw>
    82bc: e1a03004  mov r3, r4
    82c0: e5954000  ldr r4, [r5]
    82c4: e1540003  cmp r4, r3
    82c8: 1afffffa  bne 82b8 <main+0x44>
    82cc: eafffffe  b 82cc <main+0x58>
    82d0: 0000e850  andeq lr, r0, r0, asr r8
    82d4: 70617773  rsbvc r7, r1, r3, ror r7
    82d8: 746e695f  strbtvc r6, [lr], #-2399  ; 0x95f
    82dc: 00000000  andeq r0, r0, r0
    82e0: ff00000c      ; <UNDEFINED> instruction: 0xff00000c
```

The compiler can't know that control will flow to interrupt handlers. It has no way to figure that out.

**Q:** Now, edit your handler to comment out the step that clears the event. Compile and run the program and see how this changes the behavior. What changes and why?

**A:** The interrupt will be continuously triggered.

#### Use a ring buffer queue

**Q:** Why is the significance of the return value from `rb_dequeue`? Why is it essential to pay attention to that return value?

**A:** The return value of `rb_dequeue` tells whether the queue is empty. If the return value is false then we shouldn't do anything because the queue is emtpy.

**Q:** Why might you want the handler to enqueue and return instead of doing the actual task (e.g. redraw) directly in the handler?

**A:** We want to keep the interrupt handler as quick as possible because CPU disables further interrupts in interrupt mode. If interrupt handlers take too long to process, we will miss many interrupts.

**Q:** With this change, is it now necessary for gCount to be declared volatile? Does the ring buffer need to be declared volatile? Why or why not?

**A:** It's unnecessary for gCount to be declared volatile.

Based on my experient, I found that the ring buffer needn't to be declared volatile either.

#### Need for speed

> Does changing the order the pixels are accessed make a difference, i.e. instead of looping row by column, what if you loop column by row?

Actually it does. I found a very strange result: Looping column by row is slightly faster than looping row by column.

```c
// looping row by column
// 188602 ticks
for(int y = 0; y < height; y++) {
  for(int x = 0; x < width; x++) {
    arr[y][x] = color;
  }
}

// looping column by row
// 186712 ticks
for(int x = 0; x < width; x++) {
  for(int y = 0; y < height; y++) {
    arr[y][x] = color;
  }
}
```

I don't know why????

Looping column by row is definitely more unfriendly to cache. How come it runs faster?

Here are results of each modifiction:

- initial version: 1224996
- move unnecessary function calls outside of the loop: 1025915
- fill buffer directly instead of calling `gl_draw_pixel`: 188602
- loop column by row: 186712
- loop over pixels as a 1-d aray: 166351
- use a pointer `*p++ = ...` instead of `arr[index] = ...`: 148687
- `-Og`: 40875
- `-Ofast`: 38932
- `-O2`: 38928
- use a decrementing loop: 25091

> How big of an improvement were you able to make overall? Where did you get the biggest bank for your buck?

From `1224996` to `25091`, that's 48.82x faster.

Optimizing out `gl_draw_pixel` gives the biggest gain.

> It is possible to gain more than a 1000x speedup over the redraw0 function!

I don't know how to do that???? In my opinion, current assembly code is as concise as possible.

Maybe some bulk storage operation????

```
...
2c0: subs  r4, r4, #1
2c4: str r5, [r0], #4
2c8: bne 2c0 <redraw6+0x24>
...
```

### Assignment 7: System Monitor with Interrupts

#### Prepare Starter Files

Copy `start.s`, `cstart.c` and `memmap` from previous lab.

Copy `timer.c`, `gpio.c`, `strings.c`, `printf.c`, `printf_internal.h`, `malloc.c`, `backtrace.c`, `keyboard.c`, `shell.c`, `fb.c`, `gl.c`, and `console.c` from previous labs.

`tests/test_keyboard_interrupts.c` and `Makefile` are written from scratch.

#### Basic Part

Based on what we have learned, implementing the interrupt-driven keyboard driver is fairly easy.

- Init interrupts
- Enable gpio event detection
- Attach interrupt handler
- Use ring buffer to communicate between normal code and interrupt code
- Global enable interrupts

#### Need for speed?

Previously I implemented the `console` module with double buffering [console_doublebuffer.c](./week8/assign7/console_doublebuffer.c). Whenever the console receives a character, it will update the inner state, and then redraw the whole screen based on the inner state.

Although the model is very straight forward and easy understanding, we can see the deficiency: it needs to redraw the whole screen for every character.

So in this section, I reimplemented it using single buffer. The console is like a state machine, it does things based on received characters.

- normal character: draw character, move the cursor, draw the cursor
- new line `\n`: move curosr to the next line, handle scrolling
- backspace `\b`: move cursor to left
- carriage return `\r`: move cursor to the first column of the row
- formfeed `\f`: clear the whole screen and draw cursor at (0, 0)
- ...

And we can define as many special characters as we want, as long as we write code in console to handle them.

This refactoring gives a big performance boost, now the console feels very sleek.

There are some other methods we used to improve performance:

- Cache `font_get_char` for reuse.
- Adjust `y_offset` of frame buffer to implement scroll down

Check the final code [console.c](./week8/assign7/console.c).

#### Extension: Profiler

> One extension is to add profiling support to your shell. A *profiler* is a developer tool that tracks where in the code a program is spending its execution time.

There is a simple and clever way to do this using a **sampling strategy**.

- Create a big array `arr`, each item corresponds to an instruction inside the text section (we need to mark the end of the text section in our `memmap` file).
- Enable the ARM timer to interrupt with a small interval, 1ms should suffice.
- Inside the interrupt handler, the first parameter `pc` is the address of the next instruction of the interrupted one.
- Increment `arr[(pc - 4 - 0x8000)/4]` (`0x8000` is the start address of text section).
- Every time we are about to display results, we create a temporary record array. Each record has the address and the value. Sort records with quick sort and then display the top 10.

Now we have a profiler! We can clearly see that most of the time is spent reading the keyboard.

![](./assets/profiler.jpeg)

## Raspberry Pi Tips

NOTE: These are all for *raspberry pi A+ v1.1*.

- On Raspberry Pi, all GPIO banks are supplied from 3.3V. **Connection of a GPIO to a voltage higher than 3.3V will likely destroy the GPIO block within the SoC**.
- All GPIO pins revert to general-purpose inputs on power-on reset. The default pull states are also applied.

## ARM Tips

### Disassemble object file

`arm-none-eabi-objdump -D input.o`.

### Disassemble binary file

`arm-none-eabi-objdump -b binary -D -marm input.bin`.

### Disassemble one single instruction

e.g. `0xe3a0d302`

```bash
$ perl -e 'print pack "H*", "e3a0d302"' > a.out && arm-none-eabi-objdump -D -b binary -marm -EB
a.out:     file format binary


Disassembly of section .data:

00000000 <.data>:
   0:   e3a0d302        mov     sp, #134217728  ; 0x8000000
```

## ???

- Why APCS frame stores `pc` register? What is it used for?
- When interruption happens on `bl` instruction, what value does CPU put to `lr` register?

## GCC Tips

### `-mpoke-function-name`

Write the name of each function into the text section, directly preceding the function prologue. The generated code is similar to this:

```
t0
  .ascii "arm_poke_function_name", 0
  .align
t1
  .word 0xff000000 + (t1 - t0)
arm_poke_function_name
  mov     r12, sp
  push    {fp, r12, lr, pc}
  sub     fp, r12, #4
```

When performing a stack backtrace, code can inspect the word value at the location immediately preceding the first instruction. If that value has the most significant 8 bits set, then we know that there is a function name embedded preceding value and the name has length (`value & ~0xff000000`).

NOTE: `-mpoke-function-name` implies enabling function frames.

### Inline Assembly

https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html
