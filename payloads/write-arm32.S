.text
	.arm

	.global _start
_start:
	mov r0, #0
	mcr p15, 0, r0, c8, c7, 0
	mcr p15, 0, r0, c7, c5, 0
	mcr p15, 0, r0, c7, c5, 6
	mcr p15, 0, r0, c7, c10, 4
	mcr p15, 0, r0, c7, c5, 4
	b reset

_memmove:
	cmp		r1, r0
	bcc		.Lmemcpy_backwards
	moveq	r0, #0
	moveq	pc, lr
	stmdb	sp!, {r0, lr}
	subs	r2, r2, #4
	blt		.Lmemcpy_fl4
	ands	r12, r0, #3
	bne		.Lmemcpy_fdestul
	ands	r12, r1, #3
	bne		.Lmemcpy_fsrcul
.Lmemcpy_ft8:
	subs	r2, r2, #8
	blt		.Lmemcpy_fl12
	subs	r2, r2, #0x14
	blt		.Lmemcpy_fl32
	stmdb	sp!, {r4}
.Lmemcpy_floop32:
	ldmia	r1!, {r3, r4, r12, lr}
	stmia	r0!, {r3, r4, r12, lr}
	ldmia	r1!, {r3, r4, r12, lr}
	stmia	r0!, {r3, r4, r12, lr}
	subs	r2, r2, #0x20
	bge		.Lmemcpy_floop32
	cmn		r2, #0x10
	ldmgeia	r1!, {r3, r4, r12, lr}
	stmgeia	r0!, {r3, r4, r12, lr}
	subge	r2, r2, #0x10
	ldmia	sp!, {r4}
.Lmemcpy_fl32:
	adds	r2, r2, #0x14
.Lmemcpy_floop12:
	ldmgeia	r1!, {r3, r12, lr}
	stmgeia	r0!, {r3, r12, lr}
	subges	r2, r2, #0x0c
	bge		.Lmemcpy_floop12
.Lmemcpy_fl12:
	adds	r2, r2, #8
	blt		.Lmemcpy_fl4
	subs	r2, r2, #4
	ldrlt	r3, [r1], #4
	strlt	r3, [r0], #4
	ldmgeia	r1!, {r3, r12}
	stmgeia	r0!, {r3, r12}
	subge	r2, r2, #4
.Lmemcpy_fl4:
	adds	r2, r2, #4
	ldmeqia	sp!, {r0, pc}
	cmp		r2, #2
	ldrb	r3, [r1], #1
	strb	r3, [r0], #1
	ldrgeb	r3, [r1], #1
	strgeb	r3, [r0], #1
	ldrgtb	r3, [r1], #1
	strgtb	r3, [r0], #1
	ldmia	sp!, {r0, pc}
.Lmemcpy_fdestul:
	rsb		r12, r12, #4
	cmp		r12, #2
	ldrb	r3, [r1], #1
	strb	r3, [r0], #1
	ldrgeb	r3, [r1], #1
	strgeb	r3, [r0], #1
	ldrgtb	r3, [r1], #1
	strgtb	r3, [r0], #1
	subs	r2, r2, r12
	blt		.Lmemcpy_fl4
	ands	r12, r1, #3
	beq		.Lmemcpy_ft8
.Lmemcpy_fsrcul:
	bic		r1, r1, #3
	ldr		lr, [r1], #4
	cmp		r12, #2
	bgt		.Lmemcpy_fsrcul3
	beq		.Lmemcpy_fsrcul2
	cmp		r2, #0x0c
	blt		.Lmemcpy_fsrcul1loop4
	sub		r2, r2, #0x0c
	stmdb	sp!, {r4, r5}
.Lmemcpy_fsrcul1loop16:
	mov		r3, lr, lsr #8
	ldmia	r1!, {r4, r5, r12, lr}
	orr		r3, r3, r4, lsl #24
	mov		r4, r4, lsr #8
	orr		r4, r4, r5, lsl #24
	mov		r5, r5, lsr #8
	orr		r5, r5, r12, lsl #24
	mov		r12, r12, lsr #8
	orr		r12, r12, lr, lsl #24
	stmia	r0!, {r3-r5, r12}
	subs	r2, r2, #0x10
	bge		.Lmemcpy_fsrcul1loop16
	ldmia	sp!, {r4, r5}
	adds	r2, r2, #0x0c
	blt		.Lmemcpy_fsrcul1l4
