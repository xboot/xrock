# read

Read data from device memory and save it to a file, with a progress bar.

## Usage

```shell
xrock read <address> <length> <file>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Start memory address |
| `<length>` | Number of bytes to read |
| `<file>` | Path of the file to save the data |

## Description

Reads the specified amount of memory and writes it to a local file. A progress bar is shown during transfer.

:::note
The memory address starts from 0, **NOT** the SDRAM physical address. Some u-boot rockusb drivers limit the flash dump start to within 32MB; see the `RKUSB_READ_LIMIT_ADDR` patch in the README.
:::

## Example

Read 1 MB from address `0x60000000` to `dump.bin`:

```shell
xrock read 0x60000000 0x100000 dump.bin
```
