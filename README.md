# ART-Badge SDK 源码

#### 一、ART-Badge 专属网站

ART-Badge 电子胸牌应用开发资料请查看 ART-Badge 专属网站：https://art-badge.github.io/website/#/

#### 二、注意事项

- 该 SDK工程在使用之前，需要更新一下最新系统固件：`all_bin_file\all-bin-20201217\all.bin.output\ImgPacketFile_Patch_0.0.110.13_App_1.1.1.2_WithUserData-61893c89a52064ee5efca94d84e82a35.bin`，方可正常下载和调试。固件只需更新一次即可，之后便可以随意使用该 SDK工程进行下载调试等二次开发工作。

- 请勿使用 env 工具进行 `mennuconfig` 配置工程，相关功能删减可以直接修改 `rtconfig.h` 文件，修改后可 `scons --target=mdk5` 重构工程。

- 目前支持使用该 SDK 工程进行下载调试等二次开发工作的固件只有最新版本的固件 `all_bin_file\all-bin-20201217.zip`，若更新的是另外两个固件 `all-bin-20201231.zip` 和 `all-bin-20201217.zip` ，则不支持使用该 SDK 源码进行二次开发，切记。 

- 工程需要添加下载算法，请把 `flashlgo` 中的两个 `.FLM` 文件拷贝到 `C:\Keil_v5\ARM\Flash` 路径下即可（根据你自己 MDK 实际安装路径拷贝）。

#### 三、固件更新方法

- 见《 [恢复出厂固件方法](https://art-badge.github.io/website/#/docs/%E6%81%A2%E5%A4%8D%E5%87%BA%E5%8E%82%E5%9B%BA%E4%BB%B6%E6%96%B9%E6%B3%95)》（点击跳转）。

- 目前支持使用该 SDK 工程进行下载调试等二次开发工作的固件只有最新版本的固件 `all_bin_file\all-bin-20201217.zip`，若更新的是另外两个固件 `all-bin-20201231.zip` 和 `all-bin-20201217.zip` ，则不支持使用该 SDK 源码进行二次开发，切记。 

#### 四、串口输出系统日志信息

- 由于目前串口需要用于 udb 下载、安装、卸载 APP 等操作，串口 console 被 udb 占用，所以使用串口助手或者 Shell 终端工具来打开串口是无法看到 Finsh 有信息输出的。
- 需要正常使用 Finsh 输入输出，需要使用 udb tool，使用方法见《[APP 开发指南](https://art-badge.github.io/website/#/docs/APP_%E5%BC%80%E5%8F%91%E6%B5%81%E7%A8%8B)》一文中的“APP 卸载”章节，连接 udb shell，注意串口波特率是 1000000。

