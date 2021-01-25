#ifndef _AUTO_K_RF_H_
#define _AUTO_K_RF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define FLT_MAX 40000

#define K_RF_SUCCESS               0
#define K_RF_READ_DRIFT_FAIL       1
#define K_RF_K_FAIL                2
#define K_RF_WRITE_CONFIG_FAIL     3

#define BASE_MAGNIFIED          1000000
#define BIT0                    0x00000001
#define BIT1        0x00000002
#define BIT2        0x00000004
#define BIT3        0x00000008
#define BIT4        0x00000010
#define BIT5        0x00000020
#define BIT6        0x00000040
#define BIT7        0x00000080
#define BIT8        0x00000100
#define BIT9        0x00000200
#define BIT10       0x00000400
#define BIT11       0x00000800
#define BIT12       0x00001000
#define BIT13       0x00002000
#define BIT14       0x00004000
#define BIT15       0x00008000
#define BIT16       0x00010000
#define BIT17       0x00020000
#define BIT18       0x00040000
#define BIT19       0x00080000
#define BIT20       0x00100000
#define BIT21       0x00200000
#define BIT22       0x00400000
#define BIT23       0x00800000
#define BIT24       0x01000000
#define BIT25       0x02000000
#define BIT26       0x04000000
#define BIT27       0x08000000
#define BIT28       0x10000000
#define BIT29       0x20000000
#define BIT30       0x40000000
#define BIT31       0x80000000

typedef unsigned char           BOOLEAN;
typedef void                    VOID;
typedef unsigned char           UCHAR;
typedef unsigned char           UINT8;
typedef unsigned short          UINT16;
typedef unsigned int            UINT32;
typedef unsigned long long      UINT64;
typedef signed char             CHAR;
typedef signed char             INT8;
typedef signed short            INT16;
typedef signed int              INT32;
typedef signed long long        INT64;

#define MODEM_PI_PAGE_0                 0
#define MODEM_PI_PAGE_1                 1
#define MODEM_PI_PAGE_2                 2
#define MODEM_PI_PAGE_3                 3
#define MODEM_PI_PAGE_4                 4
#define MODEM_PI_PAGE_5                 5
#define MODEM_PI_PAGE_6                 6
#define MODEM_PI_PAGE_7                 7

#define TRANS_MODEM_REG(x)      (((x) >> 1) | 0x40)  /* this is used to translate byte address to word address*/

extern UINT32(*rtk_read_modem_radio_reg_pi)(UCHAR modem_page, UCHAR addr, UCHAR type);
extern void (*rtk_write_modem_radio_reg_pi)(UCHAR modem_page, UCHAR addr, UCHAR type, UINT16 val);
extern UINT16(*rtk_ioq_read_rfc_reg)(UINT8);
extern void (*rtk_write_rfc_reg_pi)(UINT8, UINT16);
extern void (*rtk_update_rfc_reg_pi)(UINT8, UINT16, UINT16);

#define RTK_WRITE_RF_REG_PI(waddr, value)   rtk_write_rfc_reg_pi(waddr, value)
#define RTK_READ_RF_REG_PI(waddr)           rtk_ioq_read_rfc_reg(waddr)
#define RTK_UPDATE_RF_REG_PI(waddr, bm_mask, value)   rtk_update_rfc_reg_pi(waddr, bm_mask, value)
#define RTK_WRITE_MODEM_REG_PI(modem_page, waddr, value) \
    rtk_write_modem_radio_reg_pi(modem_page, waddr, TYPE_MODEM, value)
#define RTK_READ_MODEM_REG_PI(modem_page, waddr) rtk_read_modem_radio_reg_pi(modem_page, waddr, TYPE_MODEM)
#define RTK_UPDATE_MODEM_REG_PI(modem_page, waddr, bm_mask, value) \
    { \
        UINT16 temp = RTK_READ_MODEM_REG_PI(modem_page, waddr); \
        RTK_WRITE_MODEM_REG_PI(modem_page, waddr, (temp & ~(bm_mask)) | ((bm_mask) & (value))); \
    }

void init_dft(UINT8 rx_channel, UINT16 rx_gain, UINT16 dft_size, UINT16 pwr_avg_times,
              UINT8 reg_dft_sum_scale, UINT32 *reg08, UINT32 *reg0A);
void deinit_dft(UINT32 reg08, UINT32 reg0A);

double modem_dft(double IF_val, UINT32 *raw_data);

uint8_t Auto_K_RF(uint8_t rx_channel, uint32_t freq_drift_threshold, uint8_t *cal_xtal_result);

#ifdef __cplusplus
}
#endif

#endif /* _AUTO_K_RF_H_ */
