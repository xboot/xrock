# flash

Flash commands: detect, erase, read and write.

## Usage

### Detect flash

```shell
xrock flash
```

### Erase

```shell
xrock flash erase <sector> <count>
```

### Read

```shell
xrock flash read <sector> <count> <file>
```

### Write

```shell
xrock flash write <sector> <file>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<sector>` | Start sector number (sector size 512 bytes) |
| `<count>` | Number of sectors to read / erase (0 means to the end) |
| `<file>` | File to save read data, or file to write |

## Description

These commands operate on the flash of the currently selected storage media. Run `xrock flash` first to detect the flash info (capacity, sector count, manufacturer, etc.).

- **erase**: erases the specified sector region;
- **read**: reads flash data to a local file;
- **write**: writes a local file to flash (handles the boundary to the end automatically).

:::note
Some u-boot rockusb drivers limit the flash dump start to within 32MB; see the `RKUSB_READ_LIMIT_ADDR` patch in the README.
:::

## Examples

Detect flash:

```shell
xrock flash
```

Sample output:

```
Flash info:
    Manufacturer: Samsung (1)
    Capacity: 8192MB
    Sector size: 512
    Sector count: 16777216
    Block size: 128KB
    Page size: 2KB
    ...
```

Erase and write a loader:

```shell
xrock flash erase 0x40 0x2000
xrock flash write 0x40 loader.bin
```

Read flash contents to a file:

```shell
xrock flash read 0x0 0x1000 flash_dump.bin
```
