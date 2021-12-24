/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-11-15     RTT       the first version
 */
#ifndef _SC7A20_H_
#define _SC7A20_H_
#include "rtthread.h"

/* 3-axis data structure */
typedef struct sc7a20_3axes
{
    rt_int16_t x;
    rt_int16_t y;
    rt_int16_t z;
}sc7a20_3axes_t;

#define OUT_TEMP_L_ADDR               0x0C
#define OUT_TEMP_H_ADDR               0x0D
#define WHO_AM_I_ADDR                 0x0F
#define USER_CAL_ADDR                 0x13        // 0x13-1A
#define NVM_WR_ADDR                   0x1E
#define TEMP_CFG_ADDR                 0x1F
#define CTRL_REG1_ADDR                0x20
#define CTRL_REG2_ADDR                0x21
#define CTRL_REG3_ADDR                0x22
#define CTRL_REG4_ADDR                0x23
#define CTRL_REG5_ADDR                0x24
#define CTRL_REG6_ADDR                0x25
#define REFERENCE_ADDR                0x26
#define STATUS_REG_ADDR               0x27
#define OUT_X_L_ADDR                  0x28
#define OUT_X_H_ADDR                  0x29
#define OUT_Y_L_ADDR                  0x2A
#define OUT_Y_H_ADDR                  0x2B
#define OUT_Z_L_ADDR                  0x2C
#define OUT_Z_H_ADDR                  0x2D
#define FIFO_CTRL_REG_ADDR            0x2E
#define FIFO_SRC_REG_ADDR             0x2F
#define AOI1_CTRL_REG_ADDR            0x30
#define AOI1_SRC_ADDR                 0x31
#define AOI1_THS_ADDR                 0x32
#define AOI1_DURATION_ADDR            0x33
#define AOI2_CTRL_REG_ADDR            0x34
#define AOI2_SRC_ADDR                 0x35
#define AOI2_THS_ADDR                 0x36
#define AOI2_DURATION_ADDR            0x37
#define CLICK_CTRL_REG_ADDR           0x38
#define CLICK_SRC_ADDR                0x39
#define CLICK_THS_ADDR                0x3A
#define TIME_LIMIT_ADDR               0x3B
#define TIME_LATENCY_ADDR             0x3C
#define TIME_WINDOW_ADDR              0x3D
#define DIG_CTRL_ADDR                 0x57

typedef rt_uint8_t  out_temp_l_t;
typedef rt_uint8_t  out_temp_h_t;
typedef rt_uint8_t  who_am_i_t;
typedef rt_uint8_t  user_cal_t;
typedef rt_uint8_t  nvm_wr_t;
typedef rt_uint8_t  temp_cfg_t;

typedef union
{
    rt_uint8_t  bytes;
    struct
    {
        rt_uint8_t  xen          :1;     /* X轴使能，默认值1 */     
        rt_uint8_t  yen          :1;     /* Y轴使能，默认值1 */ 
        rt_uint8_t  zen          :1;     /* Z轴使能，默认值1 */ 
        rt_uint8_t  lpen         :1;     /* 低功耗使能，默认值1 */
        rt_uint8_t  odr          :4;     /* 数据率选择，默认值：0000 */
    }bits;
}ctrl_reg1_t;

typedef enum
{
    POWER_OFF                    = 0b0000,    /* 电源关断模式 */
    NOMAL_1KHz                   = 0b0001,    /* 正常 l / 低功耗模式 (1 Hz) */
    NOMAL_10KHz                  = 0b0010,    /* 正常 l / 低功耗模式(10 Hz) */
    NOMAL_25KHz                  = 0b0011,    /* 正常 l / 低功耗模式(25 Hz) */
    NOMAL_50KHz                  = 0b0100,    /* 正常 l / 低功耗模式(50 Hz) */
    NOMAL_100KHz                 = 0b0101,    /* 正常 l / 低功耗模式(100 Hz) */
    NOMAL_200KHz                 = 0b0110,    /* 正常 l / 低功耗模式(200 Hz) */
    NOMAL_400KHz                 = 0b0111,    /* 正常 l / 低功耗模式(400 Hz) */
    LOW_POWER                    = 0b1000,    /* 低功耗模式 (1.6 KHz) */
    NOMAL_LOWPOWER               = 0b1001,    /* 正常工作模式 (1.25 kHz) / 低功耗模式(5 KHz) */
}DATA_OUTPUT_RATED;

