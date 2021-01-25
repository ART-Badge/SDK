# 基于 RT-Thread 的线程池实现

---

## 1、 基本介绍

### 1.1 线程池

线程池是一种多线程处理形式，处理过程中将任务添加到队列，然后在创建线程后自动启动这些任务。

- 线程池管理器（ThreadPoolManager）：用于创建并管理线程池；
- 工作线程（WorkThread）：线程池中的线程；
- 任务接口（Task）：每个任务必须实现的接口，以供工作线程调度任务的执行；
- 任务队列（queue）：用于存放没有处理的任务。提供一种缓冲机制。

### 1.2 线程池的由来

在面向对象编程中，创建和销毁对象是很费时间的，因为创建一个对象要获取内存资源或者其它更多资源。在Java中更是如此，虚拟机将试图跟踪每一个对象，以便能够在对象销毁后进行垃圾回收。所以提高服务程序效率的一个手段就是尽可能减少创建和销毁对象的次数，特别是一些很耗资源的对象创建和销毁。如何利用已有对象来服务就是一个需要解决的关键问题，其实这就是一些"池化资源"技术产生的原因。线程池也是由此而来。

> 本线程池的实现来源于 <https://github.com/armink/EasyDataManager> 开源项目

## 2、 应用场景

使用线程池是为了减小线程本身的开销对应用性能所产生的影响，但是其前提是线程本身创建、销毁的开销和线程执行任务的开销相比是不可忽略的。如果线程本身创建、销毁的开销对应用程序的性能可以忽略不计，那么使用/不使用线程池对程序的性能并不会有太大的影响。

- 单位时间内处理的任务频繁，且任务时间较短；
- 对实时性要求较高。如果接收到任务之后再创建线程，可能无法满足实时性的要求，此时必须使用线程池；
- 必须经常面对高突发性事件。比如 Web 服务器。如果有足球转播，则服务器将产生巨大冲击，此时使用传统方法，则必须不停的大量创建、销毁线程。此时采用动态线程池可以避免这种情况的发生。

## 3、用法

主要通过以下六点介绍如何使用线程池：

- 定义线程池
- 初始化线程池
- 添加任务到线程池
- 销毁线程池
- 同步锁
- 释放同步锁

### 3.1 定义线程池

通过 **thread_pool** 或 **thread_pool_t** 定义线程池

```c
//方式一 结构体类型
thread_pool pool;

//方式二 结构体指针类型
thread_pool_t pool;
```
*注：本文采用**方式一**定义线程池*

### 3.2 初始化线程池

- 原型

```c
init_thread_pool(thread_pool_t const pool, 
               uint8_t max_thread_num, 
               uint32_t thread_stack);
```

- API

|参数|名称|类型|描述|
|:---|:---|:---|:---|
|入参|pool|thread_pool_t|对象|
|入参|max_thread_num|uint8_t|线程数量|
|入参|thread_stack|uint32_t|堆栈大小|
|返回值|error_code|thread_pool_err|返回状态|


- Demo

创建一个线程数量为5，堆栈大小为1024 Byte的线程池

```c
thread_pool pool;

init_thread_pool(pool, "A", 5, 1024);	
```

### 3.3 添加任务

- 原型

```c
add_task(thread_pool_t const pool,
        void (*process)(void *arg), 
        void *arg);
```
- API 

|参数|名称|类型|描述|
|:---|:---|:---|:---|
|入参|pool|thread_pool_t|对象|
|入参|process(void *arg)|void|任务处理函数|
|入参|arg|void *|任务处理函数入参|
|返回值|error_code|thread_pool_err|返回状态|


- Demo

线程池中添加任务 task1，启动时够通过日志看到 task1 在线程池A中的运行效果。

```c
char str_a[]="A";
//任务处理函数
static void *task1(void *arg) {
    LOG_D("This is %s test ",(char*)arg);
	return NULL;
}
//添加任务
pool.addTask(&pool, task1, str_a);

```

### 3.4 销毁线程池

- 原型

```c
destroy(thread_pool_t pool);
```

- API

