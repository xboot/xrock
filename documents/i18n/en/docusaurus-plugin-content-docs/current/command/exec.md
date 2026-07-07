# exec

Call and execute a function at the given memory address.

## Usage

```shell
xrock exec <address> [dtb]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<address>` | Memory address of the code to execute |
| `[dtb]` | Optional, memory address of the device tree (DTB) |

## Description

Makes the CPU jump to the given address and start executing. A DTB address can optionally be passed in. This command is for code execution in loader mode.

:::tip
To execute code in maskrom mode, prefer the more capable `extra maskrom-exec-arm32` / `extra maskrom-exec-arm64` commands.
:::

## Example

Jump to `0x60000000` to execute, passing DTB address `0x61f00000`:

```shell
xrock exec 0x60000000 0x61f00000
```
