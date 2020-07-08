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
	.file	"timer.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.align	2
	.global	timer_init
	.type	timer_init, %function
timer_init:
.LFB0:
	.file 1 "timer.c"
	.loc 1 3 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	bx	lr
	.cfi_endproc
.LFE0:
	.size	timer_init, .-timer_init
	.align	2
	.global	timer_get_ticks
	.type	timer_get_ticks, %function
timer_get_ticks:
.LFB1:
	.loc 1 6 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 8 0
	mov	r0, #0
	bx	lr
	.cfi_endproc
.LFE1:
	.size	timer_get_ticks, .-timer_get_ticks
	.align	2
	.global	timer_delay_us
	.type	timer_delay_us, %function
timer_delay_us:
.LFB2:
	.loc 1 10 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.LVL0:
.L4:
	.loc 1 12 0 discriminator 1
	cmp	r0, #0
	bne	.L4
	.loc 1 13 0
	bx	lr
	.cfi_endproc
.LFE2:
	.size	timer_delay_us, .-timer_delay_us
	.align	2
	.global	timer_delay_ms
	.type	timer_delay_ms, %function
timer_delay_ms:
.LFB3:
	.loc 1 15 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
.LVL1:
	stmfd	sp!, {r3, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	.cfi_offset 14, -4
	.loc 1 16 0
	rsb	r3, r0, r0, asl #5
	add	r0, r0, r3, asl #2
.LVL2:
	mov	r0, r0, asl #3
	bl	timer_delay_us
.LVL3:
	.loc 1 17 0
	ldmfd	sp!, {r3, lr}
	bx	lr
	.cfi_endproc
.LFE3:
	.size	timer_delay_ms, .-timer_delay_ms
	.align	2
	.global	timer_delay
	.type	timer_delay, %function
timer_delay:
.LFB4:
	.loc 1 19 0
	.cfi_startproc
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
.LVL4:
	stmfd	sp!, {r3, lr}
	.cfi_def_cfa_offset 8
	.cfi_offset 3, -8
	.cfi_offset 14, -4
	.loc 1 20 0
	rsb	r3, r0, r0, asl #5
	rsb	r3, r3, r3, asl #6
	add	r0, r0, r3, asl #3
.LVL5:
	mov	r0, r0, asl #6
	bl	timer_delay_us
.LVL6:
	.loc 1 21 0
	ldmfd	sp!, {r3, lr}
	bx	lr
	.cfi_endproc
.LFE4:
	.size	timer_delay, .-timer_delay
.Letext0:
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x101
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
	.byte	0x3
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x3
	.4byte	.LASF9
	.byte	0x1
	.byte	0x6
	.4byte	0x4b
	.4byte	.LFB1
	.4byte	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x4
	.byte	0x4
	.byte	0x7
	.4byte	.LASF10
	.uleb128 0x5
	.4byte	.LASF0
	.byte	0x1
	.byte	0xa
	.4byte	.LFB2
	.4byte	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x80
	.uleb128 0x6
	.4byte	.LASF2
	.byte	0x1
	.byte	0xa
	.4byte	0x4b
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x7
	.4byte	.LASF11
	.byte	0x1
	.byte	0xb
	.4byte	0x4b
	.byte	0
	.uleb128 0x5
	.4byte	.LASF1
	.byte	0x1
	.byte	0xf
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xc0
	.uleb128 0x8
	.4byte	.LASF3
	.byte	0x1
	.byte	0xf
	.4byte	0x4b
	.4byte	.LLST0
	.uleb128 0x9
	.4byte	.LVL3
	.4byte	0x52
	.uleb128 0xa
	.uleb128 0x1
	.byte	0x50
	.uleb128 0xd
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x4f
	.byte	0x1e
	.byte	0x32
	.byte	0x24
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x22
	.byte	0x33
	.byte	0x24
	.byte	0
	.byte	0
	.uleb128 0xb
	.4byte	.LASF12
	.byte	0x1
	.byte	0x13
	.4byte	.LFB4
	.4byte	.LFE4-.LFB4
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x8
	.4byte	.LASF4
	.byte	0x1
	.byte	0x13
	.4byte	0x4b
	.4byte	.LLST1
	.uleb128 0x9
	.4byte	.LVL6
	.4byte	0x52
	.uleb128 0xa
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x15
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x4f
	.byte	0x1e
	.byte	0x36
	.byte	0x24
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x4f
	.byte	0x1e
	.byte	0x1c
	.byte	0x33
	.byte	0x24
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x22
	.byte	0x36
	.byte	0x24
	.byte	0
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
	.uleb128 0x4
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
	.uleb128 0x5
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
	.uleb128 0x6
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
	.uleb128 0x7
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
	.uleb128 0x8
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
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x410a
	.byte	0
	.uleb128 0x2
	.uleb128 0x18
	.uleb128 0x2111
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xb
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
	.4byte	.LFE3-.Ltext0
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.4byte	0
	.4byte	0
.LLST1:
	.4byte	.LVL4-.Ltext0
	.4byte	.LVL5-.Ltext0
	.2byte	0x1
	.byte	0x50
	.4byte	.LVL5-.Ltext0
	.4byte	.LFE4-.Ltext0
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
.LASF10:
	.ascii	"unsigned int\000"
.LASF4:
	.ascii	"secs\000"
.LASF8:
	.ascii	"timer_init\000"
.LASF5:
	.ascii	"GNU C 4.8.3 -fpreprocessed -g -Og -std=c99 -ffreest"
	.ascii	"anding\000"
.LASF6:
	.ascii	"timer.c\000"
.LASF1:
	.ascii	"timer_delay_ms\000"
.LASF2:
	.ascii	"usecs\000"
.LASF3:
	.ascii	"msecs\000"
.LASF7:
	.ascii	"/Users/cj/Documents/cs107e-home/cs107e.github.io/le"
	.ascii	"ctures/Linking/code/blink\000"
.LASF11:
	.ascii	"start\000"
.LASF0:
	.ascii	"timer_delay_us\000"
.LASF9:
	.ascii	"timer_get_ticks\000"
.LASF12:
	.ascii	"timer_delay\000"
	.ident	"GCC: (GNU) 4.8.3"
