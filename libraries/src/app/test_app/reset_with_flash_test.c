/*
  reset aon when flash is working

  test case:
  1) flash write/erase (limit time)
  2) flash try high speed
  3) flash auto mode + dma
  4) flash split read + dma
  5) flash read + cache enable
  */

#include "rtl876x.h"
#include "flash_map.h"
#include "trace.h"
#include "flash_device.h"
#include "rtl876x_wdg.h"
#include "app_section.h"
__STATIC_INLINE uint32_t flash_auto_read(uint32_t addr)
{
    return *(uint32_t *)addr;
}
extern void flash_auto_write(uint32_t start_addr, uint32_t data);
extern uint32_t flash_split_read(uint32_t start_addr, uint32_t data_len, uint8_t *data);
uint8_t lock_level = 0;
#define BIT_CACHE_FLUSH   BIT(0)
#define BIT_CACHE_FLUSH_DONE    BIT(1)
void cache_flush(void)
{
    //flush with rising edge.
    CACHE->FLUSH |= BIT_CACHE_FLUSH;
    //sync
    while (1)
    {
        if (CACHE->FLUSH & BIT_CACHE_FLUSH_DONE)
        {
            break;
        }
    }
    CACHE->FLUSH = 0;
    return;
}

void wdg_config_resetallall_1ms(void)
{
    WDG_ClockEnable();
    WDG_Config(1, 3, RESET_ALL);  // ~ 1 ms
    WDG_Enable();
    while (1); /* wait until reset */
}
void wdg_config_resetAON_1ms(void)
{
    WDG_ClockEnable();
    WDG_Config(1, 3, RESET_ALL_EXCEPT_AON);  // ~ 1 ms
    WDG_Enable();
    while (1); /* wait until reset */
}
void wdg_config_resetall(void)
{
    WDG_ClockEnable();
    WDG_Config(1, 0, RESET_ALL);
    WDG_Enable();
    while (1); /* wait until reset */
}

void wdg_config_reset_except_AON(void)
{
    WDG_ClockEnable();
    WDG_Config(1, 0, RESET_ALL_EXCEPT_AON);
    WDG_Enable();
    while (1); /* wait until reset */
}


uint32_t sub_count(uint32_t count)
{
    if (count)
    {
        for (int i = 0; i < 32; ++i)
        {
            if (count & BIT(i))
            {
                count &= ~BIT(i);
                return count;
            }
        }
    }
    else
    {
        return 0;
    }
    return 0;
}

void reset_test_boot(void)
{

    //read test time
    uint32_t time;
    uint32_t time_record_addr = 0x800000;
    flash_read_locked(time_record_addr, 4, (uint8_t *)&time);

    if (time)
    {
        DBG_DIRECT("test boot: 0x%x", time);
        time = sub_count(time);
        bool ret = flash_sw_protect_unlock_by_addr_locked(time_record_addr, &lock_level);
        DBG_DIRECT("unlock flash BP! ret=%d, old_bp_level=%d", ret, lock_level);
        flash_write_locked(time_record_addr, 4, (uint8_t *)&time);

        if ((time & 0xffff) != 0)
        {
            wdg_config_resetall();
            DBG_DIRECT("Warning!!! Not Reset ALL in boot");
        }
        else
        {
            wdg_config_reset_except_AON();
            DBG_DIRECT("Warning!!! Not Reset except_AON in boot");
        }
    }
    else
    {
        DBG_DIRECT("[watch dog] boot test done");;
    }
}
void reset_test_dlps_enter1(void)
{
    //read test time
    uint32_t time;
    uint32_t time_record_addr = 0x800004;
    time = flash_auto_read(time_record_addr | FLASH_OFFSET_TO_NO_CACHE);
    if (time)
    {
        DBG_DIRECT("test dlps_enter1: 0x%x", time);
        time = sub_count(time);
        extern void flash_auto_write(uint32_t start_addr, uint32_t data);
        time_record_addr = time_record_addr | FLASH_OFFSET_TO_NO_CACHE;
        flash_auto_write(time_record_addr, time);
        if ((time & 0xffff) != 0)
        {
            wdg_config_resetall();
            DBG_DIRECT("Warning!!! Not Reset ALL in dlps_enter1");
        }
        else
        {
            wdg_config_reset_except_AON();
            DBG_DIRECT("Warning!!! Not Reset except_AON in dlps_enter1");
        }
    }
    else
    {
        DBG_DIRECT("[watch dog] dlps_enter1 test done");;
    }
}

