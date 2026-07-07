# 快速开始

本节通过一个简单示例，帮助你在几分钟内上手 XROCK。

## 前置准备

1. 已按[编译安装](./build-guide)完成 XROCK 的编译；
2. 一块瑞芯微 SOC 开发板；
3. 一根 USB 数据线（连接开发板 USB 口与主机）；
4. 对应芯片的 DDR 与 usbplug 二进制（可从 [rkbin](https://github.com/rockchip-linux/rkbin) 获取）。

## 第一步：让设备进入 Maskrom 模式

按住板载 Maskrom 按键（或 Vol+，因板而异），上电或复位，通过 USB 连接到主机。

## 第二步：初始化芯片并进入 Loader 模式

以 RV1106 为例（需 `--rc4-off`）：

```shell
xrock maskrom rv1106_ddr_924MHz_v1.15.bin rv1106_usbplug_v1.09.bin --rc4-off
```

执行成功后，芯片完成 DDR 初始化并进入 loader 模式。

## 第三步：查看芯片版本

```shell
xrock version
```

输出示例：

```
RV1106(5618): 0x31355631 0x34312e31 0x00000000 0x00000000
```

## 第四步：操作 Flash

```shell
xrock flash                                # 探测 Flash 并显示信息
xrock flash erase 0 0x2000                 # 擦除起始扇区开始的 0x2000 个扇区
xrock flash write 0x40 loader.bin          # 将 loader 写入 0x40 扇区
```

## 更多操作

- [命令参考](../command/maskrom)：了解所有可用命令；
- [使用示例](../examples/maskrom-init)：查看典型场景的完整流程；
- [芯片参考列表](../reference/chip-list)：查看各芯片的初始化配置。
