
#ifndef LIB_VC31_HCI_VC31HCI_H_
#define LIB_VC31_HCI_VC31HCI_H_

#include <stdint.h>
#include <stdbool.h>

//Read only.
#define VC31_DEV_ID                 (0x00)
#define VC31_DEV_ID_SIZE            (1)
#define VC31_STATUS                 (0x01)
#define VC31_STATUS_SIZE            (1)
#define VC31_PPG                        (0x02)
#define VC31_PPG_SIZE               (2)
#define VC31_CURRENT                    (0x04)
#define VC31_CURRENT_SIZE           (2)
#define VC31_PRE                        (0x06)
#define VC31_PRE_SIZE               (2)
#define VC31_PS                     (0x08)
#define VC31_PS_SIZE                (2)
#define VC31_ENV                        (0x0A)
#define VC31_ENV_SIZE               (2)

//Read or write.
#define VC31_CTRL                       (0x20)
#define VC31_CTRL_SIZE          (1)
#define VC31_PPG_DIV                    (0x21)
#define VC31_PPG_DIV_SIZE           (2)
#define VC31_PS_DIV                 (0x23)
#define VC31_PS_DIV_SIZE            (1)
#define VC31_TIA_WAIT                   (0x24)
#define VC31_TIA_WAIT_SIZE      (1)
#define VC31_AMP_WAIT                   (0x25)
#define VC31_AMP_WAIT_SIZE      (1)
#define VC31_GREEN_WAIT             (0x26)
#define VC31_GREEN_WAIT_SIZE        (1)
#define VC31_GREEN_IR_GAP               (0x27)
#define VC31_GREEN_IR_GAP_SIZE  (1)
#define VC31_IR_WAIT                    (0x28)
#define VC31_IR_WAIT_SIZE           (1)
#define VC31_GREEN_ADJ              (0x29)
#define VC31_GREEN_ADJ_SIZE     (2)

// Bit fields for VC31_STATUSs.
#define VC31_STATUS_D_ENV_OK            (0x01 << 4)
#define VC31_STATUS_D_PS_OK         (0x01 << 3)
#define VC31_STATUS_D_PRE_OK            (0x01 << 2)
#define VC31_STATUS_D_CUR_OK            (0x01 << 1)
#define VC31_STATUS_D_PPG_OK            (0x01 << 0)

// Bit fields for VC31_CTRL.
#define VC31_CTRL_RESET_VALUE           0x03        // Default value for VC31_CTRL.
#define VC31_CTRL_MASK              0xF7        // Mask for VC31_CTRL.
#define VC31_CTRL_WORK_MODE         (0x01 << 7) // Enter work mode. Start sample.
#define VC31_CTRL_SLEEP_MODE            (0x00 << 7) // Enter sleep mode. 32k Crystal is closed. Only SPI/I2C is working.
#define VC31_CTRL_ENABLE_PPG            (0x01 << 6) // Enable green led sample.
#define VC31_CTRL_DISABLE_PPG           (0x00 << 6) // Disable green led sample.
#define VC31_CTRL_ENABLE_PRE            (0x01 << 5) // Enable pre sample.
#define VC31_CTRL_DISABLE_PRE           (0x00 << 5) // Disable pre sample.
#define VC31_CTRL_LDO_EXTERN            (0x01 << 4) // Extern LDO.
#define VC31_CTRL_LDO_INTERNAL      (0x00 << 4) // Internal LDO.
#define VC31_CTRL_INT_DIR_RAISING       (0x01 << 2) // Set this bit for initting the interrupt trigger as raising edge.
#define VC31_CTRL_INT_DIR_FALLING       (0x00 << 2) // Clear this bit for initting the interrupt trigger as falling edge.
#define VC31_CTRL_OPA_GAIN_12_5     (0x00 << 0) // OPA3 Gain 12.5.
#define VC31_CTRL_OPA_GAIN_25           (0x01 << 0) // OPA3 Gain 25.
#define VC31_CTRL_OPA_GAIN_50           (0x02 << 0) // OPA3 Gain 50.
#define VC31_CTRL_OPA_GAIN_100      (0x03 << 0) // OPA3 Gain 100.

// PPG_DIV
#define VC31_PPG_DIV_10_HZ          (0x0CC5)
#define VC31_PPG_DIV_12_5_HZ            (0x0A35)
#define VC31_PPG_DIV_25_HZ          (0x0516)
#define VC31_PPG_DIV_50_HZ          (0x0287)
#define VC31_PPG_DIV_100_HZ         (0x013F)
#define VC31_PPG_DIV_1000_HZ            (0x0018)

// Bit fields for VC31_GREEN_ADJ
#define VC31_GREEN_ADJ_RESET_VALUE  0x0000          // Default value for VC31_GREEN_ADJ.
#define VC31_GREEN_ADJ_MASK         0xFFFF          // Mask for VC31_GREEN_ADJ.
#define VC31_GREEN_ADJ_ENABLE           (0x0001 << 15)  // Enable current adjust.
#define VC31_GREEN_ADJ_DISABLE      (0x0000 << 15)  // Disable current adjust.
#define VC31_GREEN_ADJ_UP               (0x0001 << 14)  // Turn up the current.
#define VC31_GREEN_ADJ_DOWN         (0x0000 << 14)  // Turn down the current.
#define VC31_GREEN_ADJ_VALUE_MASK       (0x3FFF)        // Mask for VC31_ADJ_CUR value.

