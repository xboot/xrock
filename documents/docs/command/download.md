# download

在 maskrom 模式下使用打包好的 loader 文件初始化芯片。

## 用法

```shell
xrock download <loader>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<loader>` | 打包好的 loader 文件路径（rkloader 格式，含多个下载条目） |

## 说明

该命令仅在 maskrom 模式下可用。与 `maskrom` 命令不同，`download` 接收的是一个已打包的 loader 文件（由多个 471/472 条目组成），XROCK 会按文件中的条目顺序依次上传，并遵循文件内指定的 RC4 开关与延迟。

适用于使用统一 loader 镜像（如 `update.bin`）初始化芯片的场景。

## 示例

```shell
xrock download update.bin
```

若文件不是有效的 loader：

```
ERROR: Not a valid loader 'update.bin'
```
