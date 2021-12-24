# 一款轻量级的系统负荷监视器


## what

本软件包是一款轻量级的系统负荷监视器，用户可以在必要的时候 dump 出来，系统的历史负荷情况，方便进行性能分析

## how

### STEP1 启用并配置软件包

参考 menuconfig 配置描述

### STEP2 始化软件包

软件包默认使用 RT-Thread 自带的系统初始化方式，进行自动初始化( `sys_load_monitor_init` )。如果系统已经开启了自动初始化功能，那么无需再调用初始化代码。

### STEP3 dump 历史负荷统计 

可以通过下面两种方式进行 dump:

- 调用 `sys_load_monitor_dump` 函数
- shell 中执行 `sys_load` 命令

执行后，近期的系统负荷将会被输出到控制台中，大致情况如下：

```shell
msh />sys_load                                                                                   
RUNNING  | 31      23      15      7      0 | ready thread list..                                
-------- | -------------------------------- | -------------------                                
thread-j |    * * *   * $                   | thread-k(20) thread-m(24) thread-n(26) thread-o(28)
thread-e |        * *             $         | thread-l(22) thread-m(24)                          
thread-h |    * * * * *     $               | thread-k(20) thread-l(22) thread-m(24) thread-n(26)
thread-b |    *     *   *               $   | thread-j(18) thread-l(22) thread-o(28)             
thread-d |    * * * * *           * $       | thread-e(08) utest   (20) thread-l(22) thread-m(24)
thread-f |    * * * * *     * * $           | thread-g(12) thread-h(14) utest   (20) thread-l(22)
thread-a |    * * * * * *                 $ | thread-j(18) thread-k(20) utest   (20) thread-l(22)
thread-b |    * * * * *                 $   | utest   (20) thread-l(22) thread-m(24) thread-n(26)
thread-g |    * * * * *       $             | utest   (20) thread-l(22) thread-m(24) thread-n(26)
thread-h |    * * * * * * * $               | thread-i(16) thread-j(18) utest   (20) thread-l(22)
thread-f |    * * * *     *     $           | thread-i(16) thread-l(22) thread-m(24) thread-n(26)
thread-i |    *           $                 | thread-o(28)                                       
thread-k |    * * * * $                     | thread-l(22) thread-m(24) thread-n(26) thread-o(28)
thread-b |      * * * * *               $   | thread-j(18) tshell  (20) thread-l(22) thread-m(24)
thread-e |    * * * * *           $         | thread-k(20) thread-l(22) thread-m(24) thread-n(26)
thread-l |    * * * $                       | thread-m(24) thread-n(26) thread-o(28)             
thread-m |        $                         |                                                    
thread-h |    *             $               | thread-o(28)                                       
tshell   |    * * *   $                     | thread-k(20) thread-m(24) thread-n(26) thread-o(28)
msh />                                                                                           
```

每行记录了一条系统的历史线程运行情况

- 第一列：当前执行线程的名称
- 中间列：当前线程的调用状态，按照优先级进行排列。
    - `$` : 代表当前正在运行线程的优先级
    - `*` : 代表处于就绪状态的线程优先级
- 第三列：就绪列表中的线程列表
