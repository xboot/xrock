# 芯片参考列表

XROCK 支持的瑞芯微 SOC 及其 maskrom 初始化所需的 DDR / usbplug 文件与 RC4 配置。

## 支持芯片

| 芯片 | USB PID | 架构 | RC4 |
|------|---------|------|-----|
| RV1106 | 0x110c | arm32 | off |
| RK1808 | 0x180a | arm64 | on |
| RK3128 | 0x310c | arm32 | on |
| RK3288 | 0x320a | arm32 | on |
| RK3399 | 0x330c | arm64 | on |
| RK3399PRO | 0x330c | arm64 | on |
| PX30 | 0x330d | arm64 | on |
| RK3308 | 0x330e | arm32 | on |
| RK3566 | 0x350a | arm64 | off |
| RK3568 | 0x350a | arm64 | off |
| RK3588 | 0x350b | arm64 | off |
| RK3562 | 0x350d | arm64 | off |
| RK3576 | 0x350e | arm64 | off |
| RK3506 | 0x350f | arm32 | off |

:::note
完整 USB PID 列表见仓库根目录 `99-xrock.rules`。RK3566 与 RK3568 共享同一 PID（0x350a）。
:::

## 初始化配置参考

各芯片 maskrom 初始化命令示例（DDR / usbplug 文件名以实际版本为准，可从 [rkbin](https://github.com/rockchip-linux/rkbin) 获取）。

### RV1106（RC4 off）

```shell
xrock maskrom rv1106_ddr_924MHz_v1.15.bin rv1106_usbplug_v1.09.bin --rc4-off
```

### RK1808（RC4 on）

```shell
xrock maskrom rk1808_ddr_933MHz_v1.05.bin rk1808_usbplug_v1.05.bin
```

### RK3128（RC4 on）

```shell
xrock maskrom rk3128_ddr_300MHz_v2.12.bin rk3128_usbplug_v2.63.bin
```

### RK3288（RC4 on）

```shell
xrock maskrom rk3288_ddr_400MHz_v1.11.bin rk3288_usbplug_v2.63.bin
```

### RK3399（RC4 on）

```shell
xrock maskrom rk3399_ddr_800MHz_v1.30.bin rk3399_usbplug_v1.30.bin
```

### PX30（RC4 on）

```shell
xrock maskrom px30_ddr_333MHz_v1.16.bin px30_usbplug_v1.31.bin
```

### RK3308（RC4 on）

```shell
xrock maskrom rk3308_ddr_589MHz_uart2_m1_v1.31.bin rk3308_usbplug_v1.27.bin
```

### RK3566 / RK3568（RC4 off）

```shell
# RK3566
xrock maskrom rk3566_ddr_1056MHz_v1.11.bin rk356x_usbplug_v1.13.bin --rc4-off

# RK3568
xrock maskrom rk3568_ddr_1560MHz_v1.11.bin rk356x_usbplug_v1.13.bin --rc4-off
```

### RK3588（RC4 off）

```shell
xrock maskrom rk3588_ddr_lp4_2112MHz_lp5_2400MHz_v1.16.bin rk3588_usbplug_v1.11.bin --rc4-off
```

### RK3562（RC4 off）

```shell
xrock maskrom rk3562_ddr_1332MHz_v1.05.bin rk3562_usbplug_v1.04.bin --rc4-off
```

### RK3576（RC4 off）

```shell
xrock maskrom rk3576_ddr_lp4_2112MHz_lp5_2736MHz_v1.05.bin rk3576_usbplug_v1.02.bin --rc4-off
```

### RK3506（RC4 off）

```shell
xrock maskrom rk3506b_ddr_750MHz_v1.04.bin rk3506_usbplug_v1.02.bin --rc4-off
```