typedef union
{
    rt_uint8_t  bytes;
    struct
    {
        rt_uint8_t  hpis1        :1;     /* AOI1 数据高通滤波使能。(0：滤波禁止； 1：滤波使能) */     
        rt_uint8_t  hpis2        :1;     /* AOI2 数据高通滤波使能。(0：滤波禁止； 1：滤波使能) */ 
        rt_uint8_t  hpclick      :1;     /* CLICK 功能高通滤波使能。(0：滤波禁止； 1：滤波使能) */ 
        rt_uint8_t  fds          :1;     /* 数据滤波选择。默认值：0  (0：跳过内部滤波； 1：内部滤波以后的数据输出到数据寄存器或 FIFO */
        rt_uint8_t  hpcf         :2;     /* 高通截止频率选择 */
        rt_uint8_t  hpm          :2;     /* 高通模式选择。默认值：00 */
    }bits;
}ctrl_reg2_t;


typedef enum
{
    NOMAL_RESET                  = 0b00,     /* 正常模式 (读高通滤波自动复位) */
    FILTERED_REF_SIG             = 0b01,     /* 滤波参考信号 */
    NOMAL                        = 0b10,     /* 正常模式 */
    INIT_RESET                   = 0b11,     /* 中断事件自动复位 */
}HIGH_PASS_MODE;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t               :1;     /* 保留 */     
        rt_uint8_t  i1_overrun   :1;     /* FIFO 溢出中断在 INT1 脚上。默认值：0 (0：禁止； 1：使能) */ 
        rt_uint8_t  i1_wtm       :1;     /* FIFO WTM 中断在 INT1 脚上。默认值：0 (0：禁止； 1：使能) */ 
        rt_uint8_t  i1_drdy2     :1;     /* DRDY2 中断在 INT1 脚上。默认值：0 (0：禁止； 1：使能) */
        rt_uint8_t  i1_drdy1     :1;     /* DRDY1 中断在 INT1 脚上。默认值：0 (0：禁止； 1：使能) */
        rt_uint8_t  i1_aoi2      :1;     /* AOI2 中断在 INT1 脚上。默认值：0 (0：禁止； 1：使能) */
        rt_uint8_t  i1_aoi1      :1;     /* AOI1 中断在 INT1 脚上。 默认值：0 (0：禁止； 1：使能) */
        rt_uint8_t  i1_click     :1;     /* CLICK 中断在 INT1 脚上。默认值：0 (0：禁止； 1：使能) */
    }bits;
}ctrl_reg3_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  sim          :1;     /* SPI 串行接口模式配置。默认值：0。(0：4 线接口； 1：3 线接口) */
        rt_uint8_t  st           :2;     /* 自测试使能。默认值：00。(00: 自测试禁止；其他： 参考“自测试模式配置” ) */
        rt_uint8_t  dlpf_en      :1;     /* 数字低通滤波器使能。默认值：0。(0： 禁止； 1：使能) */
        rt_uint8_t  fs           :2;     /* 全量程选择。默认值：00。(00： +/- 2G； 01： +/- 4G； 10： +/- 8G； 11： +/- 16G) */
        rt_uint8_t  ble          :1;     /* 大端/小端数据选择。默认值：0。(0：低字节数据在低地址；1：高字节数据在低地址) */
        rt_uint8_t  bdu          :1;     /* 块数据更新。默认值：0。(0：连续更新；1：输出数据寄存器不更新直到 MSB 和 LSB 被读取) */
    }bits;
}ctrl_reg4_t;

typedef enum
{
    SELF_TEST_NOMAL              = 0b00,     /* 正常模式 */
    SELF_TEST0                   = 0b01,     /* 自测试 0 */
    SELF_TEST1                   = 0b10,     /* 自测试 1 */
}SELF_TEST;

