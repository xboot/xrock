# capability

显示 loader 的能力（capability）信息。

## 用法

```shell
xrock capability
```

## 说明

读取 loader 返回的 8 字节能力位图，并解析各项能力是否启用，包括：Direct LBA、Vendor Storage、First 4M Access、Read LBA、New Vendor Storage、Read Com Log、Read IDB Config、Read Secure Mode、New IDB、Switch Storage、LBA Parity、Read OTP Chip、Switch USB3。

各项能力的含义见[能力标志参考](../reference/capability-flags)。

## 示例

```shell
xrock capability
```

输出示例：

```
Capability: 1f 02 00 00 00 00 00 00
    Direct LBA: enabled
    Vendor Storage: enabled
    First 4M Access: enabled
    Read LBA: enabled
    New Vendor Storage: disabled
    ...
```
