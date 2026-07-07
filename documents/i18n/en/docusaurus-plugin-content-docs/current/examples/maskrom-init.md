# Initialize the Chip and Enter Loader Mode

This example shows how to use XROCK to initialize the chip in maskrom mode and bring it into loader mode.

## Prerequisites

- The device is in maskrom mode;
- The DDR and usbplug binaries for the chip are ready (available from [rkbin](https://github.com/rockchip-linux/rkbin)).

## Option 1: Using the maskrom command (recommended)

Taking RV1106 as an example (requires `--rc4-off`):

```shell
xrock maskrom rv1106_ddr_924MHz_v1.15.bin rv1106_usbplug_v1.09.bin --rc4-off
xrock version
```

Taking RK3399 as an example (RC4 on by default):

```shell
xrock maskrom rk3399_ddr_800MHz_v1.30.bin rk3399_usbplug_v1.30.bin
xrock version
```

## Option 2: Using the extra maskrom command (fine control)

You can control the RC4 flag and delay for SRAM/DRAM uploads separately:

```shell
xrock extra maskrom --rc4 off \
  --sram rv1106_ddr_924MHz_v1.15.bin --delay 10 \
  --rc4 off --dram rv1106_usbplug_v1.09.bin --delay 10
xrock version
```

## Option 3: Initialize DDR only

If you only need to initialize DDR memory (without entering loader mode):

```shell
xrock extra maskrom --rc4 off --sram rv1106_ddr_924MHz_v1.15.bin --delay 10
```

## Verification

Run `xrock version`; if it prints the chip name and version, the chip has successfully entered loader mode.

See the [Chip Reference List](../reference/chip-list) for the DDR/usbplug filenames and RC4 config of each chip.
