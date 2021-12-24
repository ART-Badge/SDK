/**
****************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
****************************************************************************************************
* @file      otp.h
* @brief
* @date      2018-04-04
* @version   v1.0
* **************************************************************************************************
*/

#ifndef _OTP_H_
#define _OTP_H_

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/* OTP start addr: 0x00200164, size: 0x388 */
typedef struct otp_struct
{
    uint8_t rsvd0[51];                        //start addr:0x00200164
    uint8_t bt_bd_addr[6];                    //start addr:0x00200197
    uint8_t rsvd0_1[73];
    uint8_t timer_diff_threshold;              //start addr:0x002001e6
    uint8_t rsvd01[5];                        //start addr:0x002001e7
    uint8_t bw_rf_low_clk_frac;               //0x002001ec, 0: 32768 Hz, 1: 32000 Hz
    uint8_t rsvd02[207];

    /*****************upper *********************/
    /* start addr:0x002002bc,size:0x78*/
    uint8_t upper_rsvd0[32];
    uint16_t stack_en: 1;                      //start addr:0x002002dc
    uint16_t upper_rsvd1: 15;
    uint8_t upper_rsvd2[54];
    uint32_t upper_rsvd3: 5;
    uint32_t ota_with_encryption_data: 1;
    uint32_t ota_with_encryption_use_aes256: 1;
    uint32_t upper_rsvd4: 3;
    uint32_t ota_link_loss_reset: 1;
    uint32_t upper_rsvd5: 21;
    uint8_t ota_rst_tgt_name[8];
    uint8_t ota_timeout_total;                //0x00200320
    uint8_t ota_timeout_wait4_conn;
    uint8_t ota_timeout_wait4_image_transfer;
    uint8_t ota_timeout_ctittv;
    uint8_t ota_timeout_wait_packet;
    uint8_t upper_rsvd7[11];
    uint8_t upper_padding[4];
    /****************end upper *******************/

    /*****************boot_cfg *******************/
    /* start addr:0x00200334, size:0x8*/
    uint32_t SWD_ENABLE: 1;
    uint32_t boot_cfg_rsvd0: 4;
    uint32_t CLK_SRC_FLASH: 2;
    uint32_t r_cpu_slow_en: 1;
    uint32_t boot_cfg_rsvd1: 24;
    uint32_t CLK_SRC_LOW_POW_USER: 2; //0: Internal 32K, 1: External 32K
    uint32_t boot_cfg_rsvd2: 30;
    /*************end boot_cfg *******************/

    /*****************flash_cfg *******************/
    /* start addr:0x0020033c,size:0x10*/
    uint32_t flash_cfg_rsvd0: 27;
    uint32_t is_support_deep_power_down: 1;
    uint32_t rsvd: 1;
    uint32_t data_byte_for_toggle_cs: 2;
    uint32_t basic_cfg_from_otp: 1;       /*!< basic configuration source ( 0: QVL; 1: OTP) */
    uint32_t flash_wait_max;              //0x00200340
    uint32_t spic_wait_max;               //0x00200344
    uint8_t flash_cfg_rsvd1[4];

    /*************end flash_cfg *******************/

    /*****************flash_setting ***************/
    /* start addr:0x0020034c,size:0x8 */
    uint8_t bit_mode: 2;
    uint8_t flash_setting_rsvd0: 6;
    uint8_t flash_setting_rsvd1: 3;
    uint8_t bp_enable : 1;
    uint8_t bp_lv : 4;
    uint8_t delay_10us_before_toggle_cs;       //start addr:0x0020034e
    uint8_t delay_10us_after_toggle_cs;
    uint8_t disable_irq_lv: 3;
    uint8_t read_turn_on_off_rf: 1;
    uint8_t flash_setting_rsvd2: 4;
    uint8_t flash_setting_rsvd3[2];            //start addr:0x00200351
    uint8_t flash_dma_ch: 4;
    uint8_t flash_setting_rsvd4: 4;
    /*************end flash_setting *******************/

    uint8_t rsvd2[16];                         //start addr:0x00200354, size:0xe, padding size:0x2
    uint8_t rsvd3[20];                         //start addr:0x00200364, size:0x14

    /*****************os_cfg *********************/
    /* start addr: 0x00200378, size:0x1e,padding 2 bytes to ensure 4 bytes align */
    uint8_t getStackHighWaterMark : 1;         /* 0 for release version, 1 for debug version */
    uint8_t checkForStackOverflow : 1;         /* 0 for release version, 1 for debug version */
    uint8_t printAllLogBeforeEnterDLPS : 1;    /* 0 for release version, 1 for debug version */
    uint8_t dumpMemoryWhenHardFaultOrWDG : 1;  /* 0 for release version, 1 for debug version */
    uint8_t dumpMemoryUsage : 1;               /* 0 for release version, 1 for debug version */
    uint8_t enableASSERT: 1;                   /* 0 for release version, 1 for debug version */
    uint8_t cpu_sleep_en: 1;                   /* default = 1 */
    uint8_t printDLPSCheckLog: 1;              /* default = 0 */

    uint8_t wdgConfigDivfactor;               //addr=0x00200379, default = 31
    uint8_t wdgConfigCntLimit : 4;            /* refer to WDG_Config(), default = 15 */
    uint8_t wdgEnableInRom : 1;               /* 1 for release version, 0 for debug version */
    uint8_t wdgResetInRom : 1;                /* 1 for release version, 0 for debug version */
    uint8_t wdgMode : 2;                      /* 0: interrupt CPU,     1: reset all but aon
                                                 2: reset core domain, 3: reset all */

    uint8_t  timerMaxNumber;                  /* default = 0x20 */
    uint8_t  timerQueueLength;                /* default = 0x20 */
    uint8_t  UseAliOS  : 1;
    uint8_t  reserved  : 7;
    uint16_t  dyn_malloc_task_stack_size;

    uint32_t appDataAddr;                     //0x00200380
    uint32_t appDataSize;                     //0x00200384
    uint32_t heapDataONSize;                  //0x00200388
    uint32_t heapBufferONSize;                //0x0020038c

    uint16_t idle_task_stack_size;             /* measured in bytes, default 256 * 4 bytes */
    uint16_t timer_task_stack_size;            /* measured in bytes, default 256 * 4 bytes */
    uint16_t lower_task_stack_size;            /* measured in bytes, default 768 * 4 bytes */
    uint8_t os_cfg_padding[2];
    /****************end os_cfg *******************/

    /****************platform config****************/
    /* start addr: 0x00200398, size: 0xfe, padding size: 0x2 */
    uint32_t logPin : 6;                 /* default = P0_3 */
    uint32_t logChannel : 2;             /* LogChannel_TypeDef: default is LOG_CHANNEL_LOG1_UART */
    uint32_t logBaudRate : 4;            /* UartBaudRate_TypeDef: default is BAUD_RATE_2000000 */
    uint32_t platform_cfg_rsvd0 : 4;
    uint32_t logDisable : 1;             /* Disable all DBG_DIRECT and DBG_BUFFER log */
    uint32_t dump_info_before_reset : 1;
    uint32_t write_info_to_flash_before_reset : 1;
    uint32_t platform_cfg_rsvd1 : 1;
    uint32_t log_ram_type : 1;
    uint32_t image_split_read : 1;       /* using split read in image integrity check */
    uint32_t reboot_reason_record_limit_power_2: 4;
    uint32_t write_reason_to_flash_before_reset: 1;
    uint32_t platform_cfg_rsvd2 : 4;
    uint32_t run_in_app : 1;
    uint32_t aes_key[8];                 /* for OTA encryption/decryption */
    uint32_t reboot_record_address;      // 0x002003bc

    uint32_t platform_cfg_rsvd3: 17;     // 0x002003c0
    uint32_t resetWhenHardFault : 1;
    uint32_t log_ram_size : 3;
    uint32_t hardfault_print_buf_log : 1;
    uint32_t platform_cfg_rsvd4: 8;
    uint32_t log_timestamp_src : 2;

    uint32_t platform_cfg_rsvd5: 13;     //adc_channel, 0x002003c4
    uint32_t systick_clk_src: 1;
    uint32_t platform_cfg_rsvd51: 12;
    uint32_t ftl_use_mapping_table: 1;
    uint32_t ftl_app_logical_addr_base: 3;
    uint32_t ftl_only_gc_in_idle: 1;
    uint32_t write_info_to_flash_when_hardfault: 1;

    uint32_t systick_ext_clk_freq;       //0x002003c8
    uint32_t share_cache_ram_reg;           //0x002003cc
    uint32_t reboot_reason_record_address;  //0x002003d0

    uint32_t  flash_info_tbl_rsvd0[4];   //start addr: 0x002003d4, size = 0x50
    /*** flash layout table, 12 words***/
    uint32_t  ota_bank0_addr;            //0x002003e4
    uint32_t  ota_bank0_size;
    uint32_t  ota_bank1_addr;
    uint32_t  ota_bank1_size;
    uint32_t  ftl_addr;                  //0x002003f4
    uint32_t  ftl_size;
    uint32_t  ota_tmp_addr;
    uint32_t  ota_tmp_size;
    uint32_t  bkp_data1_addr;            //0x00200404
    uint32_t  bkp_data1_size;
    uint32_t  bkp_data2_addr;
    uint32_t  bkp_data2_size;            //0x00200410
    /***** end flash layout table*******/
    uint32_t  HardFault_Record_CFG;
    uint32_t  HardFault_Record_BegAddr;
    uint32_t  HardFault_Record_EndAddr;
    uint8_t  flash_info_tbl_rsvd1[3];
    uint8_t  flash_info_tbl_padding;    //padding 1 byte

    uint8_t  flash_basic_cfg_rsvd[18];  // start addr: 0x00200424, size:0x12
    uint8_t  flash_adv_cfg_rsvd[12];    //start addr: 0x00200436 , size:0xc
    uint8_t  flash_query_info_rsvd[12]; // start addr: 0x00200442, size:0xc
    uint8_t  flash_spec_cfg_rsvd[72];   //start addr: 0x0020044e ,size:0x48
    uint8_t  flash_padding[2];
    /****************end platform config****************/

    uint8_t  ram_cfg_rsvd[16];          //start addr = 0x00200498, size= 0x10
    uint8_t  phy_init_cfg_rsvd0[3];     //start addr = 0x002004a8, size=0x28
    uint8_t  tpm_max_txgain_LE1M;       //start addr = 0x002004ab
    uint8_t  tpm_max_txgain_LE2M;
    uint8_t  tpm_max_txgain_LE2M_2402;
    uint8_t  tpm_max_txgain_LE2M_2480;
    uint8_t  phy_init_cfg_rsvd1[33];
    uint8_t  lop_rsvd[28];             //start addr = 0x002004d0, size=0x1c
}  __attribute__((packed))T_OTP_CFG;
/*end otp, addr = 0x002004Ec*/

typedef enum
{
    EXTERNAL_CLOCK  = 0,
    CORE_CLOCK      = 1
} SYSTICK_CLK_SRC_TYPE;

#define OTP_STRUCT_BASE             0x00200164UL
#define OTP                         ((T_OTP_CFG *) OTP_STRUCT_BASE)

#ifdef __cplusplus
}
#endif

#endif  /* _OTP_H_ */

