# sn

Read or write the device serial number.

## Usage

### Read serial number

```shell
xrock sn
```

### Write serial number

```shell
xrock sn <string>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<string>` | Serial number string to write |

## Description

The serial number is stored in the vendor storage area. This command requires the loader to support the `Vendor Storage` or `New Vendor Storage` capability.

## Examples

Read the serial number:

```shell
xrock sn
```

Sample output:

```
SN: XK12345678
```

When there is no serial number:

```
No serial number
```

Write a serial number:

```shell
xrock sn XK12345678
```

If the loader does not support it:

```
The loader don't support vendor storage
```
