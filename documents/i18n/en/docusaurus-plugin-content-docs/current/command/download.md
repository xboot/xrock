# download

Initialize the chip in maskrom mode using a packaged loader file.

## Usage

```shell
xrock download <loader>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<loader>` | Path of a packaged loader file (rkloader format, with multiple entries) |

## Description

This command is only available in maskrom mode. Unlike `maskrom`, `download` takes a packaged loader file (composed of multiple 471/472 entries). XROCK uploads the entries in order, honoring the RC4 flag and delays specified inside the file.

Suitable for initializing the chip with a unified loader image (e.g. `update.bin`).

## Example

```shell
xrock download update.bin
```

If the file is not a valid loader:

```
ERROR: Not a valid loader 'update.bin'
```
