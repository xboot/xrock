# vs

厂商存储（Vendor Storage）读写命令，支持转储、读取与写入。

## 用法

### 转储

```shell
xrock vs dump <index> <length> [type]
```

### 读取

```shell
xrock vs read <index> <length> <file> [type]
```

### 写入

```shell
xrock vs write <index> <file> [type]
```

## 参数

| 参数 | 说明 |
|------|------|
| `<index>` | 厂商存储项索引 |
| `<length>` | 读 / 转储的字节数（最大 512） |
| `<file>` | 读取保存或待写入的文件路径 |
| `[type]` | 可选，存储类型（0 表示旧版 Vendor Storage） |

## 说明

读写厂商存储区域，用于存放序列号、MAC 地址等自定义数据。该命令需要 loader 支持 `Vendor Storage` 或 `New Vendor Storage` 能力。单次读写最大 512 字节。

## 示例

转储索引 0 处的 512 字节：

```shell
xrock vs dump 0 512
```

读取索引 1 处的数据到文件：

```shell
xrock vs read 1 512 vs.bin
```

将文件写入索引 0：

```shell
xrock vs write 0 vs.bin
```

若 loader 不支持：

```
The loader don't support vendor storage
```
