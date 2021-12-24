/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      dataTrans_queue.h
* @brief    queue for receive data from IO peripheral.
* @details
* @author    elliot chen
* @date      2017-04-27
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef __DATATRANS_QUEUE_H
#define __DATATRANS_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "string.h"
#include "trace.h"

/* Defines ------------------------------------------------------------------*/
#define LOOP_QUEUE_MAX_SIZE             (1024)
#define QUEUE_CAPABILITY                (LOOP_QUEUE_MAX_SIZE-1)
/**
 * @brief Enable print log or not
 */

#ifdef PRINT_DATATRANS_QUEUE_LOG
#define DATATRANS_QUEUE_BUFFER(MODULE, LEVEL, pFormat, para_num,...) DBG_BUFFER_##LEVEL(TYPE_BEE2, SUBTYPE_FORMAT, MODULE, pFormat, para_num, ##__VA_ARGS__)
#else
#define DATATRANS_QUEUE_BUFFER(MODULE, LEVEL, pFormat, para_num,...) ((void)0)
#endif

/**
 * @brief write Loop queue function type
 */

typedef void (*WriteQueueFnCB)(void *, void *, uint16_t);

/**
  * @brief  write queue status
  */
typedef enum
{
    QUEUE_WRITE_OK          = 0,
    QUEUE_WRITE_OVERFLOW    = 1,
    QUEUE_WRTIE_CB_NULL     = 2
} QUEUE_WRITE_STAT;


/**
 * @brief Loop queue data struct
 */
typedef struct
{
    volatile uint16_t   readIndex;          /* index of read queue */
    volatile uint16_t   writeIndex;         /* index of write queue */
    volatile bool       overFlow;           /* loop queue overflow or not */
    uint8_t buf[LOOP_QUEUE_MAX_SIZE];       /* Buffer for loop queue */
} LoopQueue_TypeDef;

/**
  * @brief Exported function
  */
void DataTrans_RegisterPeriphReadFnCB(WriteQueueFnCB pFunc);
void DataTrans_InitQueue(void);
QUEUE_WRITE_STAT DataTrans_WriteQueue(void *peri, uint16_t len);
uint16_t DataTrans_GetFIFOLen(void);
void DataTrans_ReadQueue(uint8_t *pBuf, uint16_t len);
void DataTrans_CopyBuf(uint8_t *pBuf, uint16_t len);
uint16_t DataTrans_FindData(uint8_t data, uint16_t len);
bool DataTrans_QueueIsEmpty(void);

#ifdef __cplusplus
}
#endif

#endif /*__DATATRANS_QUEUE_H*/

/******************* (C) COPYRIGHT 2017 Realtek Semiconductor Corporation *****END OF FILE****/

