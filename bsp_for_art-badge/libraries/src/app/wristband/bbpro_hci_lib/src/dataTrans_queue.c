/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file      dataTrans_queue.c
* @brief    This file provides queue for receving data .
* @details
* @author  elliot_chen
* @date     2017-04-27
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "dataTrans_queue.h"

/* Internal defines -----------------------------------------------------------*/
#define LOOP_QUEUE_DISCARD_MAX_SIZE         ((uint8_t)16)

/* Globals -------------------------------------------------------------------*/
/* Loop queue management data structure */
LoopQueue_TypeDef   LoopQueue;
/* Loop queue write callback function pointer */
WriteQueueFnCB LoopQueue_WriteFnCB = NULL;

/**
  * @brief  Initializes loop queue to their default reset values.
  * @param  pPacket: point to loop queue data structure.
  * @retval None
  */
static void LoopQueue_Init(LoopQueue_TypeDef *pQueueStruct)
{
    memset(pQueueStruct, 0, sizeof(LoopQueue_TypeDef));
}

/**
  * @brief  check loop queue if will overflow or not.
  * @param  pQueueStruct: point to loop queue dta struct.
  * @retval The new state of loop queue (overflow:TRUE, not overflow:FALSE).
  */
static bool LoopQueue_IsOverFlow(LoopQueue_TypeDef *pQueueStruct, uint16_t write_size)
{
    /* Check queue status */
    if (pQueueStruct->writeIndex >= pQueueStruct->readIndex)
    {
        if ((pQueueStruct->writeIndex + write_size) >= (LOOP_QUEUE_MAX_SIZE + pQueueStruct->readIndex))
        {
            DATATRANS_QUEUE_BUFFER(MODULE_APP, LEVEL_ERROR, "Loop Queue will OverFlow!", 0);
            pQueueStruct->overFlow = true;
            return true;
        }
    }
    else
    {
        if ((pQueueStruct->writeIndex + write_size) >= pQueueStruct->readIndex)
        {
            DATATRANS_QUEUE_BUFFER(MODULE_APP, LEVEL_ERROR, "Loop Queue will OverFlow!", 0);
            pQueueStruct->overFlow = true;
            return true;
        }
    }
    return false;
}

/**
  * @brief  Get valid data length.
  * @param None.
  * @retval None.
  */
static uint16_t LoopQueue_GetDataLen(LoopQueue_TypeDef *pQueueStruct)
{
    return ((LOOP_QUEUE_MAX_SIZE + pQueueStruct->writeIndex - pQueueStruct->readIndex) &
            QUEUE_CAPABILITY);
}

/**
  * @brief  check loop queue if empty or not.
  * @param  pPacket: point to IR packet struct.
  * @retval The new state of loop queue (empty:true, not empty:false).
  */
static bool LoopQueue_IsEmpty(LoopQueue_TypeDef *pQueueStruct)
{
    return (pQueueStruct->writeIndex == pQueueStruct->readIndex);
}

/**
  * @brief  Write buffer data to loop queue.
  * @param None.
  * @retval status of loop queue:
    TRUE: write success.
    FALSE: write failure.
  */
static QUEUE_WRITE_STAT LoopQueue_WriteBuf(LoopQueue_TypeDef *pQueueStruct, void *peri,
                                           uint16_t len)
{
    uint16_t tail_len = 0;

    if (LoopQueue_WriteFnCB == NULL)
    {
        return QUEUE_WRTIE_CB_NULL;
    }

    if (!LoopQueue_IsOverFlow(pQueueStruct, len))
    {
        if (pQueueStruct->writeIndex + len <= LOOP_QUEUE_MAX_SIZE)
        {
            LoopQueue_WriteFnCB(peri, &(pQueueStruct->buf[pQueueStruct->writeIndex]), len);
            pQueueStruct->writeIndex += len;
        }
        else
        {
            tail_len = LOOP_QUEUE_MAX_SIZE - pQueueStruct->writeIndex;
            LoopQueue_WriteFnCB(peri, &(pQueueStruct->buf[pQueueStruct->writeIndex]), tail_len);

            pQueueStruct->writeIndex = 0;
            LoopQueue_WriteFnCB(peri, &(pQueueStruct->buf[pQueueStruct->writeIndex]), len - tail_len);
            pQueueStruct->writeIndex += (len - tail_len);
        }
        pQueueStruct->writeIndex &= QUEUE_CAPABILITY;
    }
    else
    {
        /* Discard data */
        uint8_t discard_buf[LOOP_QUEUE_DISCARD_MAX_SIZE];

        len = (len > LOOP_QUEUE_DISCARD_MAX_SIZE) ? LOOP_QUEUE_DISCARD_MAX_SIZE : len;
        LoopQueue_WriteFnCB(peri, discard_buf, len);
        DATATRANS_QUEUE_BUFFER(MODULE_APP, LEVEL_ERROR, "Discard bytes: %d!", 1, len);

        return QUEUE_WRITE_OVERFLOW;
    }

    return QUEUE_WRITE_OK;
}

/**
  * @brief  Read buffer data from loop queue.
  * @param None.
  * @retval None.
  */
static void LoopQueue_ReadBuf(LoopQueue_TypeDef *pQueueStruct, uint8_t *pBuf, uint16_t len)
{
    uint16_t remain = 0;

    /* Check parameters */
    if ((len == 0) || (pBuf == NULL))
    {
        return;
    }

    if (pQueueStruct->readIndex + len <= LOOP_QUEUE_MAX_SIZE)
    {
        memcpy(pBuf, &(pQueueStruct->buf[pQueueStruct->readIndex]), len);
        pQueueStruct->readIndex += len;
    }
    else
    {
        remain = LOOP_QUEUE_MAX_SIZE - pQueueStruct->readIndex;
        memcpy(pBuf, &(pQueueStruct->buf[pQueueStruct->readIndex]), remain);
        pQueueStruct->readIndex = 0;
        memcpy(pBuf + remain, &(pQueueStruct->buf[pQueueStruct->readIndex]), len - remain);
        pQueueStruct->readIndex = len - remain;
    }
    pQueueStruct->readIndex &= QUEUE_CAPABILITY;
}

