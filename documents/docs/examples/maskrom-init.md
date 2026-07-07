# 初始化芯片并进入 Loader 模式

本示例演示如何使用 XROCK 在 maskrom 模式下初始化芯片，使其进入 loader 模式。

## 前置条件

- 设备已进入 maskrom 模式；
- 已准备好对应芯片的 DDR 与 usbplug 二进制（可从 [rkbin](https://github.com/rockchip-linux/rkbin) 获取）。

## 方式一：使用 maskrom 命令（推荐）

以 RV1106 为例（需 `--rc4-off`）：

```shell
xrock maskrom rv1106_ddr_924MHz_v1.15.bin rv1106_usbplug_v1.09.bin --rc4-off
xrock version
```

以 RK3399 为例（RC4 默认开启）：

```shell
xrock maskrom rk3399_ddr_800MHz_v1.30.bin rk3399_usbplug_v1.30.bin
xrock version
```

## 方式二：使用 extra maskrom 命令（精细控制）

可分别控制 SRAM / DRAM 上传的 RC4 开关与延迟：

```shell
xrock extra maskrom --rc4 off \
  --sram rv1106_ddr_924MHz_v1.15.bin --delay 10 \
  --rc4 off --dram rv1106_usbplug_v1.09.bin --delay 10
xrock version
```

## 方式三：仅初始化 DDR

如只需初始化 DDR 内存（不进入 loader 模式）：

```shell
xrock extra maskrom --rc4 off --sram rv1106_ddr_924MHz_v1.15.bin --delay 10
```

## 验证

执行 `xrock version`，若输出芯片名称与版本号，说明已成功进入 loader 模式。

各芯片的 DDR / usbplug 文件名与 RC4 配置见[芯片参考列表](../reference/chip-list)。
