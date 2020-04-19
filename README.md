<div align="center">
  <h1>
    Computer Systems from the Ground Up
  </h1>
</div>

My study note of the awesome course [CS107E Winter 2020](http://cs107e.github.io/).

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
    - [Backtrace](#backtrace)
    - [Backtrace Module](#backtrace-module)
- [ARM Tips](#arm-tips)
- [GCC](#gcc)
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

You need to know that this manual __has a lot of errors__, must read it with [BCM 2835 Datasheet Errata](https://elinux.org/BCM2835_datasheet_errata#p96).

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

NOTE: If you encounter this error: _undefined reference to `__aeabi_idivmod`_, that means you have used some division operations and need to link to `libgcc.a`.

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

**Q:** Explain how the lr register is used as part of making a function call. Which instruction writes to the lr register? Which instruction reads from it? What commands could you use in gdb to observe the changes to the lr register during execution of a function call?

**A:** When we call a function, the address of the next instruction after the function is written to lr. After calling, set pc to the value lr has recorded to get us back. `bx` branch and exchange writes to the lr register. `bl` branch and link reads from the lr register. Use `display/x $lr` to show the content the lr register after every step.

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

??? I don't know why we have to store value of pc. Maybe because its value is also the entry point of the current function, so we can use it to do someting?

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

`backtrace.h` and `malloc.h` are copied from official include directory.

`printf.o`, `cstart.c` and `start.s` are copied from assignment 3.

`Makefile` is handwritten from scratch.

Now we are ready to tackle the challenges 🥊.

#### Backtrace

This task is not hard but time-consuming. You need a lot of patience to debug what went wrong.

`stack_abs.html` is very helpful.

#### Backtrace Module

## ARM Tips

- Disassemble object file: `arm-none-eabi-objdump -D input.o`.
- Disassemble binary file: `arm-none-eabi-objdump -b binary -D -marm input.bin`.

## GCC

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
