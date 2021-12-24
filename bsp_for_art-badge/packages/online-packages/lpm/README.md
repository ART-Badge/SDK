# LPM

逻辑分区管理(Logical partition management)，支持动态创建、删除、查找、读写物理存储设备上的逻辑分区。

## LPM 使用说明
### LPM 初始化

初始化 API 是一定要执行的，确保 LPM 初始化后能正确的管理 block 设备。

#### 1. LPM 初始化
```lpm_init(void);``` 

上电初始化首先要执行的函数，用于初始化 **lpm** 链表

#### 2. LPM 管理 BLOCK 设备
```int lpm_dev_blk_append(struct rt_device *dev)``` 

把 **block_dev** 交由 **lpm** 管理

#### 3. LPM 查找被管理的 BLOCK 设备
```struct lpm_dev *lpm_dev_find(const char *dev_name)```

可以查找到 **block_dev** 是否被 **lpm** 管理

### LPM API

LPM 组件提供了API 可供用户动态调用

#### 1. 查找分区

```struct lpm_partition *lpm_partition_find(const char *dev_name, const char *name)``` 

查找当前 block 设备上的 分区信息

| 参数     | 描述                                                |
| -------- | --------------------------------------------------- |
| dev_name | block 设备的名字，可以通过 list_device 查看到的名字 |
| name     | 分区的名字                                          |
| return   | 返回分区的指针                                      |

#### 2. 创建分区

```struct lpm_partition *lpm_partition_create(const char *dev_name, const char *name, uint32_t size)``` 

在当前 block 设备上创建一个分区

| 参数     | 描述                                                |
| -------- | --------------------------------------------------- |
| dev_name | block 设备的名字，可以通过 list_device 查看到的名字 |
| name     | 分区的名字                                          |
| return   | 返回分区的指针                                      |

#### 3. 删除分区

```int lpm_partition_delete(const char *dev_name, const char *name)``` 

删除当前 block 设备上的分区

| 参数     | 描述                                                |
| -------- | --------------------------------------------------- |
| dev_name | block 设备的名字，可以通过 list_device 查看到的名字 |
| name     | 分区的名字                                          |
| return   | 返回分区的指针                                      |

#### 4. 删除所有分区

```int lpm_partition_delete_all(const char *dev_name)``` 

删除当前 block 设备上的所有分区

| 参数     | 描述                                                |
| -------- | --------------------------------------------------- |
| dev_name | block 设备的名字，可以通过 list_device 查看到的名字 |
| return   | 返回分区的指针                                      |

#### 5. 创建 block 设备

```struct rt_device *lpm_blk_device_create(const char *dev_name, const char *parition_name)```

在 lpm 分区上创建一个 block 设备，新的 block 设备名是就是传递进去的分区名

| 参数     | 描述                                                |
| -------- | --------------------------------------------------- |
| dev_name | block 设备的名字，可以通过 list_device 查看到的名字 |
| name     | 分区的名字                                          |
| return   | 返回新的 block 设备的指针                           |

#### 6. 分区读数据

```int lpm_partition_read(const struct lpm_partition *part, uint32_t sector_no, uint8_t *buf, size_t size)``` 

对分区进行读操作。

| 参数      | 描述                 |
| --------- | -------------------- |
| part      | 分区的名字           |
| sector_no | 开始读的 sector      |
| buf       | 读数据的缓冲区       |
| size      | 读数据的长度         |
| return    | 返回读取到数据的长度 |

#### 7. 分区写数据

```int lpm_partition_write(const struct lpm_partition *part, uint32_t sector_no, const uint8_t *buf, size_t size)``` 

对分区进行写操作。

| 参数      | 描述             |
| --------- | ---------------- |
| part      | 分区的名字       |
| sector_no | 开始读的 sector  |
| buf       | 写数据的缓冲区   |
| size      | 写数据的长度     |
| return    | 返回写数据的长度 |

#### 8. 分区擦除

