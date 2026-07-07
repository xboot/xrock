# dump

Dump a memory region in hexadecimal format.

## Usage

```shell
xrock dump <address> <length>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Start memory address |
| `<length>` | Number of bytes to read |

## Description

Reads device memory and prints it with address, hex values and ASCII characters.

:::note
The memory address starts from 0, **NOT** the SDRAM physical address.
:::

## Example

Dump 256 bytes starting at `0x1000`:

```shell
xrock dump 0x1000 256
```
