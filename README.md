# ART-Badge SDK 源码

#### 注意事项

- 该 SDK工程在使用之前，需要更新一下最新系统固件：`all_bin_file\all-bin-20201217\all.bin.output\ImgPacketFile_Patch_0.0.110.13_App_1.1.1.2_WithUserData-61893c89a52064ee5efca94d84e82a35.bin`，方可正常下载和调试。固件只需更新一次即可，之后便可以随意使用该 SDK工程进行下载调试等二次开发工作。

- 请勿使用 env 工具进行 `mennuconfig` 配置工程，相关功能删减可以直接修改 `rtconfig.h` 文件，修改后可 `scons --target=mdk5` 重构工程。

#### 固件更新方法

- 见 [恢复出厂固件方法](https://art-badge.github.io/website/#/docs/%E6%81%A2%E5%A4%8D%E5%87%BA%E5%8E%82%E5%9B%BA%E4%BB%B6%E6%96%B9%E6%B3%95)（点击跳转）。

- 目前支持使用该 SDK 工程进行下载调试等二次开发工作的固件只有最新版本的固件 `all_bin_file\all-bin-20201217.zip`，若更新的是另外两个固件 `all-bin-20201231.zip` 和 `all-bin-20201217.zip` ，则不支持使用该 SDK 源码进行二次开发，切记。 

