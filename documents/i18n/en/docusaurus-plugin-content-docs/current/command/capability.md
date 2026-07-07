# capability

Show the loader capability information.

## Usage

```shell
xrock capability
```

## Description

Reads the 8-byte capability bitmap returned by the loader and parses whether each capability is enabled: Direct LBA, Vendor Storage, First 4M Access, Read LBA, New Vendor Storage, Read Com Log, Read IDB Config, Read Secure Mode, New IDB, Switch Storage, LBA Parity, Read OTP Chip, Switch USB3.

See the [Capability Flags Reference](../reference/capability-flags) for the meaning of each flag.

## Example

```shell
xrock capability
```

Sample output:

```
Capability: 1f 02 00 00 00 00 00 00
    Direct LBA: enabled
    Vendor Storage: enabled
    First 4M Access: enabled
    Read LBA: enabled
    New Vendor Storage: disabled
    ...
```
