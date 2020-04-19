	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 1
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"gpio.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.align	2
	.global	gpio_init
	.type	gpio_init, %function
gpio_init:
.LFB0:
	.file 1 "gpio.c"
	.loc 1 3 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	bx	lr
	.cfi_endproc
.LFE0:
	.size	gpio_init, .-gpio_init
	.align	2
	.global	gpio_set_function
	.type	gpio_set_function, %function
gpio_set_function:
.LFB1:
	.loc 1 6 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.LVL0:
	bx	lr
	.cfi_endproc
.LFE1:
	.size	gpio_set_function, .-gpio_set_function
	.align	2
	.global	gpio_get_function
	.type	gpio_get_function, %function
gpio_get_function:
.LFB2:
	.loc 1 10 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.LVL1:
	.loc 1 12 0
	mov	r0, #0
.LVL2:
	bx	lr
	.cfi_endproc
.LFE2:
	.size	gpio_get_function, .-gpio_get_function
	.align	2
	.global	gpio_set_input
	.type	gpio_set_input, %function
gpio_set_input:
.LFB3:
	.loc 1 14 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.LVL3:
	bx	lr
	.cfi_endproc
.LFE3:
	.size	gpio_set_input, .-gpio_set_input
	.align	2
	.global	gpio_set_output
	.type	gpio_set_output, %function
gpio_set_output:
.LFB4:
	.loc 1 18 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.LVL4:
	bx	lr
	.cfi_endproc
.LFE4:
	.size	gpio_set_output, .-gpio_set_output
	.align	2
	.global	gpio_write
	.type	gpio_write, %function
gpio_write:
.LFB5:
	.loc 1 22 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.LVL5:
	bx	lr
	.cfi_endproc
.LFE5:
	.size	gpio_write, .-gpio_write
	.align	2
	.global	gpio_read
	.type	gpio_read, %function
gpio_read:
.LFB6:
	.loc 1 26 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.LVL6:
	.loc 1 28 0
	mov	r0, #0
.LVL7:
	bx	lr
	.cfi_endproc
.LFE6:
	.size	gpio_read, .-gpio_read
.Letext0:
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x12e
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF7
	.byte	0x1
	.4byte	.LASF8
	.4byte	.LASF9
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.4byte	.LASF10
	.byte	0x1
	.byte	0x3
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x3
	.4byte	.LASF1
	.byte	0x1
	.byte	0x6
	.4byte	.LFB1
	.4byte	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x66
	.uleb128 0x4
	.ascii	"pin\000"
	.byte	0x1
	.byte	0x6
	.4byte	0x66
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x5
	.4byte	.LASF0
	.byte	0x1
	.byte	0x6
	.4byte	0x66
	.uleb128 0x1
	.byte	0x51
	.byte	0
	.uleb128 0x6
	.byte	0x4
	.byte	0x7
	.4byte	.LASF11
	.uleb128 0x7
	.4byte	.LASF12
	.byte	0x1
	.byte	0xa
	.4byte	0x66
	.4byte	.LFB2
	.4byte	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x96
	.uleb128 0x8
	.ascii	"pin\000"
	.byte	0x1
	.byte	0xa
	.4byte	0x66
	.4byte	.LLST0
	.byte	0
	.uleb128 0x3
	.4byte	.LASF2
	.byte	0x1
	.byte	0xe
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xb9
	.uleb128 0x4
	.ascii	"pin\000"
	.byte	0x1
	.byte	0xe
	.4byte	0x66
	.uleb128 0x1
	.byte	0x50
	.byte	0
	.uleb128 0x3
	.4byte	.LASF3
	.byte	0x1
	.byte	0x12
	.4byte	.LFB4
	.4byte	.LFE4-.LFB4
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xdc
	.uleb128 0x4
	.ascii	"pin\000"
	.byte	0x1
	.byte	0x12
	.4byte	0x66
	.uleb128 0x1
	.byte	0x50
	.byte	0
	.uleb128 0x3
	.4byte	.LASF4
	.byte	0x1
	.byte	0x16
	.4byte	.LFB5
	.4byte	.LFE5-.LFB5
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x10c
	.uleb128 0x4
	.ascii	"pin\000"
	.byte	0x1
	.byte	0x16
	.4byte	0x66
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x5
	.4byte	.LASF5
	.byte	0x1
	.byte	0x16
	.4byte	0x66
	.uleb128 0x1
	.byte	0x51
	.byte	0
	.uleb128 0x9
	.4byte	.LASF6
	.byte	0x1
	.byte	0x1a
	.4byte	0x66
	.4byte	.LFB6
	.4byte	.LFE6-.LFB6
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x8
	.ascii	"pin\000"
	.byte	0x1
	.byte	0x1a
	.4byte	0x66
	.4byte	.LLST1
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",%progbits
.Ldebug_loc0:
.LLST0:
	.4byte	.LVL1-.Ltext0
	.4byte	.LVL2-.Ltext0
	.2byte	0x1
	.byte	0x50
	.4byte	.LVL2-.Ltext0
	.4byte	.LFE2-.Ltext0
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.4byte	0
	.4byte	0
.LLST1:
	.4byte	.LVL6-.Ltext0
	.4byte	.LVL7-.Ltext0
	.2byte	0x1
	.byte	0x50
	.4byte	.LVL7-.Ltext0
	.4byte	.LFE6-.Ltext0
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.4byte	0
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF2:
	.ascii	"gpio_set_input\000"
.LASF11:
	.ascii	"unsigned int\000"
.LASF8:
	.ascii	"gpio.c\000"
.LASF7:
	.ascii	"GNU C 4.8.3 -fpreprocessed -g -Og -std=c99 -ffreest"
	.ascii	"anding\000"
.LASF4:
	.ascii	"gpio_write\000"
.LASF5:
	.ascii	"value\000"
.LASF1:
	.ascii	"gpio_set_function\000"
.LASF10:
	.ascii	"gpio_init\000"
.LASF9:
	.ascii	"/Users/cj/Documents/cs107e-home/cs107e.github.io/le"
	.ascii	"ctures/Linking/code/blink\000"
.LASF0:
	.ascii	"function\000"
.LASF12:
	.ascii	"gpio_get_function\000"
.LASF3:
	.ascii	"gpio_set_output\000"
.LASF6:
	.ascii	"gpio_read\000"
	.ident	"GCC: (GNU) 4.8.3"
