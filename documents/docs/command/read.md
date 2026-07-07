# read

从设备内存读取数据并保存到文件，读取过程带进度显示。

## 用法

```shell
xrock read <address> <length> <file>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<address>` | 内存起始地址 |
| `<length>` | 读取的字节数 |
| `<file>` | 保存数据的文件路径 |

## 说明

将指定长度的内存数据读取并写入本地文件，传输过程显示进度条。

:::note
内存地址从 0 开始，**不是** SDRAM 的物理地址。部分 u-boot rockusb 驱动限制 Flash 转储起始位置在 32MB 内，可参考 README 中的 `RKUSB_READ_LIMIT_ADDR` 补丁。
:::

## 示例

从地址 `0x60000000` 读取 1MB 数据到 `dump.bin`：

```shell
xrock read 0x60000000 0x100000 dump.bin
```
