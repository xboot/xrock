# otp

以十六进制格式转储 OTP（One-Time-Programmable）内存。

## 用法

```shell
xrock otp <length>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<length>` | 读取的字节数 |

## 说明

读取芯片的 OTP 区域并以十六进制格式输出。该命令需要 loader 支持 `Read OTP Chip` 能力，可通过 `xrock capability` 确认。

## 示例

读取 32 字节 OTP：

```shell
xrock otp 32
```

若 loader 不支持：

```
The loader don't support dump otp
```
