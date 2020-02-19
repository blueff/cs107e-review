# CS107E

http://cs107e.github.io/resources/

课程主页：http://cs107e.github.io/schedule/

CS107E: Raspberry Pi 1 Model A+, BCM 2835
https://www.raspberrypi.org/products/raspberry-pi-1-model-a-plus/

## Setup

http://cs107e.github.io/guides/install/

`source ./setup.sh`

## Raspberry PI

一共 54 个 GPIO，每个 GPIO 有八种工作模式，需要 3 位来选择。

手册上的地址 0x7E200000 要改成 0x20...，因为手册上 0x7E 是逻辑地址，0x20 是物理地址。

BCM 2835 手册勘误表：https://elinux.org/BCM2835_datasheet_errata#p96

## ARM

- 所有的 ARM 指令都可以条件执行

反汇编，对于 object 文件，直接使用 `arm-none-eabi-objdump --no-show-raw-insn -d input.o`，有时候会需要使用 `-m arm` 来指定 architecture。

如果是 binary 文件，先使用 `arm-none-eabi-objcopy -I binary -O elf32-littlearm input.bin output.o` 得到 object 文件。

ARM 立即数的编码，实际上是 8 位的值加上 4 位的 rotate。
https://alisdair.mcdiarmid.org/arm-immediate-value-encoding/#play-with-it

## Week 1: Introduction and Welcome

物品清单：http://cs107e.github.io/guides/bom/

ARM 指令集总设计：http://cs107e.github.io/readings/armisa.pdf

Intro to ARM assembly: http://www.toves.org/books/arm/

ARM 汇编语言：https://www.csie.ntu.edu.tw/~cyy/courses/assembly/09fall/lectures/handouts/lec09_ARMisa.pdf

[VisUAL]: 可视化的 ARM 模拟器

## Week 2

Mac Installation Guide: http://cs107e.github.io/guides/mac_toolchain/

Compiler Explorer: https://gcc.godbolt.org/

Assignment:

调整亮度，我们可以通过控制高电平时间和低电平时间比例来调整。

GPIO 输出的电压是不能变化的，要么 3.3V，要么 0V。

## Week 3

数码管模拟 http://www.uize.com/examples/seven-segment-display.html

## References

- [pinout]

[pinout]: https://pinout.xyz/
[VisUAL]: https://salmanarif.bitbucket.io/visual/index.html
