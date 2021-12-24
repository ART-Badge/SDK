# KDB使用方法及相关API文档

## 简介

Kernel Debug Tool 简称 KDB。是一种内核信息追踪工具。当我们查找程序漏洞时，需要查看系统运行的各种信息。如：线程创建时间，线程切换，内存的分配与回收，信号量，互斥量，邮箱，事件等IPC 变量的使用。通过这些内核活动信息来对程序进行分析，有效查找程序漏洞。

## 使用方法

本节将介绍KDB软件包的使用方法。

### 帮助信息

```
msh />kdb -h
kdb
kdb -h        des: print help
kdb -s        des: start take notes
kdb -d        des: dump take notes
kdb -ii       des: is inval
kdb --stop    des: stop takle notes
```
通过命令行调用kdb，了解kdb的使用信息。

### 开启kdb

```
msh />kdb -s
rtx start...
```
开启KDB工具，记录内核信息。

### 中止kdb

```
msh />kdb --stop
rtx stop.
```
将kdb工具中止，终止后工具不再记录内核信息。

### 打印kdb记录信息

```
msh />kdb -d
[tick]:2304 thread scheduler tidle0 -> tshell
[tick]:2304 thread resume tshell
[tick]:2274 thread scheduler tshell -> tidle0
[tick]:2274 thread suspend tshell
[tick]:2274 thread scheduler tidle0 -> tshell
[tick]:2274 thread resume tshell
[tick]:2264 thread scheduler tshell -> tidle0
[tick]:2264 thread suspend tshell
[tick]:2264 thread scheduler tidle0 -> tshell
[tick]:2264 thread resume tshell
...

```
将kdb工具的储存的信息打印出来


## KDB API

本节讲介绍kdb工具所提供的API

`int kdb_sys_init(void);`

> 用于初始化kdb工具。

`void kdb_sys_start(void);`

> 启动kdb并且开始记录内核活动信息。

`void kdb_sys_stop(void);`

> 中止记录内核活动信息。

`void kdb_sys_dump(void);`

> 将记录的内核活动信息打印出来。

`void kdb_sys_stop_dump(void);`

> 停止kdb工作，并将内核活动信息打印出来。

