	.global _start
_start:
    isb
    dsb sy
	b reset

	.align 2
_uart_address:
	.word 0x2ad40000
_dump_address:
	.word 0xffff0000
_dump_size:
	.word 0x00000010

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

	ldr w0, _dump_address
	ldr w1, _dump_address
	ldr w2, _dump_size
	bl hexdump

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

	.global sys_uart_putc
sys_uart_putc:
	ldr w1, _uart_address
wait:
	add	x2, x1, #0x7c
	ldr w2, [x2]
	and	w2, w2, #0x2
	cmp	w2, #0x0
	b.eq wait
	str x0, [x1]
	ret
