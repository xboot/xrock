# upgrade

在 loader 模式下将 loader 升级（烧录）到 Flash。

## 用法

```shell
xrock upgrade <loader>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<loader>` | 打包好的 loader 文件路径（rkloader 格式） |

## 说明

该命令读取 loader 文件中的 IDB（ID Block），并将其写入 Flash 的起始扇区（通常从第 64 扇区开始，SPI NOR 从 128 扇区、SPI NAND 从 512 扇区）。写入位置根据当前存储介质类型自动调整。

执行前需确保芯片已进入 loader 模式，并已选中正确的存储介质。

## 示例

```shell
xrock upgrade update.bin
```

若 Flash 探测失败：

```
Failed to detect flash
```