typedef union
{
    rt_uint8_t bytes; 
    struct
    {
        rt_uint8_t  aoi2_d4d     :1;     /* 4D 使能： 使能 AOI2 上 4D 检测，需要同时要把 AOI2 配置寄存器中的 6D 为置 1。 */
        rt_uint8_t  aoi2_lir     :1;     /* 锁存 AOI2 配置寄存器上指定中断功能的中断响应信号（PIN 脚的高低电平）。 */
        rt_uint8_t  aoi1_d4d     :1;     /* 4D 使能： 使能 AOI1 上 4D 检测，需要同时要把 AOI1 配置寄存器中的 6D 为置 1。 */
        rt_uint8_t  aoi1_lir     :1;     /* 锁存 AOI1 配置寄存器上指定中断功能的中断响应信号（PIN 脚的高低电平）(0： 不锁存中断信号； 1：锁存中断信号) */
        rt_uint8_t               :2;     /* 保留 */  
        rt_uint8_t  fifo_en      :1;     /* FIFO 使能。默认值：0。(0：FIFO 禁止； 1： FIFO 使能,需要同时使能 FIFO 控制寄存器的 FIFO 模式才能正常工作) */
        rt_uint8_t  boot         :1;     /* 重载修调值。默认值：0。(0：正常模式； 1：重载修调值) */
    }bits;
}ctrl_reg5_t;

typedef union 
{
    rt_uint8_t bytes; 
    struct
    {
        rt_uint8_t               :1;    /* 保留 */    
        rt_uint8_t  h_lactive    :1;    /* 0：中断触发时输出高电平(默认低电平)；1：中断触发时输出低电平(默认高电平) */
        rt_uint8_t               :1;    /* 保留 */ 
        rt_uint8_t               :1;    /* 保留 */ 
        rt_uint8_t  i2_boot      :1;    /* BOOT 状态在 INT2 脚上。 默认值：0。(0：禁止； 1：使能) */
        rt_uint8_t  I2_AOI2      :1;    /* AOI2 中断在 INT2 脚上。 默认值：0。(0：禁止； 1：使能) */
        rt_uint8_t  I2_AOI1      :1;    /* AOI1 中断在 INT2 脚上。 默认值：0。(0：禁止； 1：使能) */
        rt_uint8_t  I2_CLICK     :1;    /* CLICK 中断在 INT2 脚上。默认值：0。(0：禁止； 1：使能) */
    }bits;
}ctrl_reg6_t;

typedef union {
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  xda          :1;    /* X 轴新的数据到来。 默认值：0 */         
        rt_uint8_t  yda          :1;    /* Y 轴新的数据到来。 默认值：0 */    
        rt_uint8_t  zda          :1;    /* Z 轴新的数据到来。 默认值：0 */ 
        rt_uint8_t  zyxda        :1;    /* X, Y 和 Z 三个轴新的数据全都转换完成。默认值：0 */    
        rt_uint8_t  xor          :1;    /* X 轴新的数据已经覆盖老的数据。 默认值：0 */
        rt_uint8_t  yor          :1;    /* Y 轴新的数据已经覆盖老的数据。 默认值：0 */
        rt_uint8_t  zor          :1;    /* Z 轴新的数据已经覆盖老的数据。 默认值：0 */
        rt_uint8_t  zyxor        :1;    /* X，Y 和 Z 三个轴新的数据至少有一个已经覆盖老的数据。 默认值：0 */
    }bits;
}state_reg_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        const rt_uint8_t    out_x_l;    
        const rt_uint8_t    out_x_h;
        const rt_uint8_t    out_y_l;
        const rt_uint8_t    out_y_h;
        const rt_uint8_t    out_z_l;
        const rt_uint8_t    out_z_h;   
    }bits;
}out_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  fth          :5;    /* FIFO 功能 WTM 阈值设置 */
        rt_uint8_t  tr           :1;    /* FIFO 触发模式选择。默认值：0 */
        rt_uint8_t  fm           :2;    /* FIFO 模式选择。默认值：00 */
    }bits;
}fifo_cfg_t;

typedef enum
{
    BY_PASS                      = 0b00,  /* By-Pass 模式 (旁路模式，即不使用 FIFO 功能) */
    FIFO                         = 0b01,  /* FIFO 模式 (缓存满未及时读取，新数据丢弃) */
    STREAM                       = 0b10,  /* Stream 模式 (缓存满后，最早数据丢弃，添加新数据) */  
    trigger                      = 0b11,  /* 触发模式 (AOI1 或者 AOI2 中断事件有效，从 stream 模式进入 FIFO 模式) */
}FIFO_MODE;