.Lmemcpy_fsrcul1loop4:
	mov		r12, lr, lsr #8
	ldr		lr, [r1], #4
	orr		r12, r12, lr, lsl #24
	str		r12, [r0], #4
	subs	r2, r2, #4
	bge		.Lmemcpy_fsrcul1loop4
.Lmemcpy_fsrcul1l4:
	sub		r1, r1, #3
	b		.Lmemcpy_fl4
.Lmemcpy_fsrcul2:
	cmp		r2, #0x0c
	blt		.Lmemcpy_fsrcul2loop4
	sub		r2, r2, #0x0c
	stmdb	sp!, {r4, r5}
.Lmemcpy_fsrcul2loop16:
	mov		r3, lr, lsr #16
	ldmia	r1!, {r4, r5, r12, lr}
	orr		r3, r3, r4, lsl #16
	mov		r4, r4, lsr #16
	orr		r4, r4, r5, lsl #16
	mov		r5, r5, lsr #16
	orr		r5, r5, r12, lsl #16
	mov		r12, r12, lsr #16
	orr		r12, r12, lr, lsl #16
	stmia	r0!, {r3-r5, r12}
	subs	r2, r2, #0x10
	bge		.Lmemcpy_fsrcul2loop16
	ldmia	sp!, {r4, r5}
	adds	r2, r2, #0x0c
	blt		.Lmemcpy_fsrcul2l4
.Lmemcpy_fsrcul2loop4:
	mov		r12, lr, lsr #16
	ldr		lr, [r1], #4
	orr		r12, r12, lr, lsl #16
	str		r12, [r0], #4
	subs	r2, r2, #4
	bge		.Lmemcpy_fsrcul2loop4
.Lmemcpy_fsrcul2l4:
	sub		r1, r1, #2
	b		.Lmemcpy_fl4
.Lmemcpy_fsrcul3:
	cmp		r2, #0x0c
	blt		.Lmemcpy_fsrcul3loop4
	sub		r2, r2, #0x0c
	stmdb	sp!, {r4, r5}
.Lmemcpy_fsrcul3loop16:
	mov		r3, lr, lsr #24
	ldmia	r1!, {r4, r5, r12, lr}
	orr		r3, r3, r4, lsl #8
	mov		r4, r4, lsr #24
	orr		r4, r4, r5, lsl #8
	mov		r5, r5, lsr #24
	orr		r5, r5, r12, lsl #8
	mov		r12, r12, lsr #24
	orr		r12, r12, lr, lsl #8
	stmia	r0!, {r3-r5, r12}
	subs	r2, r2, #0x10
	bge		.Lmemcpy_fsrcul3loop16
	ldmia	sp!, {r4, r5}
	adds	r2, r2, #0x0c
	blt		.Lmemcpy_fsrcul3l4
.Lmemcpy_fsrcul3loop4:
	mov		r12, lr, lsr #24
	ldr		lr, [r1], #4
	orr		r12, r12, lr, lsl #8
	str		r12, [r0], #4
	subs	r2, r2, #4
	bge		.Lmemcpy_fsrcul3loop4
.Lmemcpy_fsrcul3l4:
	sub		r1, r1, #1
	b		.Lmemcpy_fl4
.Lmemcpy_backwards:
	add		r1, r1, r2
	add		r0, r0, r2
	subs	r2, r2, #4
	blt		.Lmemcpy_bl4
	ands	r12, r0, #3
	bne		.Lmemcpy_bdestul
	ands	r12, r1, #3
	bne		.Lmemcpy_bsrcul
.Lmemcpy_bt8:
	subs	r2, r2, #8
	blt		.Lmemcpy_bl12
	stmdb	sp!, {r4, lr}
	subs	r2, r2, #0x14
	blt		.Lmemcpy_bl32
