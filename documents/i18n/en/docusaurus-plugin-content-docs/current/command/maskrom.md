# maskrom

Initialize the chip in maskrom mode using DDR and usbplug, bringing it into loader mode.

## Usage

```shell
xrock maskrom <ddr> <usbplug> [--rc4-off]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<ddr>` | DDR init binary path (uploaded to SRAM, code 0x471) |
| `<usbplug>` | usbplug binary path (uploaded to DRAM, code 0x472) |
| `--rc4-off` | Optional, disable RC4 encrypted transfer (needed by some newer chips) |

## Description

This command is only available in maskrom mode. It uploads the DDR init code to SRAM and usbplug to DRAM in order. Once usbplug runs, the chip enters loader mode, after which loader commands like `version` and `flash` can be used.

Whether `--rc4-off` is needed depends on the chip model; see the [Chip Reference List](../reference/chip-list).

## Examples

```shell
# RV1106 (RC4 off)
xrock maskrom rv1106_ddr_924MHz_v1.15.bin rv1106_usbplug_v1.09.bin --rc4-off

# RK3399 (RC4 on by default)
xrock maskrom rk3399_ddr_800MHz_v1.30.bin rk3399_usbplug_v1.30.bin
```

If the device is not in maskrom mode:

```
ERROR: The chip '<chip name>' does not in maskrom mode
```
