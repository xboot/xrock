# reset

Reset the chip, optionally to maskrom mode.

## Usage

```shell
xrock reset [maskrom]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `maskrom` | Optional, reset into maskrom mode; if omitted, reset to normal mode |

## Description

Resets the chip. Without an argument it resets to normal boot mode; with `maskrom` it resets and forces maskrom mode for re-downloading.

## Examples

```shell
# Reset to normal mode
xrock reset

# Reset to maskrom mode
xrock reset maskrom
```
