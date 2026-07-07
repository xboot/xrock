# ready

查询芯片是否处于就绪（ready）状态。

## 用法

```shell
xrock ready
```

## 说明

该命令用于确认 loader 模式下的设备是否已就绪可接受后续命令。

## 示例

```shell
xrock ready
```

就绪时输出：

```
The chip is ready
```

否则：

```
Failed to show chip ready status
```
