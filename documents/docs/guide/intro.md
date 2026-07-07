# 关于 XROCK

XROCK 是一款面向瑞芯微（Rockchip）SOC 的底层工具，支持 maskrom 与 loader 两种模式，可通过 USB 完成芯片初始化、内存读写、Flash 烧录、OTP / SN 读写等操作。

## 工作模式

XROCK 支持两种设备工作模式：

- **Maskrom 模式**：芯片上电时 BootROM 的 USB 下载模式。此模式下需先上传 DDR 初始化代码与 usbplug，将芯片带入 loader 模式后才能执行后续命令。`maskrom`、`download` 及 `extra maskrom*` 命令仅在此模式可用。
- **Loader 模式**：由 usbplug / loader 运行后进入的模式，提供版本查询、内存读写、Flash 操作、OTP / SN / 厂商存储等丰富命令。

:::note
在执行 loader 模式命令前，必须先通过 `maskrom` 或 `download` 命令初始化芯片并进入 loader 模式。
:::

## 特性

- 跨平台支持：Linux、Windows、macOS；
- 基于 libusb-1.0，无需内核驱动；
- Maskrom 模式：上传 DDR / usbplug、加载 loader、调试 UART 转储内存、内存读写与执行；
- Loader 模式：版本 / 能力查询、复位、内存读写、执行；
- OTP 读取、序列号（SN）读写、厂商存储（Vendor Storage）读写；
- 存储介质查询与切换；
- Flash 探测、擦除、读、写（支持多种存储介质）。

## 进入 Maskrom 模式

一般方法：按住板载 Maskrom / Vol+ 按键（因板而异），上电或复位，主机通过 USB 识别到瑞芯微设备（USB Vendor ID `0x2207`）。

## 相关链接

- 源码仓库：[https://github.com/xboot/xrock](https://github.com/xboot/xrock)
- 瑞芯微 loader 二进制：[https://github.com/rockchip-linux/rkbin](https://github.com/rockchip-linux/rkbin)
- 官方 rkdeveloptool：[https://github.com/rockchip-linux/rkdeveloptool](https://github.com/rockchip-linux/rkdeveloptool)
