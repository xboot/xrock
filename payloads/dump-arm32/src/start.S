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

	.align 2
_uart_address:
	.long 0xff4c0000
_dump_address:
	.long 0x00000000
_dump_size:
	.long 0x00000000

	.align 2
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

	ldr r0, _dump_address
	ldr r1, _dump_address
	ldr r2, _dump_size
	bl hexdump

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

	.global sys_uart_putc
sys_uart_putc:
	ldr r1, _uart_address
wait:
	add	r2, r1, #0x7c
	ldr r2, [r2]
	and	r2, r2, #0x2
	cmp	r2, #0x0
	beq wait
	str r0, [r1]
	bx lr
