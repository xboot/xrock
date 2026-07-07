# storage

Query or switch the current storage media.

## Usage

### List storage media

```shell
xrock storage
```

### Switch storage media

```shell
xrock storage <index>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<index>` | Storage media index (see table below) |

## Storage media index

| Index | Type |
|-------|------|
| 0 | UNKNOWN |
| 1 | FLASH |
| 2 | EMMC |
| 3 | SD |
| 4 | SD1 |
| 5 | SPINOR |
| 6 | SPINAND |
| 7 | RAM |
| 8 | USB |
| 9 | SATA |
| 10 | PCIE |

## Description

Without an argument, lists all storage media and marks the currently selected one (`-->`). With an index, switches to the specified media and lists again. Switching requires the loader to support the `Switch Storage` capability.

## Examples

Query the current storage media:

```shell
xrock storage
```

Sample output:

```
--> 1.FLASH
   2.EMMC
   3.SD
   ...
```

Switch to EMMC:

```shell
xrock storage 2
```

If the loader does not support switching:

```
The loader don't support switch storage
```
