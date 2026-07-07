# 能力标志参考

`capability` 命令返回的 8 字节能力位图，各标志位含义如下。

## 字节 0

| 位 | 标志 | 含义 |
|----|------|------|
| 0 | Direct LBA | 支持直接 LBA 读写 |
| 1 | Vendor Storage | 支持厂商存储读写 |
| 2 | First 4M Access | 支持访问前 4MB |
| 3 | Read LBA | 支持 LBA 读取 |
| 4 | New Vendor Storage | 支持新版厂商存储 |
| 5 | Read Com Log | 支持读取通信日志 |
| 6 | Read IDB Config | 支持读取 IDB 配置 |
| 7 | Read Secure Mode | 支持读取安全模式 |

## 字节 1

| 位 | 标志 | 含义 |
|----|------|------|
| 0 | New IDB | 新版 IDB |
| 1 | Switch Storage | 支持切换存储介质 |
| 2 | LBA Parity | LBA 校验 |
| 3 | Read OTP Chip | 支持 OTP 读取 |
| 4 | Switch USB3 | 支持切换 USB3 |

## 命令依赖

部分命令依赖特定能力：

| 命令 | 依赖能力 |
|------|----------|
| `otp` | Read OTP Chip |
| `sn` | Vendor Storage 或 New Vendor Storage |
| `vs` | Vendor Storage 或 New Vendor Storage |
| `storage <index>`（切换） | Switch Storage |

可通过 `xrock capability` 查看当前 loader 支持的能力。
