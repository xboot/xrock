# extra

Extended commands in maskrom mode, providing finer-grained download and debug control.

## Usage

### Upload SRAM / DRAM files

```shell
xrock extra maskrom --rc4 <on|off> [--sram <file> --delay <ms>] [--dram <file> --delay <ms>] [...]
```

### Dump memory via debug UART

```shell
xrock extra maskrom-dump-arm32 --rc4 <on|off> --uart <register> <address> <length>
xrock extra maskrom-dump-arm64 --rc4 <on|off> --uart <register> <address> <length>
```

### Write to memory

```shell
xrock extra maskrom-write-arm32 --rc4 <on|off> <address> <file>
xrock extra maskrom-write-arm64 --rc4 <on|off> <address> <file>
```

### Execute code

```shell
xrock extra maskrom-exec-arm32 --rc4 <on|off> <address>
xrock extra maskrom-exec-arm64 --rc4 <on|off> <address>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `--rc4 <on\|off>` | Whether to enable RC4 encrypted transfer |
| `--sram <file>` | Upload file to SRAM (code 0x471, DDR init) |
| `--dram <file>` | Upload file to DRAM (code 0x472, usbplug) |
| `--delay <ms>` | Delay in milliseconds after upload |
| `--uart <register>` | Debug UART register base address |
| `<address>` | Memory address |
| `<length>` | Number of bytes to dump |
| `<file>` | File to write |

## Description

The `extra` commands are only available in maskrom mode and provide more flexible control than `maskrom`:

- **maskrom**: upload SRAM/DRAM files separately, each with its own RC4 flag and delay; supports multi-segment upload;
- **maskrom-dump-arm32/arm64**: dump memory at a given address via the debug UART (for reading areas not directly accessible, such as the BootROM);
- **maskrom-write-arm32/arm64**: write a file to a given memory address;
- **maskrom-exec-arm32/arm64**: jump to a given address to execute code.

`arm32` and `arm64` correspond to the target CPU execution mode and must match the chip architecture.

## Examples

Initialize DDR only (upload SRAM):

```shell
xrock extra maskrom --rc4 off --sram rv1106_ddr_924MHz_v1.15.bin --delay 10
```

Full init (SRAM + DRAM):

```shell
xrock extra maskrom --rc4 off \
  --sram rv1106_ddr_924MHz_v1.15.bin --delay 10 \
  --rc4 off --dram rv1106_usbplug_v1.09.bin --delay 10
```

Dump the BootROM via the debug UART:

```shell
xrock extra maskrom-dump-arm32 --rc4 off --uart 0xff4c0000 0xffff0000 1024
```

Write firmware and execute:

```shell
xrock extra maskrom-write-arm32 --rc4 off 0x00000000 xstar.bin
xrock extra maskrom-exec-arm32 --rc4 off 0x00000000
```
