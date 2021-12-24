# 日志管理器介绍

## 1、介绍

该软件包主要用于配置和管理系统中日志相关功能，实现功能如下：

- 支持 ulog 文件后端功能启动；
- 重定向系统 hardfault 和 assert 异常错误回调，添加更多系统异常相关日志输出，包括
  - 函数调用栈日志
  - 内核运行日志
  - 系统负荷监视器日志
  - 当前系统 IPC 状态、内存状态、JS 堆等日志信息
- 支持系统异常时日志输出到 Flash，并在重启后导出到文件功能；

### 1.1 许可证

本软件包遵循 Apache-2.0 许可，详见 `LICENSE` 文件。 

### 1.2 依赖

- [ulog_file 软件包](https://github.com/RT-Thread-packages/ulog_file.git)
- [flashdb 软件包](https://github.com/armink-rtt-pkgs/FlashDB.git)
- [cmbacktrace 软件包](https://github.com/armink-rtt-pkgs/CmBacktrace.git)（可配置）
- [kdb 软件包](http://packages.rt-thread.org/detail.html?package=kdb)（可配置）
- [sys_load_monitor 软件包](https://github.com/armink-rtt-pkgs/sys_load_monitor)（可配置）

## 2、如何打开

使用本软件包需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    tools packages --->
        [*] logmgr: A log management system for rt-thread.
        [ ]   Enable automatic initialization
        [ ]   Enable log file backend feature
        [*]   Enable log abort feature
        (/abort.log) abort file path
        (logmgr) flashdb fal part name
        (65536) falshdb data max size
        [*]     cmbacktrace log support
        [*]     system ipc log support
        [*]     kernel running log support
        [*]     system load monitor log support
        [*]     system memory log support
        [ ]   Enable samples
            Version (latest)  --->
```

- **Enable automatic initialization**：自动初始化功能支持
- **Enable log file backend feature** ：开启日志文件后端功能支持
- **Enable log abort feature**：开启异常日志打印功能支持
  - **abort file path**：定义异常日志存储文件位置
  - **flashdb fal part name**：定义异常日志保存使用 flashdb 分区名称
  - **falshdb data max size**：定义异常日志保存使用 flashdb 数据最大值
  - **cmbacktrace log support**：开启系统异常后 cmbacktrace 日志打印
  - **system ipc log support**：开启系统异常后 IPC 信息打印
  - **kernel running log support**：开启系统异常后系统运行日志打印
  - **system load monitor log support**：开启系统异常系统负荷监视器打印
  - **system memory log support**：开启系统异常后系统内存相关信息日志打印
- **Enable samples**：开启示例文件

## 3、使用说明

### 3.1 日志管理器初始化

```
int logmgr_init(void);
```

上述功能配置完成之后，需要在应用层调用 `logmgr_init()` 初始化函数，或者开启自动初始化，即可完成日志管理系统初始化。

### 3.1 日志管理器取消初始化

```
int logmgr_deinit(void);
```

需要在应用层调用 `logmgr_deinit()` 函数，可以注销日志管理初始化。

## 4、联系方式

- 维护：ChenYong
- 主页：<https://github.com/RT-Thread-packages/logmgr.git>