void reset_test_dlps_enter2(void)
{
    //read test time
    uint32_t time;
    uint32_t time_record_addr = 0x800008;
    time = flash_auto_read(time_record_addr | FLASH_OFFSET_TO_NO_CACHE);
    if (time)
    {
        DBG_DIRECT("test dlps_enter2: 0x%x", time);
        time = sub_count(time);
        time_record_addr = time_record_addr | FLASH_OFFSET_TO_NO_CACHE;
        flash_auto_write(time_record_addr, time);
        if ((time & 0xffff) != 0)
        {
            wdg_config_resetall();
            DBG_DIRECT("Warning!!! Not Reset ALL in dlps_enter2");
        }
        else
        {
            wdg_config_reset_except_AON();
            DBG_DIRECT("Warning!!! Not Reset except_AON in dlps_enter2");
        }
    }
    else
    {
        DBG_DIRECT("[watch dog] dlps_enter2 test done");;
    }
}
void reset_test_dlps_enter3(void)
{
    //read test time
    uint32_t time;
    uint32_t time_record_addr = 0x80000C;
    time = flash_auto_read(time_record_addr | FLASH_OFFSET_TO_NO_CACHE);

    if (time)
    {
        DBG_DIRECT("test dlps_enter3: 0x%x", time);
        time = sub_count(time);
        time_record_addr = time_record_addr | FLASH_OFFSET_TO_NO_CACHE;
        flash_auto_write(time_record_addr, time);

        if ((time & 0xffff) != 0)
        {
            wdg_config_resetall();
//            DBG_DIRECT("Warning!!! Not Reset ALL in dlps_enter3");
        }
        else
        {
            wdg_config_reset_except_AON();
//            DBG_DIRECT("Warning!!! Not Reset except_AON in dlps_enter3");
        }
    }
    else
    {
        DBG_DIRECT("[watch dog] dlps_enter3 test done");;
    }
}
void reset_test_dlps_exit1(void)
{
    //read test time
    uint32_t time;
    uint32_t time_record_addr = 0x800010;
    flash_read_locked(time_record_addr, 4, (uint8_t *)&time);
    if (time)
    {
        DBG_DIRECT("test dlps_exit1: 0x%x", time);
        time = sub_count(time);
        time_record_addr = time_record_addr | FLASH_OFFSET_TO_NO_CACHE;
        flash_write_locked(time_record_addr, 4, (uint8_t *)&time);

        if ((time & 0xffff) != 0)
        {
            wdg_config_resetall();
            DBG_DIRECT("Warning!!! Not Reset ALL in dlps_exit1");
        }
        else
        {
            wdg_config_reset_except_AON();
            DBG_DIRECT("Warning!!! Not Reset except_AON in dlps_exit1");
        }
    }
    else
    {
        DBG_DIRECT("[watch dog] dlps_exit1 test done");;
    }

}
void reset_test_dlps_exit2(void)
{
    //read test time
    uint32_t time;
    uint32_t time_record_addr = 0x800014;
    flash_read_locked(time_record_addr, 4, (uint8_t *)&time);
    if (time)
    {
        DBG_DIRECT("test dlps_exit2: 0x%x", time);
        time = sub_count(time);
        time_record_addr = time_record_addr | FLASH_OFFSET_TO_NO_CACHE;
        flash_write_locked(time_record_addr, 4, (uint8_t *)&time);

        if ((time & 0xffff) != 0)
        {
            wdg_config_resetall();
            DBG_DIRECT("Warning!!! Not Reset ALL in dlps_exit2");
        }
        else
        {
            wdg_config_reset_except_AON();
            DBG_DIRECT("Warning!!! Not Reset except_AON in dlps_exit2");
        }
    }
    else
    {
        DBG_DIRECT("[watch dog] dlps_exit2 test done");;
    }
}
void reset_test_dlps_exit3(void)
{
    //read test time
    uint32_t time;
    uint32_t time_record_addr = 0x80001C;
    time = flash_auto_read(time_record_addr | FLASH_OFFSET_TO_NO_CACHE);
    if (time)
    {
        DBG_DIRECT("test dlps_exit3: 0x%x", time);
        time = sub_count(time);
        time_record_addr = time_record_addr | FLASH_OFFSET_TO_NO_CACHE;
        flash_auto_write(time_record_addr, time);

        if ((time & 0xffff) != 0)
        {
            wdg_config_resetall();
//            DBG_DIRECT("Warning!!! Not Reset ALL in dlps_exit3");
        }
        else
        {
            wdg_config_reset_except_AON();
//            DBG_DIRECT("Warning!!! Not Reset except_AON in dlps_exit3");
        }
    }
    else
    {
        DBG_DIRECT("[watch dog] dlps_exit3 test done");;
    }
}
void reset_test_ble_scan(void)
{
    //read test time
    uint32_t time;
    uint32_t time_record_addr = 0x800020;
    flash_read_locked(time_record_addr, 4, (uint8_t *)&time);

    if (time)
    {
        DBG_DIRECT("test ble_scan: 0x%x", time);
        time = sub_count(time);
        flash_sw_protect_unlock_by_addr_locked(time_record_addr, &lock_level);
        flash_write_locked(time_record_addr, 4, (uint8_t *)&time);

        if ((time & 0xffff) == 0)
        {
            wdg_config_resetallall_1ms();
//            DBG_DIRECT("Warning!!! Not Reset ALL in ble_scan");
        }
        else
        {
            wdg_config_resetAON_1ms();
//            DBG_DIRECT("Warning!!! Not Reset except_AON in ble_scan");
        }
    }
    else
    {
        DBG_DIRECT("[watch dog] ble_scan test done");;
    }
}
void reset_test_ble_adv(void)
{
    //read test time
    uint32_t time;
    uint32_t time_record_addr = 0x800024;
    flash_read_locked(time_record_addr, 4, (uint8_t *)&time);
    if (time)
    {
        DBG_DIRECT("test ble_adv: 0x%x", time);
        time = sub_count(time);
        flash_sw_protect_unlock_by_addr_locked(time_record_addr, &lock_level);
        flash_write_locked(time_record_addr, 4, (uint8_t *)&time);

        if ((time & 0xffff) == 0)
        {
            wdg_config_resetallall_1ms();
//            DBG_DIRECT("Warning!!! Not Reset ALL in ble_adv");
        }
        else
        {
            wdg_config_resetAON_1ms();
//            DBG_DIRECT("Warning!!! Not Reset except_AON in ble_adv");
        }
    }
    else
    {
        DBG_DIRECT("[watch dog] ble_adv test done");;
    }
}
void reset_test_over(void)
{
    __disable_irq();
    flash_erase_locked(FLASH_ERASE_SECTOR, 0x1800000);
    __enable_irq();
    DBG_DIRECT("[watch dog] RESET TEST OVER ONCE");
    wdg_config_resetall();
}

