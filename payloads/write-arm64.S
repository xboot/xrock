	.global _start
_start:
	isb
	dsb sy
	nop
	b reset

_memmove:
	sub	sp, sp, #0x30
	str	x0, [sp, #24]
	str	x1, [sp, #16]
	str	x2, [sp, #8]
	ldr	x1, [sp, #24]
	ldr	x0, [sp, #16]
	cmp	x1, x0
	b.hi ml2
	ldr	x0, [sp, #24]
	str	x0, [sp, #32]
	ldr	x0, [sp, #16]
	str	x0, [sp, #40]
	b ml1
ml0:
	ldr	x1, [sp, #40]
	add	x0, x1, #0x1
	str	x0, [sp, #40]
	ldr	x0, [sp, #32]
	add	x2, x0, #0x1
	str	x2, [sp, #32]
	ldrb w1, [x1]
	strb w1, [x0]
ml1:
	ldr	x0, [sp, #8]
	sub	x1, x0, #0x1
	str	x1, [sp, #8]
	cmp	x0, #0x0
	b.ne ml0
	b ml5
ml2:
	ldr	x0, [sp, #24]
	str	x0, [sp, #32]
	ldr	x1, [sp, #32]
	ldr	x0, [sp, #8]
	add	x0, x1, x0
	str	x0, [sp, #32]
	ldr	x0, [sp, #16]
	str	x0, [sp, #40]
	ldr	x1, [sp, #40]
	ldr	x0, [sp, #8]
	add	x0, x1, x0
	str	x0, [sp, #40]
	b ml4
ml3:
	ldr	x0, [sp, #40]
	sub	x0, x0, #0x1
	str	x0, [sp, #40]
	ldr	x0, [sp, #32]
	sub	x0, x0, #0x1
	str	x0, [sp, #32]
	ldr	x0, [sp, #40]
	ldrb w1, [x0]
	ldr	x0, [sp, #32]
	strb w1, [x0]
ml4:
	ldr	x0, [sp, #8]
	sub	x1, x0, #0x1
	str	x1, [sp, #8]
	cmp	x0, #0x0
	b.ne ml3
ml5:
	ldr	x0, [sp, #24]
	add	sp, sp, #0x30
	ret

	.align 3
_maskrom:
	.dword 0x0, 0x0, 0x0, 0x0

reset:
	adr x0, _maskrom
	mov x1, x30
	str w1, [x0]
	mov x1, x29
	str w1, [x0, #0x4]
	mov x1, x28
	str w1, [x0, #0x8]
	mov x1, sp
	str w1, [x0, #0xc]

	adr x0, _prog_data
	ldr x1, _prog_address
	cmp x0, x1
	beq 1f
	ldr x0, _prog_address
	adr x1, _prog_data
	ldr x2, _prog_size
	bl _memmove
1:	nop

	mov x1, xzr
	adr x0, _maskrom
	ldr w1, [x0]
	mov x30, x1
	ldr w1, [x0, #0x4]
	mov x29, x1
	ldr w1, [x0, #0x8]
	mov x28, x1
	ldr w1, [x0, #0xc]
	mov sp, x1
	mov x0, #0x0
	ret

	.align 3
_prog_address:
	.dword 0x1122334455667788
_prog_size:
	.dword 0x1234567890abcdef
_prog_data:
	.dword 0xdeadbeefdeadbeef