.Lmemcpy_bloop32:
	ldmdb	r1!, {r3, r4, r12, lr}
	stmdb	r0!, {r3, r4, r12, lr}
	ldmdb	r1!, {r3, r4, r12, lr}
	stmdb	r0!, {r3, r4, r12, lr}
	subs	r2, r2, #0x20
	bge		.Lmemcpy_bloop32
.Lmemcpy_bl32:
	cmn	r2, #0x10
	ldmgedb	r1!, {r3, r4, r12, lr}
	stmgedb	r0!, {r3, r4, r12, lr}
	subge	r2, r2, #0x10
	adds	r2, r2, #0x14
	ldmgedb	r1!, {r3, r12, lr}
	stmgedb	r0!, {r3, r12, lr}
	subge	r2, r2, #0x0c
	ldmia	sp!, {r4, lr}
.Lmemcpy_bl12:
	adds	r2, r2, #8
	blt		.Lmemcpy_bl4
	subs	r2, r2, #4
	ldrlt	r3, [r1, #-4]!
	strlt	r3, [r0, #-4]!
	ldmgedb	r1!, {r3, r12}
	stmgedb	r0!, {r3, r12}
	subge	r2, r2, #4
.Lmemcpy_bl4:
	adds	r2, r2, #4
	moveq	pc, lr
	cmp		r2, #2
	ldrb	r3, [r1, #-1]!
	strb	r3, [r0, #-1]!
	ldrgeb	r3, [r1, #-1]!
	strgeb	r3, [r0, #-1]!
	ldrgtb	r3, [r1, #-1]!
	strgtb	r3, [r0, #-1]!
	mov		pc, lr
.Lmemcpy_bdestul:
	cmp		r12, #2
	ldrb	r3, [r1, #-1]!
	strb	r3, [r0, #-1]!
	ldrgeb	r3, [r1, #-1]!
	strgeb	r3, [r0, #-1]!
	ldrgtb	r3, [r1, #-1]!
	strgtb	r3, [r0, #-1]!
	subs	r2, r2, r12
	blt		.Lmemcpy_bl4
	ands	r12, r1, #3
	beq		.Lmemcpy_bt8
.Lmemcpy_bsrcul:
	bic		r1, r1, #3
	ldr		r3, [r1, #0]
	cmp		r12, #2
	blt		.Lmemcpy_bsrcul1
	beq		.Lmemcpy_bsrcul2
	cmp		r2, #0x0c
	blt		.Lmemcpy_bsrcul3loop4
	sub		r2, r2, #0x0c
	stmdb	sp!, {r4, r5, lr}
.Lmemcpy_bsrcul3loop16:
	mov		lr, r3, lsl #8
	ldmdb	r1!, {r3-r5, r12}
	orr		lr, lr, r12, lsr #24
	mov		r12, r12, lsl #8
	orr		r12, r12, r5, lsr #24
	mov		r5, r5, lsl #8
	orr		r5, r5, r4, lsr #24
	mov		r4, r4, lsl #8
	orr		r4, r4, r3, lsr #24
	stmdb	r0!, {r4, r5, r12, lr}
	subs	r2, r2, #0x10
	bge		.Lmemcpy_bsrcul3loop16
	ldmia	sp!, {r4, r5, lr}
	adds	r2, r2, #0x0c
	blt		.Lmemcpy_bsrcul3l4
.Lmemcpy_bsrcul3loop4:
	mov		r12, r3, lsl #8
	ldr		r3, [r1, #-4]!
	orr		r12, r12, r3, lsr #24
	str		r12, [r0, #-4]!
	subs	r2, r2, #4
	bge		.Lmemcpy_bsrcul3loop4
.Lmemcpy_bsrcul3l4:
	add		r1, r1, #3
	b		.Lmemcpy_bl4
.Lmemcpy_bsrcul2:
	cmp		r2, #0x0c
	blt		.Lmemcpy_bsrcul2loop4
	sub		r2, r2, #0x0c
	stmdb	sp!, {r4, r5, lr}
.Lmemcpy_bsrcul2loop16:
	mov		lr, r3, lsl #16
	ldmdb	r1!, {r3-r5, r12}
	orr		lr, lr, r12, lsr #16
	mov		r12, r12, lsl #16
	orr		r12, r12, r5, lsr #16
	mov		r5, r5, lsl #16
	orr		r5, r5, r4, lsr #16
	mov		r4, r4, lsl #16
	orr		r4, r4, r3, lsr #16
	stmdb	r0!, {r4, r5, r12, lr}
	subs	r2, r2, #0x10
	bge		.Lmemcpy_bsrcul2loop16
	ldmia	sp!, {r4, r5, lr}
	adds	r2, r2, #0x0c
	blt		.Lmemcpy_bsrcul2l4
.Lmemcpy_bsrcul2loop4:
	mov		r12, r3, lsl #16
	ldr		r3, [r1, #-4]!
	orr		r12, r12, r3, lsr #16
	str		r12, [r0, #-4]!
	subs	r2, r2, #4
	bge		.Lmemcpy_bsrcul2loop4
.Lmemcpy_bsrcul2l4:
	add		r1, r1, #2
	b		.Lmemcpy_bl4
.Lmemcpy_bsrcul1:
	cmp		r2, #0x0c
	blt		.Lmemcpy_bsrcul1loop4
	sub		r2, r2, #0x0c
	stmdb	sp!, {r4, r5, lr}
.Lmemcpy_bsrcul1loop32:
	mov		lr, r3, lsl #24
	ldmdb	r1!, {r3-r5, r12}
	orr		lr, lr, r12, lsr #8
	mov		r12, r12, lsl #24
	orr		r12, r12, r5, lsr #8
	mov		r5, r5, lsl #24
	orr		r5, r5, r4, lsr #8
	mov		r4, r4, lsl #24
	orr		r4, r4, r3, lsr #8
	stmdb	r0!, {r4, r5, r12, lr}
	subs	r2, r2, #0x10
	bge		.Lmemcpy_bsrcul1loop32
	ldmia	sp!, {r4, r5, lr}
	adds	r2, r2, #0x0c
	blt		.Lmemcpy_bsrcul1l4
.Lmemcpy_bsrcul1loop4:
	mov		r12, r3, lsl #24
	ldr		r3, [r1, #-4]!
	orr		r12, r12, r3, lsr #8
	str		r12, [r0, #-4]!
	subs	r2, r2, #4
	bge		.Lmemcpy_bsrcul1loop4
.Lmemcpy_bsrcul1l4:
	add		r1, r1, #1
	b		.Lmemcpy_bl4

_maskrom:
	.long 0x0, 0x0, 0x0, 0x0, 0x0, 0x0

reset:
	adr r0, _maskrom
	str sp, [r0, #0]
	str lr, [r0, #4]
	mrs lr, cpsr
	str lr, [r0, #8]
	mrc p15, 0, lr, c1, c0, 0
	str lr, [r0, #12]
	mrc p15, 0, lr, c12, c0, 0
	str lr, [r0, #16]
	mrc p15, 0, lr, c1, c0, 0
	str lr, [r0, #20]

	adr r0, _prog_data
	ldr r1, _prog_address
	cmp r0, r1
	beq 1f
	ldr r0, _prog_address
	adr r1, _prog_data
	ldr r2, _prog_size
	bl _memmove
1:	nop

	adr r0, _maskrom
	ldr sp, [r0, #0]
	ldr lr, [r0, #4]
	ldr r1, [r0, #20]
	mcr p15, 0, r1, c1, c0, 0
	ldr r1, [r0, #16]
	mcr p15, 0, r1, c12, c0, 0
	ldr r1, [r0, #12]
	mcr p15, 0, r1, c1, c0, 0
	ldr r1, [r0, #8]
	msr cpsr, r1
	bx lr

_prog_address:
	.long 0x00000000
_prog_size:
	.long 0x00000004
_prog_data:
	.long 0xdeadbeef
