# flash

Flash 操作命令，支持探测、擦除、读取与写入。

## 用法

### 探测 Flash

```shell
xrock flash
```

### 擦除

```shell
xrock flash erase <sector> <count>
```

### 读取

```shell
xrock flash read <sector> <count> <file>
```

### 写入

```shell
xrock flash write <sector> <file>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<sector>` | 起始扇区号（扇区大小 512 字节） |
| `<count>` | 读 / 擦除的扇区数（0 表示到末尾） |
| `<file>` | 读取保存或待写入的文件路径 |

## 说明

该组命令操作当前选中的存储介质上的 Flash。操作前需先执行 `xrock flash` 探测 Flash 信息（容量、扇区数、厂商等）。

- **erase**：擦除指定扇区区域；
- **read**：将 Flash 数据读取到本地文件；
- **write**：将本地文件写入 Flash（自动处理到末尾的边界）。

:::note
部分 u-boot rockusb 驱动限制 Flash 转储起始位置在 32MB 内，可参考 README 中的 `RKUSB_READ_LIMIT_ADDR` 补丁。
:::

## 示例

探测 Flash：

```shell
xrock flash
```

输出示例：

```
Flash info:
    Manufacturer: Samsung (1)
    Capacity: 8192MB
    Sector size: 512
    Sector count: 16777216
    Block size: 128KB
    Page size: 2KB
    ...
```

擦除并写入 loader：

```shell
xrock flash erase 0x40 0x2000
xrock flash write 0x40 loader.bin
```

读取 Flash 内容到文件：

```shell
xrock flash read 0x0 0x1000 flash_dump.bin
```