void wdg_config_and_en_1ms(void)
{
    WDG_ClockEnable();
    WDG_Config(1, 3, RESET_ALL);  // ~ 1 ms
    WDG_Enable();
    while (1); /* wait until reset */
}

void wdg_config_and_en_60us(void)
{
    WDG_ClockEnable();
    //WDG_Config(1, 0, RESET_CORE_DOMAIN);   // change by yurui
    WDG_Config(1, 0, RESET_ALL);
    WDG_Enable();
    while (1); /* wait until reset */
}

//case 1: flash write/erase (limit time)
bool reset_with_flash_write(void)
{
    //read test time
    uint32_t time;
    uint32_t time_record_addr = 0x800030;
    flash_read_locked(time_record_addr, 4, (uint8_t *)&time);  // max 32 times
    if (time)
    {
        DBG_DIRECT("test1 time remain: 0x%x", time);
        time = sub_count(time);
        flash_sw_protect_unlock_by_addr_locked(time_record_addr, &lock_level);
        flash_write_locked(time_record_addr, 4, (uint8_t *)&time);

        //test
        if ((time & 0xffff) == 0)
        {
            DBG_DIRECT("test flash erase + reset");

            __disable_irq();
            wdg_config_and_en_1ms();
            flash_erase_locked(FLASH_ERASE_SECTOR, OTA_TMP_ADDR);
            __enable_irq();

            DBG_DIRECT("Warning!!! Not reset until erase done");
        }
        else
        {
            DBG_DIRECT("test flash write + reset");

            __disable_irq();
            wdg_config_and_en_1ms();
            flash_write_locked(OTA_TMP_ADDR, 4096, (uint8_t *)0x200000);
            __enable_irq();

            DBG_DIRECT("Warning!!! Not reset until write done");
        }

        return true;
    }
    else
    {
        DBG_DIRECT("[watch dog] flash test1 done");
        return false;
    }
}