|参数|名称|类型|描述|
|:---|:---|:---|:---|
|入参|pool|thread_pool_t|对象|
|返回值|error_code|thread_pool_err|返回状态|


- Demo

销毁线程池

```c
pool.destroy(&pool);				
```
### 3.5 同步锁

- 原型
```c
sync_lock(thread_pool_t pool)；
```
- API 

|入参|名称|类型|描述|
|:---|:---|:---|:---|
|入参|pool|thread_pool_t|对象|
|返回值|NULL|void*|返回状态|


- Demo

```c
pool.lock(&pool);	
```

### 3.6 释放同步锁

- 原型

```c
sync_unlock(thread_pool_t pool)；
```

- API 

|入参|名称|类型|描述|
|:---|:---|:---|:---|
|入参|pool|thread_pool_t|对象|
|返回值|NULL|void*|返回状态|


- Demo

```c
pool.unlock(&pool);
```

## 4、示例

代码位于 thread_pool_sample.c ，源码大致如下：

```c
#include <finsh.h>
#include "thread_pool.h"

static void task(void *arg) {
    LOG_I("The task on thread %.*s is running", RT_NAME_MAX, rt_thread_self()->name);
    rt_thread_delay(rt_tick_from_millisecond((uint32_t)arg));
    LOG_I("The task on thread %.*s will finish", RT_NAME_MAX, rt_thread_self()->name);
}

static void thread_pool_sample(uint8_t argc, char **argv) {
    thread_pool pool;

    init_thread_pool(&pool, "sam", 3, 1024);
    /* add 5 task to thread pool */
    pool.add_task(&pool, task, (void *)(rand() % 5000));
    pool.add_task(&pool, task, (void *)(rand() % 5000));
    pool.add_task(&pool, task, (void *)(rand() % 5000));
    pool.add_task(&pool, task, (void *)(rand() % 5000));
    pool.add_task(&pool, task, (void *)(rand() % 5000));
    /* wait 10S */
    rt_thread_delay(rt_tick_from_millisecond(10 * 1000));
    /* delete all task */
    pool.del_all(&pool);
    /* destroy the thread pool */
    pool.destroy(&pool);
}
MSH_CMD_EXPORT(thread_pool_sample, Run thread pool sample);
```

功能流程大致如下：

- thread_pool_sample 中会初始化一个包含有 3 个线程的线程池
- 向线程池中一次性添加 5 个任务
- 每个任务执行时会延时 5S 的随机值
- 等待 10 S 删除线程中剩余的全部任务
- 销毁线程池

使用前需在 menuconfig 中开启 `THREAD_POOL_USING_SAMPLES` 选项。

开启后，在 Finsh/MSH 中执行 `thread_pool_sample` 命令，即可看到示例运行效果，大致如下：

```shell
msh />thread_pool_sample
[D/thread_pool] create thread success.Current total thread number is 1
[D/thread_pool] create thread success.Current total thread number is 2
[D/thread_pool] create thread success.Current total thread number is 3
[D/thread_pool] initialize thread pool success!
[D/thread_pool.sample] The task on thread sam_0 is running
[D/thread_pool] add a task to task queue success.
[D/thread_pool.sample] The task on thread sam_1 is running
[D/thread_pool] add a task to task queue success.
[D/thread_pool.sample] The task on thread sam_2 is running
[D/thread_pool] add a task to task queue success.
[D/thread_pool] add a task to task queue success.
[D/thread_pool] add a task to task queue success.
[D/thread_pool.sample] The task on thread sam_1 finish
[D/thread_pool.sample] The task on thread sam_1 is running
[D/thread_pool.sample] The task on thread sam_1 finish
[D/thread_pool.sample] The task on thread sam_1 is running
[D/thread_pool.sample] The task on thread sam_0 finish
[D/thread_pool.sample] The task on thread sam_2 finish
[D/thread_pool.sample] The task on thread sam_1 finish
[D/thread_pool] delete all wait task success
[D/thread_pool] Thread pool destroy success
```

## 5、联系方式

* 维护：[armink](https://github.com/armink)
* 主页：https://github.com/armink-rtt-pkgs/thread_pool