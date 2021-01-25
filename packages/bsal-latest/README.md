# BSAL

## 1.  BSAL 软件包介绍

BSAL （Bluetooth Stack Abstract Layer）软件包是由 RT-Thread 针对不同 蓝牙协议栈接口实现的，目前支持的 协议栈有：nimble，realtek等协议栈 。

### 1.1 背景介绍

Bluetooth Stack Abstract Layer 蓝牙协议栈抽象层。用于适配各家协议栈以及底层卡片。

主要在协议栈基础之上封装一层抽象层接口。

# 2. BSAL 使用说明

BSAL 配置如下图所示：

![img](./doc/figures/menuconfig.png)

选择需要生成的角色（默认选择Peripheral即可）：

- Peripheral role:       
- Central role
- Broadcaster role
- Observer role

选择不同的示例可以实现不同的APP功能：

- Battery service     单battery servcie 测试sample
- battery and blufi service  两个profile servic（blufi 配网， bas）测试sample

再选择对应的协议栈：

- nimble   （nimble 协议栈适配层）
- realtek  （realtek 协议栈适配层）
- empty_stack   （移植协议栈模板）

# 3. BSAL 依赖

BSAL 依赖对应的协议栈，比如nimble，需要先有nimble+RTT的sample工程，首先确保可以跑对应协议栈的原生的sample。再试着添加BSAL层跑sample

每个sample对应一个cmd：

- battery service： bsal_bas_app
- battery and blufi service： bsal_bas_blufi_app

输入对应的cmd即可

# 4. 测试验证工具

输入cmd bsal_bas_blufi_app 启动sample

手机下载nRF Connect 工具（apple使用lightblue）

[nRF Connect](https://github.com/NordicSemiconductor/Android-nRF-Connect/releases)

搜索广播看到如下界面

![image-20200618164034578](./doc/figures/adv.png)

点击connect连接上之后：

![image-20200618164126725](./doc/figures/srv_table.png)

可以看到battery service和0xffff配网blufi service

说明sample运行成功。