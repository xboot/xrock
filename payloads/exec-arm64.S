	.global _start
_start:
	isb
	dsb sy
	b reset

	.align 2
_exec_address:
	.word 0x11223344

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

	mov x0, xzr
	mov x1, xzr
	ldr w2, _exec_address
	br x2

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
