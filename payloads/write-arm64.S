	.global _start
_start:
	isb
	dsb sy
	nop
	b reset

_memmove:
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