#define VC31_SPI_WRITE_CMD          (0x06) // SPI write command.
#define VC31_SPI_READ_CMD               (0x03) // SPI read command.

#define VC31_I2C_SLAVE_ADDRESS_0        (0x66) // SA1 :0 ;SA0 :0;
#define VC31_I2C_SLAVE_ADDRESS_1        (0x6E) // SA1 :0 ;SA0 :1;
#define VC31_I2C_SLAVE_ADDRESS_2        (0xA6) // SA1 :1 ;SA0 :0;
#define VC31_I2C_SLAVE_ADDRESS_3        (0xAE) // SA1 :1 ;SA0 :1;
#define VC31_I2C_SLAVE_ADDRESS      VC31_I2C_SLAVE_ADDRESS_1 // VC31 I2C address.

#define VC31_ADJUST_FACTOR_INCREASE     (22) // 1.4 << 4 = 22.4
#define VC31_ADJUST_FACTOR_DECREASE     (11) // 0.7 << 4 = 11.2
#define VC31_ADJUST_FACTOR_MAX      1536000
#define VC31_ADJUST_FACTOR_MIN      15360
#define VC31_ADJUST_STEP_MAX            1000
#define VC31_ADJUST_STEP_MIN            2

#define VC31_ENV_LIMIT          2500
#define VC31_PS_LIMIT               150//350

#define VC31_PPG_LIMIT_L            200
#define VC31_PPG_LIMIT_H            3900
#define VC31_CURRENT_LIMIT_L            12
#define VC31_CURRENT_LIMIT_H            1000
#define VC31_UNWEAR_CNT         (3)
#define VC31_ISWEAR_CNT         (1)

#define VC31_WEAR_STATUS_DETECTION_OFF  (0)
#define VC31_WEAR_STATUS_DETECTION_ON       (1)
#define VC31_WEAR_STATUS_DETECTION_MODE VC31_WEAR_STATUS_DETECTION_ON


#define VC3120kLength      15
#define VC3120kVecDiff     20
#define VC3120kFmin        15000
#define VC3120kFmax        30000


typedef enum
{
    AdjustDirection_Null    = 0,
    AdjustDirection_Up      = 1,
    AdjustDirection_Down    = 2,
} VC31AdjustDirection;

typedef struct
{
    VC31AdjustDirection directionLast;
    VC31AdjustDirection directionLastBefore;
    int32_t step;
} VC31AdjustInfo_t;

typedef enum
{
    wearST_unWear   = 0,
    wearST_isWear   = 1,
} wearStatus_e;

typedef enum
{
    VC31Ret_isOK                = 0,
    VC31Ret_isWearToUnWear  = 1,
    VC31Ret_unWearToIsWear  = 2,
    VC31Ret_isERR               = -1,
} VC31Ret_t;

typedef enum
{
    VC31Mode_NormalWork       = 0,
    VC31Mode_CrossTalkTest    = 1,
} VC31Mode_t;

typedef enum
{
    VC31Test_Failed           = -1,
    VC31Test_Waiting          = 0,
    VC31Test_Passed           = 1,
} VC31TestResult_t;

typedef struct
{
    VC31TestResult_t  testResult;
    int8_t              testPassCnt;
    int8_t              testFailCnt;
} VC31TestInfo_t;

typedef struct
{
    uint16_t        ppgValue;   //PPG sample value.
    uint16_t        currentValue;//Current Sample value.
    uint16_t        psValue;    //PS Sample value.
    uint16_t        preValue;   //Environment Sample value.
    uint16_t        envValue;   //Environment Sample value.
} VC31SampleData_t;

typedef struct
{
    uint8_t             ctrl;
    wearStatus_e        wearStatus;
    int32_t             unWearCnt;
    int32_t             isWearCnt;
    VC31SampleData_t    sampleData;
    VC31AdjustInfo_t    adjustInfo;
    bool                isRunning;
    VC31Mode_t        workMode;
    VC31TestInfo_t    testInfo;
} VC31_t;


/***************************************************************************//**
 * @brief
 *   Init the VC31.
 *
 ******************************************************************************/
VC31Ret_t VC31Init(VC31_t *pVC31, VC31Mode_t workMode);
/***************************************************************************//**
 * @brief
 *   Get sample datas from VC31.
 *
 ******************************************************************************/
VC31Ret_t VC31GetSampleValues(VC31_t *pVC31);
/***************************************************************************//**
* @brief
*   Manual start sample of VC31.
*
******************************************************************************/
VC31Ret_t VC31StartSample(VC31_t *pVC31);
/***************************************************************************//**
* @brief
*   Manual stop sample of VC31.
*
******************************************************************************/
VC31Ret_t VC31StopSample(VC31_t *pVC31);
bool VC31OscCheck(uint32_t mcuOsc, VC31_t *pVC31, uint8_t VC31SampleRate);
extern uint32_t VC31GetTimeIntervalFromMCU(void);
extern VC31Ret_t VC31ReadRegisters(uint8_t startAddress, uint8_t *pRegisters, uint8_t len);
extern VC31Ret_t VC31WriteRegisters(uint8_t startAddress, uint8_t *pRegisters, uint8_t len);

typedef VC31Ret_t (* vc31_read_cb)(uint8_t startAddress, uint8_t *pRegisters, uint8_t len);
typedef VC31Ret_t (* vc31_write_cb)(uint8_t startAddress, uint8_t *pRegisters, uint8_t len);
typedef uint32_t (* vc31_gettime_cb)(void);
#endif /* LIB_VC31_HCI_VC31HCI_H_ */