//case 2: flash try high speed
bool reset_with_flash_high_speed(void)
{
    uint32_t time;
    uint32_t time_record_addr = 0x800034;
    flash_read_locked(time_record_addr, 4, (uint8_t *)&time);
    if (time)
    {
        DBG_DIRECT("test2 time remain: 0x%x", time);
        time = sub_count(time);
        flash_sw_protect_unlock_by_addr_locked(time_record_addr, &lock_level);
        flash_write_locked(time_record_addr, 4, (uint8_t *)&time);

        //test
        __disable_irq();
        wdg_config_and_en_60us();
        for (uint32_t i = 0; i < 1; ++i)
        {
            flash_try_high_speed(FLASH_MODE_4BIT);
        }
        __enable_irq();

        DBG_DIRECT("Warning!!! Not reset until switch to high speed done");

        return true;
    }
    else
    {
        DBG_DIRECT("[watch dog] flash test2 done");
        return false;
    }
}

#define TEST_DMA_LENGTH             (1024)

void flash_dma_done_CB(void)
{
    DBG_DIRECT("[watch dog] dma transfer done");
}

uint8_t ram_test[TEST_DMA_LENGTH];

//case 3: flash auto mode + dma
bool reset_with_flash_auto_dma(void)
{
    uint32_t time;
    uint32_t time_record_addr = 0x800038;
    flash_read_locked(time_record_addr, 4, (uint8_t *)&time);
    if (time)
    {
        DBG_DIRECT("test3 time remain: 0x%x", time);
        time = sub_count(time);
        flash_sw_protect_unlock_by_addr_locked(time_record_addr, &lock_level);
        flash_write_locked(time_record_addr, 4, (uint8_t *)&time);

        //test
        wdg_config_and_en_1ms();

        uint32_t src_addr = 0x180E000;
        uint32_t dest_addr = (uint32_t)&ram_test;
        flash_auto_dma_read_locked(FLASH_DMA_AUTO_F2R, flash_dma_done_CB, src_addr, dest_addr,
                                   TEST_DMA_LENGTH);

        DBG_DIRECT("Warning!!! Not reset until flash dma read done");

        return true;
    }
    else
    {
        DBG_DIRECT("[watch dog] flash test3 done");
        return false;
    }
}

//case 4: flash split read
bool reset_with_flash_split_dma(void) DATA_RAM_FUNCTION;
bool reset_with_flash_split_dma(void)
{
    uint32_t time;
    uint32_t time_record_addr = 0x80003C;
    flash_read_locked(time_record_addr, 4, (uint8_t *)&time);
    if (time)
    {
        DBG_DIRECT("test4 time remain: 0x%x", time);
        time = sub_count(time);
        flash_sw_protect_unlock_by_addr_locked(time_record_addr, &lock_level);
        flash_write_locked(time_record_addr, 4, (uint8_t *)&time);

        //test
        wdg_config_and_en_60us();

        uint32_t src_addr = 0x180E000;
        uint32_t dest_addr = (uint32_t)&ram_test;
        flash_split_read(src_addr, TEST_DMA_LENGTH, (uint8_t *)dest_addr);
//        extern void flash_dma_init(T_FLASH_DMA_TYPE dma_type, uint32_t src_addr, uint32_t dst_addr,
//                    uint32_t data_len);
//        flash_dma_init(FLASH_DMA_USER_F2R, src_addr, dest_addr, TEST_DMA_LENGTH);
//        flash_split_read_by_dma_locked(flash_dma_done_CB, src_addr, dest_addr, TEST_DMA_LENGTH);

        DBG_DIRECT("Warning!!! Not reset until flash split dma done");

        return true;
    }
    else
    {
        DBG_DIRECT("[watch dog] flash test4 done");
        return false;
    }
}

//case 5: flash read + cache enable
bool reset_with_flash_read_with_cache(void)
{
    uint32_t time;
    uint32_t time_record_addr = 0x800040;
    flash_read_locked(time_record_addr, 4, (uint8_t *)&time);
    if (time)
    {
        DBG_DIRECT("test5 time remain: 0x%x", time);
        time = sub_count(time);
        flash_sw_protect_unlock_by_addr_locked(time_record_addr, &lock_level);
        flash_write_locked(time_record_addr, 4, (uint8_t *)&time);

        //test
        wdg_config_and_en_60us();

        //cache_flush                              0x00008fc1   Thumb Code
        cache_flush();
        uint32_t src_addr = 0x80E000;  //cacheable address
        uint32_t dest_addr = (uint32_t)&ram_test;
        flash_split_read(src_addr, TEST_DMA_LENGTH, (uint8_t *)dest_addr);

        DBG_DIRECT("Warning!!! Not reset until flash read with cache done");

        return true;
    }
    else
    {
        DBG_DIRECT("[watch dog] flash test5 done");
        return false;
    }
}
