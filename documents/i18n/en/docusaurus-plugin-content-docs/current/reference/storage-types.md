# Storage Media Types

Storage media types and indexes supported by the `storage` command.

| Index | Type | Description |
|-------|------|-------------|
| 0 | UNKNOWN | Unknown |
| 1 | FLASH | Generic flash |
| 2 | EMMC | eMMC |
| 3 | SD | SD card |
| 4 | SD1 | Second SD interface |
| 5 | SPINOR | SPI NOR flash |
| 6 | SPINAND | SPI NAND flash |
| 7 | RAM | RAM |
| 8 | USB | USB |
| 9 | SATA | SATA |
| 10 | PCIE | PCIe |

## upgrade write start sector

The `upgrade` command selects the IDB write start sector automatically based on the storage media type:

| Storage media | Start sector |
|---------------|--------------|
| FLASH / EMMC / SD / SD1 / RAM / USB / SATA / PCIE | 64 |
| SPINOR | 128 |
| SPINAND | 512 |

## Switching storage media

Switching requires the loader to support the `Switch Storage` capability:

```shell
xrock storage 2    # Switch to EMMC
```