typedef union {
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  fss          :5;    /* 在 FIFO 中未读取数据的组数 */
        rt_uint8_t  empty        :1;    /* 当 FIFO 中无数据时，EMPTY 位置“1” */ 
        rt_uint8_t  over         :1;    /* 当 FIFO 中的数据溢出时，OVER 位置“1” */
        rt_uint8_t  wtm          :1;    /* 当 FIFO 中的数据个数超过设定阈值时，WTM 位置“1” */          
    }bits;
}fifo_src_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  xlie_xdowne  :1;    /* X 轴低事件中断或者 X 轴方向检测中断使能。默认值：0 */ 
        rt_uint8_t  xhie_xupe    :1;    /* X 轴高事件中断或者 X 轴方向检测中断使能。默认值：0 */
        rt_uint8_t  ylie_ydowne  :1;    /* Y 轴低事件中断或者 Y 轴方向检测中断使能。默认值：0 */
        rt_uint8_t  yhie_yupe    :1;    /* Y 轴高事件中断或者 Y 轴方向检测中断使能。默认值：0 */  
        rt_uint8_t  zlie_zdowne  :1;    /* Z 轴低事件中断或者 Z 轴方向检测中断使能。默认值：0 */
        rt_uint8_t  zhie_zupe    :1;    /* Z 轴高事件中断或者 Z 轴方向检测中断使能。默认值：0 */       
        rt_uint8_t  sixd         :1;    /* 6 个方向检测功能使能。默认值：0。参考“中断模式”  */   
        rt_uint8_t  aoi          :1;    /* 与/或中断事件。默认值：0。参考“中断模式” */
    }bits;
}aoi1_crtl_reg_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
       rt_uint8_t  xl            :1;    /* X 轴低。默认值：0 */ 
       rt_uint8_t  xh            :1;    /* X 轴高。 默认值：0 */
       rt_uint8_t  yl            :1;    /* Y 轴低。默认值：0 */
       rt_uint8_t  yh            :1;    /* Y 轴高。 默认值：0 */
       rt_uint8_t  zl            :1;    /* Z 轴低。默认值：0 */
       rt_uint8_t  zh            :1;    /* Z 轴高。 默认值：0 */
       rt_uint8_t  ia            :1;    /* AOI1 中断触发状态位。默认值：0 */
       rt_uint8_t                :1;    /* 保留 */ 
    }bits;
}aoi1_src_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  ths          :7;    /* AOI1 阈值。 默认值： 000 0000 */
        rt_uint8_t               :1;    /* 保留 */ 
    }bits;
}aoi1_ths_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  duration     :7;    /* 持续时间设定值，1LSB = 1/ODR。默认值： 000 0000 */
        rt_uint8_t               :1;    /* 保留 */
    }bits;
}aoi1_duration_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  xlie_xdowne  :1;    /* X 轴低事件中断或者 X 轴方向检测中断使能。默认值：0 */ 
        rt_uint8_t  xhie_xupe    :1;    /* X 轴高事件中断或者 X 轴方向检测中断使能。默认值：0 */
        rt_uint8_t  ylie_ydowne  :1;    /* Y 轴低事件中断或者 Y 轴方向检测中断使能。默认值：0 */
        rt_uint8_t  yhie_yupe    :1;    /* Y 轴高事件中断或者 Y 轴方向检测中断使能。默认值：0 */  
        rt_uint8_t  zlie_zdowne  :1;    /* Z 轴低事件中断或者 Z 轴方向检测中断使能。默认值：0 */
        rt_uint8_t  zhie_zupe    :1;    /* Z 轴高事件中断或者 Z 轴方向检测中断使能。默认值：0 */       
        rt_uint8_t  sixd         :1;    /* 6 个方向检测功能使能。默认值：0。参考“中断模式”  */   
        rt_uint8_t  aoi          :1;    /* 与/或中断事件。默认值：0。参考“中断模式” */
    }bits;
}aoi2_crtl_reg_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  xl           :1;    /* X 轴低。默认值：0 */ 
        rt_uint8_t  xh           :1;    /* X 轴高。 默认值：0 */
        rt_uint8_t  yl           :1;    /* Y 轴低。默认值：0 */
        rt_uint8_t  yh           :1;    /* Y 轴高。 默认值：0 */
        rt_uint8_t  zl           :1;    /* Z 轴低。默认值：0 */
        rt_uint8_t  zh           :1;    /* Z 轴高。 默认值：0 */
        rt_uint8_t  ia           :1;    /* AOI2 中断触发状态位。默认值：0 */
        rt_uint8_t               :1;    /* 保留 */ 
    }bits;
}aoi2_src_t;

