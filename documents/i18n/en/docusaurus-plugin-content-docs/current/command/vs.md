# vs

Vendor Storage read/write commands: dump, read and write.

## Usage

### Dump

```shell
xrock vs dump <index> <length> [type]
```

### Read

```shell
xrock vs read <index> <length> <file> [type]
```

### Write

```shell
xrock vs write <index> <file> [type]
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<index>` | Vendor storage item index |
| `<length>` | Number of bytes to read / dump (max 512) |
| `<file>` | File to save read data, or file to write |
| `[type]` | Optional, storage type (0 means legacy Vendor Storage) |

## Description

Reads/writes the vendor storage area, used to store custom data such as serial numbers and MAC addresses. This command requires the loader to support the `Vendor Storage` or `New Vendor Storage` capability. A single read/write is limited to 512 bytes.

## Examples

Dump 512 bytes at index 0:

```shell
xrock vs dump 0 512
```

Read data at index 1 to a file:

```shell
xrock vs read 1 512 vs.bin
```

Write a file to index 0:

```shell
xrock vs write 0 vs.bin
```

If the loader does not support it:

```
The loader don't support vendor storage
```
