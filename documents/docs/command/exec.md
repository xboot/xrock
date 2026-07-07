# exec

调用并执行设备内存中指定地址的函数。

## 用法

```shell
xrock exec <address> [dtb]
```

## 参数

| 参数 | 说明 |
|------|------|
| `<address>` | 待执行代码的内存地址 |
| `[dtb]` | 可选，设备树（DTB）的内存地址 |

## 说明

使 CPU 跳转到指定地址开始执行，可附带传入 DTB 地址。该命令用于 loader 模式下的代码执行。

:::tip
在 maskrom 模式下执行代码，推荐使用功能更完善的 `extra maskrom-exec-arm32` / `extra maskrom-exec-arm64` 命令。
:::

## 示例

跳转到 `0x60000000` 执行，并传入 DTB 地址 `0x61f00000`：

```shell
xrock exec 0x60000000 0x61f00000
```
