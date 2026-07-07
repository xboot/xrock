# Quick Start

This section walks you through a simple example to get started with XROCK in minutes.

## Prerequisites

1. XROCK has been built as described in [Build and Install](./build-guide);
2. A Rockchip SOC development board;
3. A USB data cable connecting the board to the host;
4. The DDR and usbplug binaries for your chip (available from [rkbin](https://github.com/rockchip-linux/rkbin)).

## Step 1: Put the device into maskrom mode

Hold the board's Maskrom button (or Vol+, varies by board), power on or reset, and connect to the host via USB.

## Step 2: Initialize the chip and enter loader mode

Taking RV1106 as an example (requires `--rc4-off`):

```shell
xrock maskrom rv1106_ddr_924MHz_v1.15.bin rv1106_usbplug_v1.09.bin --rc4-off
```

On success, the chip finishes DDR init and enters loader mode.

## Step 3: Check the chip version

```shell
xrock version
```

Sample output:

```
RV1106(5618): 0x31355631 0x34312e31 0x00000000 0x00000000
```

## Step 4: Operate the flash

```shell
xrock flash                                # Detect flash and show info
xrock flash erase 0 0x2000                 # Erase 0x2000 sectors starting at sector 0
xrock flash write 0x40 loader.bin          # Write loader at sector 0x40
```

## Next steps

- [Command Reference](../command/maskrom): learn all available commands;
- [Examples](../examples/maskrom-init): see complete workflows for typical scenarios;
- [Chip Reference List](../reference/chip-list): see initialization config for each chip.
