# write

Write a local file to device memory, with a progress bar.

## Usage

```shell
xrock write <address> <file>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Start memory address |
| `<file>` | Path of the local file to write |

## Description

Reads the entire contents of a local file and writes it to device memory at the given address. A progress bar is shown during transfer. Commonly used to load firmware into memory before executing it with `exec`.

:::note
The memory address starts from 0, **NOT** the SDRAM physical address.
:::

## Example

Write `xstar.bin` to memory address `0x60000000`:

```shell
xrock write 0x60000000 xstar.bin
```
