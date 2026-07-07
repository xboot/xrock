# maskrom

在 maskrom 模式下使用 DDR 与 usbplug 初始化芯片，使其进入 loader 模式。

## 用法

```shell
xrock maskrom <ddr> <usbplug> [--rc4-off]
```

## 参数

| 参数 | 说明 |
|------|------|
| `<ddr>` | DDR 初始化二进制文件路径（上传到 SRAM，代码 0x471） |
| `<usbplug>` | usbplug 二进制文件路径（上传到 DRAM，代码 0x472） |
| `--rc4-off` | 可选，关闭 RC4 加密传输（部分新芯片需要） |

## 说明

该命令仅在 maskrom 模式下可用。它依次将 DDR 初始化代码上传至 SRAM、usbplug 上传至 DRAM，usbplug 运行后芯片即进入 loader 模式，之后才能执行 `version`、`flash` 等 loader 命令。

是否需要 `--rc4-off` 取决于芯片型号，详见[芯片参考列表](../reference/chip-list)。

## 示例

```shell
# RV1106（需关闭 RC4）
xrock maskrom rv1106_ddr_924MHz_v1.15.bin rv1106_usbplug_v1.09.bin --rc4-off

# RK3399（RC4 默认开启）
xrock maskrom rk3399_ddr_800MHz_v1.30.bin rk3399_usbplug_v1.30.bin
```

若设备不在 maskrom 模式，将提示：

```
ERROR: The chip '<芯片名称>' does not in maskrom mode
```
