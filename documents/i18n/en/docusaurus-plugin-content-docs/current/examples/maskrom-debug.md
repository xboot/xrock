# Memory Debugging in Maskrom Mode

This example shows how to dump memory via the debug UART, write firmware, and execute code in maskrom mode.

## Scenario

In maskrom mode, some memory regions (such as the BootROM) cannot be accessed through normal loader commands. XROCK provides the `extra maskrom-dump-*` commands, which upload a small program that outputs memory data over the debug UART.

## Example: Dump memory on RV1106

```shell
# Initialize DDR only (optional; dumping the BootROM usually does not need it)
xrock extra maskrom --rc4 off --sram rv1106_ddr_924MHz_v1.15.bin --delay 10

# Dump 1024 bytes at 0xffff0000 via the debug UART
xrock extra maskrom-dump-arm32 --rc4 off --uart 0xff4c0000 0xffff0000 1024
```

The `--uart` argument is the register base address of the debug UART, which varies by chip:

| Chip | Debug UART base | Architecture |
|------|-----------------|--------------|
| RV1106 | `0xff4c0000` | arm32 |
| RK1808 | `0xff550000` | arm64 |
| RK3128 | `0xff1a0000` | arm32 |
| RK3288 | `0xff690000` | arm32 |
| RK3399 | `0xff1a0000` | arm64 |
| PX30 | `0xff160000` | arm64 |
| RK3576 | `0x2ad40000` | arm64 |
| RK3506 | `0xff0a0000` | arm32 |

## Example: Write firmware and execute

```shell
# Initialize DDR
xrock extra maskrom --rc4 off --sram rv1106_ddr_924MHz_v1.15.bin --delay 10

# Write firmware to memory
xrock extra maskrom-write-arm32 --rc4 off 0x00000000 xstar.bin

# Jump and execute
xrock extra maskrom-exec-arm32 --rc4 off 0x00000000
```

:::note
The memory address starts from 0, **NOT** the SDRAM physical address. Choose `arm32`/`arm64` according to the chip architecture.
:::
