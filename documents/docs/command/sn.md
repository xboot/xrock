# sn

读取或写入设备序列号（Serial Number）。

## 用法

### 读取序列号

```shell
xrock sn
```

### 写入序列号

```shell
xrock sn <string>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<string>` | 待写入的序列号字符串 |

## 说明

序列号存储在厂商存储（Vendor Storage）区域。该命令需要 loader 支持 `Vendor Storage` 或 `New Vendor Storage` 能力。

## 示例

读取序列号：

```shell
xrock sn
```

输出示例：

```
SN: XK12345678
```

若无序列号：

```
No serial number
```

写入序列号：

```shell
xrock sn XK12345678
```

若 loader 不支持：

```
The loader don't support vendor storage
```
