# 开源图形库µGUI 

## 1 介绍

µGUI 是一个用于嵌入式系统的免费开源图形库。 它独立于平台，可以轻松移植到几乎任何微控制器系统。 只要显示器能够显示图形，μGUI 就不受某种显示技术的限制。 因此，支持LCD、TFT、E-Paper、LED或OLED等显示技术。 整个模块由两个文件组成：ugui.c 和 ugui.h。

### 1.1 许可证

详见 `license.txt` 文件。

### 1.2 依赖

- RT-Thread LCD设备框架

## 2 如何打开

使用本软件包需要在 RT-Thread 的包管理器中选择它，具体路径如下： 

```c
RT-Thread online packages
    multimedia packages  --->
        [*] uGUI:Open source graphics library ugui ported to rtthread. --->
```

## 3 使用说明

### 3.1 初始化

```c
int gui_port_init(void);
```

需要在应用层调用 `gui_port_init()` 初始化函数，即可初始化LCD设备，同时初始化uGUI库；

### 3.2 参考使用手册

通过阅读 µGUI 库自带的使用手册《µGUI v0.3.pdf》学会对 µGUI 库的使用；

## 4 联系方式

- 维护：xidongxu
- 主页：<https://github.com/xidongxu/ugui>