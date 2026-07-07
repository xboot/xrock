# reset

复位芯片，可选择复位到 maskrom 模式或正常模式。

## 用法

```shell
xrock reset [maskrom]
```

## 参数

| 参数 | 说明 |
|------|------|
| `maskrom` | 可选，指定复位后进入 maskrom 模式；省略则复位到正常模式 |

## 说明

复位芯片。不带参数时复位到正常启动模式；带 `maskrom` 参数时复位并强制进入 maskrom 模式，便于重新下载。

## 示例

```shell
# 复位到正常模式
xrock reset

# 复位到 maskrom 模式
xrock reset maskrom
```
