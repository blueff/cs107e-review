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
	.file	"blink.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.file 1 "blink.c"
	.loc 1 4 0
	.cfi_startproc
	@ Function supports interworking.
	@ Volatile: function does not return.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r3, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	.cfi_offset 14, -4
	.loc 1 6 0
	bl	gpio_init
.LVL0:
	.loc 1 7 0
	bl	timer_init
.LVL1:
	.loc 1 9 0
	mov	r0, #20
	mov	r1, #1
	bl	gpio_set_function
.LVL2:
	.loc 1 11 0
	mov	r5, #20
	mov	r4, #1
	.loc 1 13 0
	mov	r6, #0
.L2:
	.loc 1 11 0 discriminator 1
	mov	r0, r5
	mov	r1, r4
	bl	gpio_write
.LVL3:
	.loc 1 12 0 discriminator 1
	mov	r0, r4
	bl	timer_delay
.LVL4:
	.loc 1 13 0 discriminator 1
	mov	r0, r5
	mov	r1, r6
	bl	gpio_write
.LVL5:
	.loc 1 14 0 discriminator 1
	mov	r0, r4
	bl	timer_delay
.LVL6:
	b	.L2
	.cfi_endproc
.LFE0:
	.size	main, .-main
.Letext0:
	.file 2 "gpio.h"
	.file 3 "timer.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x29a
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF68
	.byte	0x1
	.4byte	.LASF69
	.4byte	.LASF70
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.byte	0x18
	.4byte	0x17e
	.uleb128 0x3
	.4byte	.LASF0
	.sleb128 0
	.uleb128 0x3
	.4byte	.LASF1
	.sleb128 0
	.uleb128 0x3
	.4byte	.LASF2
	.sleb128 1
	.uleb128 0x3
	.4byte	.LASF3
	.sleb128 2
	.uleb128 0x3
	.4byte	.LASF4
	.sleb128 3
	.uleb128 0x3
	.4byte	.LASF5
	.sleb128 4
	.uleb128 0x3
	.4byte	.LASF6
	.sleb128 5
	.uleb128 0x3
	.4byte	.LASF7
	.sleb128 6
	.uleb128 0x3
	.4byte	.LASF8
	.sleb128 7
	.uleb128 0x3
	.4byte	.LASF9
	.sleb128 8
	.uleb128 0x3
	.4byte	.LASF10
	.sleb128 9
	.uleb128 0x3
	.4byte	.LASF11
	.sleb128 10
	.uleb128 0x3
	.4byte	.LASF12
	.sleb128 11
	.uleb128 0x3
	.4byte	.LASF13
	.sleb128 12
	.uleb128 0x3
	.4byte	.LASF14
	.sleb128 13
	.uleb128 0x3
	.4byte	.LASF15
	.sleb128 14
	.uleb128 0x3
	.4byte	.LASF16
	.sleb128 15
	.uleb128 0x3
	.4byte	.LASF17
	.sleb128 16
	.uleb128 0x3
	.4byte	.LASF18
	.sleb128 17
	.uleb128 0x3
	.4byte	.LASF19
	.sleb128 18
	.uleb128 0x3
	.4byte	.LASF20
	.sleb128 19
	.uleb128 0x3
	.4byte	.LASF21
	.sleb128 20
	.uleb128 0x3
	.4byte	.LASF22
	.sleb128 21
	.uleb128 0x3
	.4byte	.LASF23
	.sleb128 22
	.uleb128 0x3
	.4byte	.LASF24
	.sleb128 23
	.uleb128 0x3
	.4byte	.LASF25
	.sleb128 24
	.uleb128 0x3
	.4byte	.LASF26
	.sleb128 25
	.uleb128 0x3
	.4byte	.LASF27
	.sleb128 26
	.uleb128 0x3
	.4byte	.LASF28
	.sleb128 27
	.uleb128 0x3
	.4byte	.LASF29
	.sleb128 28
	.uleb128 0x3
	.4byte	.LASF30
	.sleb128 29
	.uleb128 0x3
	.4byte	.LASF31
	.sleb128 30
	.uleb128 0x3
	.4byte	.LASF32
	.sleb128 31
	.uleb128 0x3
	.4byte	.LASF33
	.sleb128 32
	.uleb128 0x3
	.4byte	.LASF34
	.sleb128 33
	.uleb128 0x3
	.4byte	.LASF35
	.sleb128 34
	.uleb128 0x3
	.4byte	.LASF36
	.sleb128 35
	.uleb128 0x3
	.4byte	.LASF37
	.sleb128 36
	.uleb128 0x3
	.4byte	.LASF38
	.sleb128 37
	.uleb128 0x3
	.4byte	.LASF39
	.sleb128 38
	.uleb128 0x3
	.4byte	.LASF40
	.sleb128 39
	.uleb128 0x3
	.4byte	.LASF41
	.sleb128 40
	.uleb128 0x3
	.4byte	.LASF42
	.sleb128 41
	.uleb128 0x3
	.4byte	.LASF43
	.sleb128 42
	.uleb128 0x3
	.4byte	.LASF44
	.sleb128 43
	.uleb128 0x3
	.4byte	.LASF45
	.sleb128 44
	.uleb128 0x3
	.4byte	.LASF46
	.sleb128 45
	.uleb128 0x3
	.4byte	.LASF47
	.sleb128 46
	.uleb128 0x3
	.4byte	.LASF48
	.sleb128 47
	.uleb128 0x3
	.4byte	.LASF49
	.sleb128 48
	.uleb128 0x3
	.4byte	.LASF50
	.sleb128 49
	.uleb128 0x3
	.4byte	.LASF51
	.sleb128 50
	.uleb128 0x3
	.4byte	.LASF52
	.sleb128 51
	.uleb128 0x3
	.4byte	.LASF53
	.sleb128 52
	.uleb128 0x3
	.4byte	.LASF54
	.sleb128 53
	.uleb128 0x3
	.4byte	.LASF55
	.sleb128 53
	.byte	0
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.byte	0x5b
	.4byte	0x1b7
	.uleb128 0x3
	.4byte	.LASF56
	.sleb128 0
	.uleb128 0x3
	.4byte	.LASF57
	.sleb128 1
	.uleb128 0x3
	.4byte	.LASF58
	.sleb128 4
	.uleb128 0x3
	.4byte	.LASF59
	.sleb128 5
	.uleb128 0x3
	.4byte	.LASF60
	.sleb128 6
	.uleb128 0x3
	.4byte	.LASF61
	.sleb128 7
	.uleb128 0x3
	.4byte	.LASF62
	.sleb128 3
	.uleb128 0x3
	.4byte	.LASF63
	.sleb128 2
	.byte	0
	.uleb128 0x4
	.4byte	.LASF71
	.byte	0x1
	.byte	0x4
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x24f
	.uleb128 0x5
	.4byte	.LVL0
	.4byte	0x24f
	.uleb128 0x5
	.4byte	.LVL1
	.4byte	0x256
	.uleb128 0x6
	.4byte	.LVL2
	.4byte	0x25d
	.4byte	0x1f6
	.uleb128 0x7
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x1
	.byte	0x31
	.uleb128 0x7
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x1
	.byte	0x44
	.byte	0
	.uleb128 0x6
	.4byte	.LVL3
	.4byte	0x27a
	.4byte	0x210
	.uleb128 0x7
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x2
	.byte	0x74
	.sleb128 0
	.uleb128 0x7
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x75
	.sleb128 0
	.byte	0
	.uleb128 0x6
	.4byte	.LVL4
	.4byte	0x290
	.4byte	0x224
	.uleb128 0x7
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x74
	.sleb128 0
	.byte	0
	.uleb128 0x6
	.4byte	.LVL5
	.4byte	0x27a
	.4byte	0x23e
	.uleb128 0x7
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x2
	.byte	0x76
	.sleb128 0
	.uleb128 0x7
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x75
	.sleb128 0
	.byte	0
	.uleb128 0x8
	.4byte	.LVL6
	.4byte	0x290
	.uleb128 0x7
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x74
	.sleb128 0
	.byte	0
	.byte	0
	.uleb128 0x9
	.4byte	.LASF64
	.byte	0x2
	.byte	0x6e
	.uleb128 0x9
	.4byte	.LASF65
	.byte	0x3
	.byte	0x12
	.uleb128 0xa
	.4byte	.LASF66
	.byte	0x2
	.byte	0x7a
	.4byte	0x273
	.uleb128 0xb
	.4byte	0x273
	.uleb128 0xb
	.4byte	0x273
	.byte	0
	.uleb128 0xc
	.byte	0x4
	.byte	0x7
	.4byte	.LASF72
	.uleb128 0xa
	.4byte	.LASF67
	.byte	0x2
	.byte	0x9b
	.4byte	0x290
	.uleb128 0xb
	.4byte	0x273
	.uleb128 0xb
	.4byte	0x273
	.byte	0
	.uleb128 0xd
	.4byte	.LASF73
	.byte	0x3
	.byte	0x30
	.uleb128 0xb
	.4byte	0x273
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
	.uleb128 0x4
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xd
	.byte	0
	.byte	0
	.uleb128 0x4
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
	.uleb128 0x5
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x410a
	.byte	0
	.uleb128 0x2
	.uleb128 0x18
	.uleb128 0x2111
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
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
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0xa
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
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
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
	.uleb128 0xd
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
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
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
.LASF32:
	.ascii	"GPIO_PIN31\000"
