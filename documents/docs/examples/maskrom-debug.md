# Maskrom 模式下的内存调试

本示例演示如何在 maskrom 模式下，通过调试 UART 转储内存、写入固件并执行代码。

## 场景

在 maskrom 模式下，部分内存区域（如 BootROM）无法通过常规 loader 命令访问。XROCK 提供 `extra maskrom-dump-*` 命令，通过上传一段利用调试 UART 输出内存数据的小程序来实现转储。

## 示例：RV1106 转储内存

```shell
# 仅初始化 DDR（可选，转储 BootROM 通常不需要）
xrock extra maskrom --rc4 off --sram rv1106_ddr_924MHz_v1.15.bin --delay 10

# 通过调试 UART 转储 0xffff0000 处的 1024 字节
xrock extra maskrom-dump-arm32 --rc4 off --uart 0xff4c0000 0xffff0000 1024
```

`--uart` 参数为调试 UART 的寄存器基地址，因芯片而异：

| 芯片 | 调试 UART 基地址 | 架构 |
|------|------------------|------|
| RV1106 | `0xff4c0000` | arm32 |
| RK1808 | `0xff550000` | arm64 |
| RK3128 | `0xff1a0000` | arm32 |
| RK3288 | `0xff690000` | arm32 |
| RK3399 | `0xff1a0000` | arm64 |
| PX30 | `0xff160000` | arm64 |
| RK3576 | `0x2ad40000` | arm64 |
| RK3506 | `0xff0a0000` | arm32 |

## 示例：写入固件并执行

```shell
# 初始化 DDR
xrock extra maskrom --rc4 off --sram rv1106_ddr_924MHz_v1.15.bin --delay 10

# 写入固件到内存
xrock extra maskrom-write-arm32 --rc4 off 0x00000000 xstar.bin

# 跳转执行
xrock extra maskrom-exec-arm32 --rc4 off 0x00000000
```

:::note
内存地址从 0 开始，**不是** SDRAM 的物理地址。`arm32` / `arm64` 需按芯片架构选择。
:::
