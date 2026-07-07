# extra

执行 maskrom 模式下的扩展命令，提供更精细的下载与调试能力。

## 用法

### 上传 SRAM / DRAM 文件

```shell
xrock extra maskrom --rc4 <on|off> [--sram <file> --delay <ms>] [--dram <file> --delay <ms>] [...]
```

### 通过调试 UART 转储内存

```shell
xrock extra maskrom-dump-arm32 --rc4 <on|off> --uart <register> <address> <length>
xrock extra maskrom-dump-arm64 --rc4 <on|off> --uart <register> <address> <length>
```

### 写入内存

```shell
xrock extra maskrom-write-arm32 --rc4 <on|off> <address> <file>
xrock extra maskrom-write-arm64 --rc4 <on|off> <address> <file>
```

### 执行代码

```shell
xrock extra maskrom-exec-arm32 --rc4 <on|off> <address>
xrock extra maskrom-exec-arm64 --rc4 <on|off> <address>
```

## 参数

| 参数 | 说明 |
|------|------|
| `--rc4 <on\|off>` | 是否启用 RC4 加密传输 |
| `--sram <file>` | 上传文件到 SRAM（代码 0x471，DDR 初始化） |
| `--dram <file>` | 上传文件到 DRAM（代码 0x472，usbplug） |
| `--delay <ms>` | 上传后延迟的毫秒数 |
| `--uart <register>` | 调试 UART 寄存器基地址 |
| `<address>` | 内存地址 |
| `<length>` | 转储的字节数 |
| `<file>` | 待写入的文件路径 |

## 说明

`extra` 命令仅在 maskrom 模式下可用，提供比 `maskrom` 命令更灵活的控制：

- **maskrom**：可单独上传 SRAM / DRAM 文件，并分别设置 RC4 与延迟，支持多段上传；
- **maskrom-dump-arm32/arm64**：通过调试 UART 转储指定地址的内存（用于读取 BootROM 等无法直接访问的区域）；
- **maskrom-write-arm32/arm64**：将文件写入指定内存地址；
- **maskrom-exec-arm32/arm64**：跳转到指定地址执行代码。

`arm32` 与 `arm64` 对应目标 CPU 的执行模式，需按芯片架构选择。

## 示例

仅初始化 DDR（上传 SRAM）：

```shell
xrock extra maskrom --rc4 off --sram rv1106_ddr_924MHz_v1.15.bin --delay 10
```

完整初始化（SRAM + DRAM）：

```shell
xrock extra maskrom --rc4 off \
  --sram rv1106_ddr_924MHz_v1.15.bin --delay 10 \
  --rc4 off --dram rv1106_usbplug_v1.09.bin --delay 10
```

通过调试 UART 转储 BootROM：

```shell
xrock extra maskrom-dump-arm32 --rc4 off --uart 0xff4c0000 0xffff0000 1024
```

写入固件并执行：

```shell
xrock extra maskrom-write-arm32 --rc4 off 0x00000000 xstar.bin
xrock extra maskrom-exec-arm32 --rc4 off 0x00000000
```