.LASF26:
	.ascii	"GPIO_PIN25\000"
.LASF33:
	.ascii	"GPIO_PIN32\000"
.LASF27:
	.ascii	"GPIO_PIN26\000"
.LASF28:
	.ascii	"GPIO_PIN27\000"
.LASF29:
	.ascii	"GPIO_PIN28\000"
.LASF69:
	.ascii	"blink.c\000"
.LASF42:
	.ascii	"GPIO_PIN41\000"
.LASF67:
	.ascii	"gpio_write\000"
.LASF43:
	.ascii	"GPIO_PIN42\000"
.LASF51:
	.ascii	"GPIO_PIN50\000"
.LASF52:
	.ascii	"GPIO_PIN51\000"
.LASF61:
	.ascii	"GPIO_FUNC_ALT3\000"
.LASF53:
	.ascii	"GPIO_PIN52\000"
.LASF62:
	.ascii	"GPIO_FUNC_ALT4\000"
.LASF63:
	.ascii	"GPIO_FUNC_ALT5\000"
.LASF11:
	.ascii	"GPIO_PIN10\000"
.LASF12:
	.ascii	"GPIO_PIN11\000"
.LASF13:
	.ascii	"GPIO_PIN12\000"
.LASF14:
	.ascii	"GPIO_PIN13\000"
