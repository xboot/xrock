# version

Show the chip version information.

## Usage

```shell
xrock version
```

## Description

Reads the 16-byte version info and prints it as the chip name followed by four 32-bit hex groups. Commonly used to verify the connection and chip model.

## Example

```shell
xrock version
```

Sample output:

```
RK3399(9230): 0x56313030 0x322e3230 0x00000000 0x00000000
```