/**
  * @brief  copy buffer data from loop queue.
  * @param None.
  * @retval None.
  */
static void LoopQueue_CopyData(LoopQueue_TypeDef *pQueueStruct, uint8_t *pBuf, uint16_t len)
{
    uint16_t readIndex = pQueueStruct->readIndex;
    uint16_t remain = 0;

    /* Check parameters */
    if ((len == 0) || (pBuf == NULL))
    {
        return;
    }

    if (readIndex + len <= LOOP_QUEUE_MAX_SIZE)
    {
        memcpy(pBuf, &(pQueueStruct->buf[readIndex]), len);
        readIndex += len;
    }
    else
    {
        remain = LOOP_QUEUE_MAX_SIZE - readIndex;
        memcpy(pBuf, &(pQueueStruct->buf[readIndex]), remain);
        readIndex = 0;
        memcpy(pBuf + remain, &(pQueueStruct->buf[readIndex]), len - remain);
    }
    readIndex &= QUEUE_CAPABILITY;
}

/**
 * @brief Register callback function for reading data from peripheral and write to loop queue.
 * @param pFunc callback function.
 * @return none
*/
void DataTrans_RegisterPeriphReadFnCB(WriteQueueFnCB pFunc)
{
    LoopQueue_WriteFnCB = pFunc;
}

/**
  * @brief  Initializes loop queue to their default reset values.
  * @param  None.
  * @retval None.
  */
void DataTrans_InitQueue(void)
{
    LoopQueue_Init(&LoopQueue);
}

/**
  * @brief  Check loop queue is empty or not.
  * @param None.
  * @retval status of loop queue.:
    TRUE: loop queue is empty.
    FALSE: loop queue is not empty.
  */
bool DataTrans_QueueIsEmpty(void)
{
    return LoopQueue_IsEmpty(&LoopQueue);
}

/**
  * @brief  Write buffer data to loop queue.
  * @param None.
 * @retval status of loop queue: QUEUE_WRITE_STAT type
  */
QUEUE_WRITE_STAT DataTrans_WriteQueue(void *peri, uint16_t len)
{

#if 1
    return LoopQueue_WriteBuf(&LoopQueue, peri, len);
#else
    uint16_t tail_len = 0;

    if (!LoopQueue_IsOverFlow(&LoopQueue, len))
    {
        if (LoopQueue.writeIndex + len <= LOOP_QUEUE_MAX_SIZE)
        {
            LoopQueue_WriteFnCB(peri, &(LoopQueue.buf[LoopQueue.writeIndex]), len);
            LoopQueue.writeIndex += len;
        }
        else
        {
            tail_len = LOOP_QUEUE_MAX_SIZE - LoopQueue.writeIndex;
            LoopQueue_WriteFnCB(peri, &(LoopQueue.buf[LoopQueue.writeIndex]), tail_len);

            LoopQueue.writeIndex = 0;
            LoopQueue_WriteFnCB(peri, &(LoopQueue.buf[LoopQueue.writeIndex]), len - tail_len);
            LoopQueue.writeIndex += (len - tail_len);
        }
        LoopQueue.writeIndex &= QUEUE_CAPABILITY;
    }
    else
    {
        /* Discard data */
        uint8_t discard_buf[LOOP_QUEUE_DISCARD_MAX_SIZE];

        len = (len > LOOP_QUEUE_DISCARD_MAX_SIZE) ? LOOP_QUEUE_DISCARD_MAX_SIZE : len;
        LoopQueue_WriteFnCB(peri, discard_buf, len);
        DATATRANS_QUEUE_BUFFER(MODULE_DRIVERTASK, LEVEL_ERROR, "Discard bytes: %d!", 1, len);

        return FALSE;
    }

    return TRUE;
#endif
}

/**
  * @brief  Get valid data length.
  * @param None.
  * @retval loop queue length.
  */
uint16_t DataTrans_GetFIFOLen(void)
{
    return LoopQueue_GetDataLen(&LoopQueue);
}

/**
  * @brief  Read data from loop queue.
  * @param None.
  * @retval None.
  */
void DataTrans_ReadQueue(uint8_t *pBuf, uint16_t len)
{
    return LoopQueue_ReadBuf(&LoopQueue, pBuf, len);
}

/**
  * @brief  copy buffer data from loop queue.
  * @param None.
  * @retval None.
  */
void DataTrans_CopyBuf(uint8_t *pBuf, uint16_t len)
{
    return LoopQueue_CopyData(&LoopQueue, pBuf, len);
}

/**
  * @brief  Find special data in loop queue.
  * @param data: Find data.
  * @param data: data length in Loop queue.
  * @retval actual data position in loop queue.
    0: no matching data found.
    1: first index in loop queue and so on.
  */
uint16_t DataTrans_FindData(uint8_t data, uint16_t len)
{
    uint16_t pos = LoopQueue.readIndex;

    while (len--)
    {
        pos &= QUEUE_CAPABILITY;

        if (data == LoopQueue.buf[pos++])
        {
            return (pos + LOOP_QUEUE_MAX_SIZE - LoopQueue.readIndex) & QUEUE_CAPABILITY;
        }
    }

    return 0;
}

/******************* (C) COPYRIGHT 2017 Realtek Semiconductor Corporation *****END OF FILE****/