typedef union 
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  ths          :7;    /* AOI1 阈值。 默认值： 000 0000 */
        rt_uint8_t               :1;    /* 保留 */ 
    }bits;
}aoi2_ths_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  duration     :7;    /* 持续时间设定值，1LSB = 1/ODR。默认值： 000 0000 */
        rt_uint8_t               :1;    /* 保留 */
    }bits;
}aoi2_duration_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  XS           :1;    /* Z 轴双击中断使能控制。默认值：0 */ 
        rt_uint8_t  XD           :1;    /* Z 轴单击中断使能控制。默认值：0 */
        rt_uint8_t  YS           :1;    /* Y 轴双击中断使能控制。默认值：0 */
        rt_uint8_t  YD           :1;    /* Y 轴单击中断使能控制。默认值：0 */
        rt_uint8_t  ZS           :1;    /* X 轴双击中断使能控制。默认值：0 */
        rt_uint8_t  ZD           :1;    /* X 轴单击中断使能控制。默认值：0 */
        rt_uint8_t               :1;    /* 保留 */
        rt_uint8_t               :1;    /* 保留 */
    }bits;
}click_crtl_reg_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  x            :1;    /* X 轴检测到单双击。默认值：0 */     
        rt_uint8_t  y            :1;    /* Y 轴检测到单双击。默认值：0 */      
        rt_uint8_t  z            :1;    /* Z 轴检测到单双击。默认值：0 */      
        rt_uint8_t  sign         :1;    /* 单双击检测到的符号 */     
        rt_uint8_t  sclick       :1;    /* 单击检测状态位。默认值：0 */ 
        rt_uint8_t  dclick       :1;    /* 双击检测状态位。默认值：0 */ 
        rt_uint8_t  ia           :1;    /* 中断激活。默认值：0 */
        rt_uint8_t               :1;    /* 保留 */
    }bits;
}click_src_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  ths          :7;    /* 单双击触发的阈值。默认值：000 0000 */
        rt_uint8_t               :1;    /* 保留 */
    }bits;
}click_ths_t;

typedef union
{
   rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  tli          :7;    /* 单击检测时间的阈值。默认值：000 0000 */
        rt_uint8_t               :1;    /* 保留 */    
    }bits;
}time_limit_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  tla          :8;    /* 延迟时间。默认值：000 0000 */
    }bits;
}time_latency_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t  tw           :8;    /* 时间窗。默认值：000 0000 */
    }bits;  
}time_window_t;

typedef union
{
    rt_uint8_t bytes;
    struct
    {
        rt_uint8_t               :2;    /* 保留 */
        rt_uint8_t  i2c_pu       :1;    /* SDA 和 SCL 内部上拉电阻控制位。默认值：0 */
        rt_uint8_t  sdo_pu       :1;    /* SDO 内部上拉电阻控制位。默认值：0 */
        rt_uint8_t               :4;    /* 保留 */            
    }bits;
}dig_ctrl_t;

typedef union
{   
    out_temp_l_t        out_temp_l;
    out_temp_h_t        out_temp_h;
    who_am_i_t          who_am_i;
    user_cal_t          user_cal;
    nvm_wr_t            nvm_wr;
    temp_cfg_t          temp_cfg;
    ctrl_reg1_t         ctrl_reg1;
    ctrl_reg2_t         ctrl_reg2;
    ctrl_reg3_t         ctrl_reg3;
    ctrl_reg4_t         ctrl_reg4;
    ctrl_reg5_t         ctrl_reg5;
    ctrl_reg6_t         ctrl_reg6;
    rt_uint8_t          reference;
    state_reg_t         state_reg;
    out_t               out;
    fifo_cfg_t          fifo_cfg;
    fifo_src_t          fifo_src;
    aoi1_crtl_reg_t     aoi1_crtl_reg;
    aoi1_src_t          aoi1_src;
    aoi1_ths_t          aoi1_ths;
    aoi1_duration_t     aoi1_duration;
    aoi2_crtl_reg_t     aoi2_crtl_reg;
    aoi2_src_t          aoi2_src;
    aoi2_ths_t          aoi2_ths;
    aoi2_duration_t     aoi2_duration;
    click_crtl_reg_t    click_crtl_reg;
    click_src_t         click_src;
    click_ths_t         click_ths;
    time_limit_t        time_limit;
    time_latency_t      time_latency;
    time_window_t       time_window;
    dig_ctrl_t          dig_ctrl;
    rt_uint8_t          byte;
}sc7a20_reg_t;

