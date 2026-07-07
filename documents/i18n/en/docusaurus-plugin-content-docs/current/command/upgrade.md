# upgrade

Upgrade (flash) a loader to the flash in loader mode.

## Usage

```shell
xrock upgrade <loader>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<loader>` | Path of a packaged loader file (rkloader format) |

## Description

This command reads the IDB (ID Block) from the loader file and writes it to the beginning of the flash (usually starting at sector 64; SPI NOR at 128, SPI NAND at 512). The write offset is adjusted automatically based on the current storage media type.

Make sure the chip is in loader mode and the correct storage media is selected before running.

## Example

```shell
xrock upgrade update.bin
```

If flash detection fails:

```
Failed to detect flash
```
