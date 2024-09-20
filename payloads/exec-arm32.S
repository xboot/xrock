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

_exec_address:
	.long 0x11223344
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

	mov r0, #0
	mov r1, #0
	ldr r2, _exec_address
	mov pc, r2

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