#define OUT_TEMP_L              ((out_temp_l_t *)OUT_TEMP_L_ADDR)            
#define OUT_TEMP_H              ((out_temp_h_t *)OUT_TEMP_H_ADDR)     
#define WHO_AM_I                ((who_am_i_t *)WHO_AM_I_ADDR) 
#define USER_CAL                ((user_cal_t *)USER_CAL_ADDR)    
#define NVM_WR                  ((nvm_wr_t *)NVM_WR_ADDR)       
#define TEMP_CFG                ((temp_cfg_t *)TEMP_CFG_ADDR)    
#define CTRL_REG1               ((ctrl_reg1_t *)CTRL_REG1_ADDR) 
#define CTRL_REG2               ((ctrl_reg2_t *)CTRL_REG2_ADDR)     
#define CTRL_REG3               ((ctrl_reg3_t *)CTRL_REG3_ADDR)     
#define CTRL_REG4               ((ctrl_reg4_t *)CTRL_REG4_ADDR)     
#define CTRL_REG5               ((ctrl_reg5_t *)CTRL_REG5_ADDR)     
#define CTRL_REG6               ((ctrl_reg6_t *)CTRL_REG6_ADDR)     
#define REFERENCE               ((reference_t *)REFERENCE_ADDR)      
#define STATUS_REG              ((status_reg_t *)STATUS_REG_ADDR)    
#define out_x_l                 ((out_x_l_addr_t *)OUT_X_L_ADDR)                                         
#define OUT_X_H                 ((out_x_h_addr_t *)OUT_X_H_ADDR)                        
#define OUT_Y_L                 ((out_y_l_addr_t *)OUT_Y_L_ADDR)                        
#define OUT_Y_H                 ((out_y_h_addr_t *)OUT_Y_H_ADDR)                        
#define OUT_Z_L                 ((out_z_l_addr_t *)OUT_Z_L_ADDR)                        
#define OUT_Z_H                 ((out_z_h_addr_t *)OUT_Z_H_ADDR)      
#define FIFO_CTRL               ((fifo_ctrl_t *)FIFO_CTRL_REG_ADDR)  
#define FIFO_SRC                ((fifo_src_t *)FIFO_SRC_REG_ADDR) 
#define AOI1_CTRL               ((aoi1_ctrl_t *)AOI1_CTRL_REG_ADDR)  
#define AOI1_SRC                ((aoi1_src_t *)AOI1_SRC_ADDR)                     
#define AOI1_THS                ((aoi1_ths_t *)AOI1_THS_ADDR)    
#define AOI1_DURATION           ((aoi1_duration_t *)AOI1_DURATION_ADDR)            
#define AOI2_CTRL_REG           ((aoi2_ctrl_reg_t *)AOI2_CTRL_REG_ADDR) 
#define AOI2_SRC                ((aoi2_src_t *)AOI2_SRC_ADDR)                      
#define AOI2_THS                ((aoi2_ths_t *)AOI2_THS_ADDR)       
#define AOI2_DURATION           ((aoi2_duration_t *)AOI2_DURATION_ADDR)   
#define CLICK_CTRL_REG          ((click_ctrl_reg_t *)CLICK_CTRL_REG_ADDR)  
#define CLICK_SRC               ((click_src_t *)CLICK_SRC_ADDR)     
#define CLICK_THS               ((click_ths_addr_t *)CLICK_THS_ADDR)     
#define TIME_LIMIT              ((time_limit_t *)TIME_LIMIT_ADDR)       
#define TIME_LATENCY            ((time_latency_t *)TIME_LATENCY_ADDR)  
#define TIME_WINDOW             ((time_window_t *)TIME_WINDOW_ADDR)   
#define DIG_CTRL                ((dig_ctrl_t *)DIG_CTRL_ADDR)                   

typedef struct sc7a20_device
{
    rt_device_t bus;
    rt_uint8_t  id;
    rt_uint8_t  i2c_addr;
}sc7a20_device_t;

sc7a20_device_t *sc7a20_init(const char *dev_name, rt_uint8_t param);
void sc7a20_deinit(sc7a20_device_t* dev);
#endif /* PACKAGES_HELLO_V1_0_0_SC7A20_H_ */
