# Capability Flags Reference

The 8-byte capability bitmap returned by the `capability` command. The meaning of each flag bit is as follows.

## Byte 0

| Bit | Flag | Meaning |
|-----|------|---------|
| 0 | Direct LBA | Direct LBA read/write supported |
| 1 | Vendor Storage | Vendor storage read/write supported |
| 2 | First 4M Access | Access to the first 4MB supported |
| 3 | Read LBA | LBA read supported |
| 4 | New Vendor Storage | New vendor storage supported |
| 5 | Read Com Log | Read communication log supported |
| 6 | Read IDB Config | Read IDB config supported |
| 7 | Read Secure Mode | Read secure mode supported |

## Byte 1

| Bit | Flag | Meaning |
|-----|------|---------|
| 0 | New IDB | New IDB |
| 1 | Switch Storage | Storage media switching supported |
| 2 | LBA Parity | LBA parity |
| 3 | Read OTP Chip | OTP read supported |
| 4 | Switch USB3 | USB3 switching supported |

## Command dependencies

Some commands depend on specific capabilities:

| Command | Required capability |
|---------|---------------------|
| `otp` | Read OTP Chip |
| `sn` | Vendor Storage or New Vendor Storage |
| `vs` | Vendor Storage or New Vendor Storage |
| `storage <index>` (switch) | Switch Storage |

Use `xrock capability` to view the capabilities supported by the current loader.
