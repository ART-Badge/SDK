/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_alias.h
* \brief    The header file of all peripherals internal alias.
* \details  To be compatible with the previous driver,create an alias file for the previous driver.
* \author   yuan
* \date     2020-06-19
* \version  v2.1.0
* *********************************************************************************************************
*/


#ifndef _RTL876X_ALIAS_H_
#define _RTL876X_ALIAS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \cond        private
 * \brief       To be compatible with the previous driver.
 * \defgroup    ADC_InitTypeDef_Alias ADC Init TypeDef Alias
 * \{
 */
#define ADC_One_Shot_Mode           ADC_ONE_SHOT_MODE
#define schIndex                    ADC_SchIndex
#define bitmap                      ADC_Bitmap
#define adcSamplePeriod             ADC_SampleTime
#define PowerAlwaysOnEn             ADC_PowerAlwaysOnEn
#define dataWriteToFifo             ADC_DataWriteToFifo
#define timerTriggerEn              ADC_TimerTriggerEn

/**
 * \}
 * \endcond
 */

/**
 * \cond        private
 * \brief       To be compatible with the previous driver.
 * \defgroup    ADC_API_Alias ADC API Alias
 * \{
 */
#define ADC_ReadByScheduleIndex     ADC_ReadRawData
#define ADC_ReadFifoData            ADC_ReadFIFO
#define ADC_GetFifoData             ADC_ReadFIFOData
#define ADC_GetFifoLen              ADC_GetFIFODataLen
#define ADC_SchTableSet             ADC_BitMapConfig
#define ADC_PowerAlwaysOnCmd        ADC_ManualPowerOnCmd
#define ADC_FifoWriteCmd            ADC_WriteFIFOCmd
#define ADC_AnalogCircuitConfig     ADC_PowerSupplyConfig
#define ADC_GetIntFlagStatus        ADC_GetINTStatus
#define ADC_ClearFifo               ADC_ClearFIFO
#define ADC_GetIntStatus            ADC_GetAllFlagStatus

/**
 * \}
 * \endcond
 */

/**
 * \cond        private
 * \brief       To be compatible with the previous driver.
 * \defgroup    RTC_API_Alias RTC API Alias
 * \{
 */
#define RTC_SetComp                 RTC_SetCompValue
#define RTC_RunCmd                  RTC_Cmd
#define RTC_INT_CMP1                RTC_INT_COMP1

/**
 * \}
 * \endcond
 */

/**
 * \cond        private
 * \brief       To be compatible with the previous driver.
 * \defgroup    UART_InitTypeDef_Alias UART Init TypeDef Alias
 * \{
 */
#define ovsr_adj                    UART_OvsrAdj
#define div                         UART_Div
#define ovsr                        UART_Ovsr
#define wordLen                     UART_WordLen
#define parity                      UART_Parity
#define stopBits                    UART_StopBits
#define autoFlowCtrl                UART_HardwareFlowControl
#define txTriggerLevel              UART_TxThdLevel
#define rxTriggerLevel              UART_RxThdLevel
#define dmaEn                       UART_DmaEn
#define idle_time                   UART_IdleTime
#define TxWaterlevel                UART_TxWaterLevel
#define RxWaterlevel                UART_RxWaterLevel
#define TxDmaEn                     UART_TxDmaEn
#define RxDmaEn                     UART_RxDmaEn

#define UART_INT_LINE_STS           UART_INT_RX_LINE_STS
#define UART_INT_ID_RX_TMEOUT       UART_INT_ID_RX_DATA_TIMEOUT
#define UART_FLAG_THR_EMPTY         UART_FLAG_TX_FIFO_EMPTY
#define UART_FLAG_THR_TSR_EMPTY     UART_FLAG_TX_EMPTY
#define UART_FLAG_RX_DATA_RDY       UART_FLAG_RX_DATA_AVA

/**
 * \}
 * \endcond
 */

/**
 * \cond        private
 * \brief       To be compatible with the previous driver.
 * \defgroup    UART_API_Alias UART API Alias
 * \{
 */
#define UART_GetFlagState           UART_GetFlagStatus
#define UART_ChangeBaudRate         UART_SetBaudRate
#define UART_ChangeParams           UART_SetParams
#define UART_ClearTxFifo            UART_ClearTxFIFO
#define UART_ClearRxFifo            UART_ClearRxFIFO
#define UART_GetTxFIFOLen           UART_GetTxFIFODataLen
#define UART_GetRxFIFOLen           UART_GetRxFIFODataLen

/**
 * \}
 * \endcond
 */




#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_ALIAS_H_ */



/******************* (C) COPYRIGHT 2020 Realtek Semiconductor *****END OF FILE****/



