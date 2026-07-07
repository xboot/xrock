# otp

Dump OTP (One-Time-Programmable) memory in hexadecimal format.

## Usage

```shell
xrock otp <length>
```

## Arguments

| Argument | Description |
|----------|-------------|
| `<length>` | Number of bytes to read |

## Description

Reads the chip's OTP area and prints it in hex. This command requires the loader to support the `Read OTP Chip` capability, which can be confirmed with `xrock capability`.

## Example

Read 32 bytes of OTP:

```shell
xrock otp 32
```

If the loader does not support it:

```
The loader don't support dump otp
```
