# 存储介质类型

`storage` 命令支持的存储介质类型及索引。

| 索引 | 类型 | 说明 |
|------|------|------|
| 0 | UNKNOWN | 未知 |
| 1 | FLASH | 通用 Flash |
| 2 | EMMC | eMMC |
| 3 | SD | SD 卡 |
| 4 | SD1 | 第二个 SD 接口 |
| 5 | SPINOR | SPI NOR Flash |
| 6 | SPINAND | SPI NAND Flash |
| 7 | RAM | RAM |
| 8 | USB | USB |
| 9 | SATA | SATA |
| 10 | PCIE | PCIe |

## upgrade 写入起始扇区

`upgrade` 命令根据存储介质类型自动选择 IDB 写入的起始扇区：

| 存储介质 | 起始扇区 |
|----------|----------|
| FLASH / EMMC / SD / SD1 / RAM / USB / SATA / PCIE | 64 |
| SPINOR | 128 |
| SPINAND | 512 |

## 切换存储介质

切换需要 loader 支持 `Switch Storage` 能力：

```shell
xrock storage 2    # 切换到 EMMC
```
