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
	.file	"cstart.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.align	2
	.global	_cstart
	.type	_cstart, %function
_cstart:
.LFB0:
	.file 1 "cstart.c"
	.loc 1 11 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r3, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	.cfi_offset 14, -4
.LVL0:
	.loc 1 12 0
	ldr	r3, .L4
	.loc 1 15 0
	ldr	r2, .L4+4
	.loc 1 16 0
	mov	r1, #0
	.loc 1 15 0
	b	.L2
.LVL1:
.L3:
	.loc 1 16 0
	str	r1, [r3]
	add	r3, r3, #4
.LVL2:
.L2:
	.loc 1 15 0 discriminator 1
	cmp	r3, r2
	bcc	.L3
	.loc 1 19 0
	bl	main
.LVL3:
	.loc 1 24 0
	ldr	r3, .L4+8
	ldr	r2, [r3, #16]
	bic	r2, r2, #14680064
	orr	r2, r2, #2097152
	str	r2, [r3, #16]
	.loc 1 25 0
	mov	r2, #32768
	str	r2, [r3, #32]
	.loc 1 26 0
	ldmfd	sp!, {r3, lr}
	bx	lr
.L5:
	.align	2
.L4:
	.word	__bss_start__
	.word	__bss_end__
	.word	538968064
	.cfi_endproc
.LFE0:
	.size	_cstart, .-_cstart
.Letext0:
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0xb5
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF5
	.byte	0x1
	.4byte	.LASF6
	.4byte	.LASF7
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.4byte	.LASF8
	.byte	0x1
	.byte	0xa
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x7c
	.uleb128 0x3
	.ascii	"bss\000"
	.byte	0x1
	.byte	0xc
	.4byte	0x7c
	.4byte	.LLST0
	.uleb128 0x4
	.4byte	.LASF9
	.byte	0x1
	.byte	0xd
	.4byte	0x7c
	.uleb128 0x5
	.4byte	.LASF0
	.byte	0x1
	.byte	0x16
	.4byte	0x89
	.4byte	0x20200010
	.uleb128 0x5
	.4byte	.LASF1
	.byte	0x1
	.byte	0x17
	.4byte	0x89
	.4byte	0x20200020
	.uleb128 0x6
	.4byte	.LVL3
	.4byte	0xb1
	.byte	0
	.uleb128 0x7
	.byte	0x4
	.4byte	0x82
	.uleb128 0x8
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x7
	.byte	0x4
	.4byte	0x8f
	.uleb128 0x9
	.4byte	0x94
	.uleb128 0xa
	.byte	0x4
	.byte	0x7
	.4byte	.LASF2
	.uleb128 0xb
	.4byte	.LASF3
	.byte	0x1
	.byte	0x2
	.4byte	0x82
	.uleb128 0xb
	.4byte	.LASF4
	.byte	0x1
	.byte	0x2
	.4byte	0x82
	.uleb128 0xc
	.4byte	.LASF10
	.byte	0x1
	.byte	0x4
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
	.uleb128 0x3
	.uleb128 0x34
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
	.uleb128 0x4
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
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
	.uleb128 0xb
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0xc
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
	.byte	0
	.section	.debug_loc,"",%progbits
.Ldebug_loc0:
.LLST0:
	.4byte	.LVL1-.Ltext0
	.4byte	.LVL2-.Ltext0
	.2byte	0x3
	.byte	0x73
	.sleb128 4
	.byte	0x9f
	.4byte	.LVL2-.Ltext0
	.4byte	.LVL3-1-.Ltext0
	.2byte	0x1
	.byte	0x53
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
	.ascii	"unsigned int\000"
.LASF9:
	.ascii	"bss_end\000"
.LASF5:
	.ascii	"GNU C 4.8.3 -fpreprocessed -g -Og -std=c99 -ffreest"
	.ascii	"anding\000"
.LASF4:
	.ascii	"__bss_end__\000"
.LASF6:
	.ascii	"cstart.c\000"
.LASF1:
	.ascii	"GPIO_SET1\000"
.LASF8:
	.ascii	"_cstart\000"
.LASF10:
	.ascii	"main\000"
.LASF7:
	.ascii	"/Users/cj/Documents/cs107e-home/cs107e.github.io/le"
	.ascii	"ctures/Linking/code/blink\000"
.LASF0:
	.ascii	"GPIO_FSEL4\000"
.LASF3:
	.ascii	"__bss_start__\000"
	.ident	"GCC: (GNU) 4.8.3"
