# 烧写 Loader 到 Flash

本示例演示如何在 loader 模式下将 loader 烧录到 Flash。

## 前置条件

- 设备已通过 `maskrom` 或 `download` 命令进入 loader 模式；
- 已准备好打包好的 loader 文件（rkloader 格式）。

## 使用 upgrade 命令（推荐）

`upgrade` 命令会自动读取 loader 的 IDB 并写入 Flash 起始扇区，写入位置根据存储介质类型自动调整：

```shell
xrock upgrade update.bin
```

## 手动烧写

如需手动控制烧写位置，可使用 `flash` 命令：

```shell
xrock flash                                # 探测 Flash，确认容量与扇区数
xrock flash erase 0x40 0x2000              # 擦除 0x40 扇区开始的 0x2000 个扇区
xrock flash write 0x40 loader.bin          # 将 loader 写入 0x40 扇区
```

:::note
- 不同存储介质的起始扇区不同：Flash / eMMC / SD 通常为 64，SPI NOR 为 128，SPI NAND 为 512。`upgrade` 命令会自动处理，手动烧写时需注意。
- 烧写前可用 `xrock storage` 确认当前存储介质。
:::

## 验证

烧写完成后复位芯片：

```shell
xrock reset
```

芯片应从新烧录的 loader 启动。