.LASF15:
	.ascii	"GPIO_PIN14\000"
.LASF16:
	.ascii	"GPIO_PIN15\000"
.LASF17:
	.ascii	"GPIO_PIN16\000"
.LASF18:
	.ascii	"GPIO_PIN17\000"
.LASF19:
	.ascii	"GPIO_PIN18\000"
.LASF20:
	.ascii	"GPIO_PIN19\000"
.LASF34:
	.ascii	"GPIO_PIN33\000"
.LASF35:
	.ascii	"GPIO_PIN34\000"
.LASF36:
	.ascii	"GPIO_PIN35\000"
.LASF37:
	.ascii	"GPIO_PIN36\000"
.LASF38:
	.ascii	"GPIO_PIN37\000"
.LASF39:
	.ascii	"GPIO_PIN38\000"
.LASF40:
	.ascii	"GPIO_PIN39\000"
.LASF54:
	.ascii	"GPIO_PIN53\000"
.LASF41:
	.ascii	"GPIO_PIN40\000"
.LASF57:
	.ascii	"GPIO_FUNC_OUTPUT\000"
.LASF65:
	.ascii	"timer_init\000"
.LASF71:
	.ascii	"main\000"
.LASF64:
	.ascii	"gpio_init\000"
.LASF0:
	.ascii	"GPIO_PIN_FIRST\000"
.LASF72:
	.ascii	"unsigned int\000"
.LASF68:
	.ascii	"GNU C 4.8.3 -fpreprocessed -g -Og -std=c99 -ffreest"
	.ascii	"anding\000"
.LASF58:
	.ascii	"GPIO_FUNC_ALT0\000"
.LASF59:
	.ascii	"GPIO_FUNC_ALT1\000"
.LASF66:
	.ascii	"gpio_set_function\000"
.LASF60:
	.ascii	"GPIO_FUNC_ALT2\000"
.LASF1:
	.ascii	"GPIO_PIN0\000"
.LASF2:
	.ascii	"GPIO_PIN1\000"
.LASF3:
	.ascii	"GPIO_PIN2\000"
.LASF4:
	.ascii	"GPIO_PIN3\000"
.LASF5:
	.ascii	"GPIO_PIN4\000"
.LASF6:
	.ascii	"GPIO_PIN5\000"
.LASF7:
	.ascii	"GPIO_PIN6\000"
.LASF8:
	.ascii	"GPIO_PIN7\000"
.LASF9:
	.ascii	"GPIO_PIN8\000"
.LASF10:
	.ascii	"GPIO_PIN9\000"
.LASF30:
	.ascii	"GPIO_PIN29\000"
.LASF44:
	.ascii	"GPIO_PIN43\000"
.LASF45:
	.ascii	"GPIO_PIN44\000"
.LASF46:
	.ascii	"GPIO_PIN45\000"
.LASF47:
	.ascii	"GPIO_PIN46\000"
.LASF48:
	.ascii	"GPIO_PIN47\000"
.LASF49:
	.ascii	"GPIO_PIN48\000"
.LASF50:
	.ascii	"GPIO_PIN49\000"
.LASF70:
	.ascii	"/Users/cj/Documents/cs107e-home/cs107e.github.io/le"
	.ascii	"ctures/Linking/code/blink\000"
.LASF73:
	.ascii	"timer_delay\000"
.LASF55:
	.ascii	"GPIO_PIN_LAST\000"
.LASF21:
	.ascii	"GPIO_PIN20\000"
.LASF56:
	.ascii	"GPIO_FUNC_INPUT\000"
.LASF22:
	.ascii	"GPIO_PIN21\000"
.LASF23:
	.ascii	"GPIO_PIN22\000"
.LASF24:
	.ascii	"GPIO_PIN23\000"
.LASF31:
	.ascii	"GPIO_PIN30\000"
.LASF25:
	.ascii	"GPIO_PIN24\000"
	.ident	"GCC: (GNU) 4.8.3"