```int lpm_partition_erase(const struct lpm_partition *part, uint32_t block_no, size_t num)``` 

擦除分区。

| 参数     | 描述              |
| -------- | ----------------- |
| part     | 分区的名字        |
| block_no | 开始擦的 block    |
| num      | 擦除的 block 数量 |
| return   | 返回结果          |

#### 9. 分区全部擦除

```int lpm_partition_erase_all(const struct lpm_partition *part)``` 

擦除分区上的所有数据。

| 参数     | 描述              |
| -------- | ----------------- |
| part     | 分区的名字        |
| return   | 返回结果          |

#### 10. 打印分区信息

```int lpm_dump_partition(void)``` 

打印所有的分区信息。

```
==================== LPM partition table ============================
|  dev_name  |  par_name  |  block_size  |   offset   |   length   |
---------------------------------------------------------------------
|  norflash  |  app       |  0x00001000  | 0x00000003 | 0x00000004 |
|  norflash  |  abb       |  0x00001000  | 0x00000008 | 0x00000004 |
=====================================================================
```

### LPM 命令行

#### 1. 探测分区信息
```lpmm probe norflash app ``` 

探测 **norflash** block 设备上是否存在 **app** 这个分区

```
msh />lpmm probe norflash app
find partition name is app
Probed a block device | app | offset: 3 | size: 4 |
```

#### 2. 读取分区信息
```lpmm read norflash app 0 1``` 

读取 **norflash** 设备上，第 0 个 sector 开始，1 个 sector 长度的数据

```
Read data success. Start sector_no from 0x00000000, size is 4096. The data is:
Offset (h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
[00000000] 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ................
[00000010] 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F ................
[00000020] 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F  !"#$%&'()*+,-./
[00000030] 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 0123456789:;<=>?
```

#### 3. 在分区上写数据

```lpmm write norflash app 0 1``` 

写 **norflash** 设备上，第 0 个 sector 开始，1 个 sector 长度的数据，block 设备每次读写都是一个 block , 目前不支持用户写自定义数据。

```
msh />lpmm write norflash app 0 1
find partition name is app
LPM Write Success at the Device name (norflash)  the partition name (app) .
```

#### 4. 创建一个新的分区

```lpmm create norflash cpp 10``` 

在**norflash** 设备上，创建一个分区**cpp**，大小为 **10** 个 block

```
msh />lpmm create norflash cpp 10
Create block device | cpp | offset: 3 | size: 10 |.
```

#### 5. 删除一个分区

```lpmm delete norflash app``` 

删除 **norflash** 设备上的分区 **app**

```
msh />lpmm delete norflash app
delete lpm_dev name is norflash  partition name is app
Delete norflash Partition app Success.
```

#### 6. 删除所有分区

```lpmm delete norflash all``` 

删除 **norflash** 设备上的所有分区

```
msh />lpmm delete norflash all
delete lpm_dev name is norflash  partition name is cpp
delete lpm_dev name is norflash  partition name is abb
delete lpm_dev name is norflash  partition name is app

```

#### 7. 显示分区信息

```lpmm dump norflash part``` 

打印 **norflash** 的分区信息

```
msh />lpmm dump norflash part
==================== LPM partition table ============================
|  dev_name  |  par_name  |  block_size  |   offset   |   length   |
---------------------------------------------------------------------
|  norflash  |  cpp       |  0x00001000  | 0x00000003 | 0x0000000a |
|  norflash  |  abb       |  0x00001000  | 0x00000008 | 0x00000004 |
|  norflash  |  app       |  0x00001000  | 0x00000003 | 0x0000000a |
=====================================================================
```

#### 8. 显示 BLOCK 设备

```lpmm dump norflash dev``` 

打印被 **lpm** 管理的 block 设备

```
msh />lpmm dump norflash dev
==================== LPM Device table =======================
|  dev_name  | bytes_per_sector | sector_count | block_size |
-------------------------------------------------------------
|  norflash  |    0x00001000    |  0x00001000  | 0x00001000 |
=============================================================
```

