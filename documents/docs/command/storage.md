# storage

查询或切换当前存储介质。

## 用法

### 查询存储介质列表

```shell
xrock storage
```

### 切换存储介质

```shell
xrock storage <index>
```

## 参数

| 参数 | 说明 |
|------|------|
| `<index>` | 存储介质索引（见下表） |

## 存储介质索引

| 索引 | 类型 |
|------|------|
| 0 | UNKNOWN |
| 1 | FLASH |
| 2 | EMMC |
| 3 | SD |
| 4 | SD1 |
| 5 | SPINOR |
| 6 | SPINAND |
| 7 | RAM |
| 8 | USB |
| 9 | SATA |
| 10 | PCIE |

## 说明

不带参数时列出所有存储介质并标记当前选中的介质（`-->`）。带索引参数时切换到指定介质并重新列出。切换介质需要 loader 支持 `Switch Storage` 能力。

## 示例

查询当前存储介质：

```shell
xrock storage
```

输出示例：

```
--> 1.FLASH
   2.EMMC
   3.SD
   ...
```

切换到 EMMC：

```shell
xrock storage 2
```

若 loader 不支持切换：

```
The loader don't support switch storage
```
