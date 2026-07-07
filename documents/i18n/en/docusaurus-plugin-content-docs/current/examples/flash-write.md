# Write a Loader to Flash

This example shows how to flash a loader to the flash in loader mode.

## Prerequisites

- The device has entered loader mode via `maskrom` or `download`;
- A packaged loader file (rkloader format) is ready.

## Using the upgrade command (recommended)

The `upgrade` command reads the IDB from the loader and writes it to the beginning of the flash. The write offset is adjusted automatically based on the storage media type:

```shell
xrock upgrade update.bin
```

## Manual flashing

If you need manual control over the write location, use the `flash` commands:

```shell
xrock flash                                # Detect flash, confirm capacity and sector count
xrock flash erase 0x40 0x2000              # Erase 0x2000 sectors starting at 0x40
xrock flash write 0x40 loader.bin          # Write the loader at sector 0x40
```

:::note
- The start sector differs by storage media: Flash/eMMC/SD usually 64, SPI NOR 128, SPI NAND 512. The `upgrade` command handles this automatically; for manual flashing, pay attention to the offset.
- Use `xrock storage` to confirm the current storage media before flashing.
:::

## Verification

After flashing, reset the chip:

```shell
xrock reset
```

The chip should boot from the newly flashed loader.
