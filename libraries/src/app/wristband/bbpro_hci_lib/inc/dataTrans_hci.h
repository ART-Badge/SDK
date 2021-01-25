/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      dataTrans_hci.h
* @brief
* @details
* @author    elliot chen
* @date      2018-08-14
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef __DATATRANS_HCI_H
#define __DATATRANS_HCI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "dataTrans_queue.h"
#include "dataTrans_uart.h"

/* Defines ------------------------------------------------------------------*/
#define HCI_CMD_PACK_PAYLOAD_MAX_SIZE               (200)

#pragma pack (1)
/**
  * @brief HCI command packet data structure
  */

typedef struct
{
    uint8_t     sync_word;
    uint8_t     seqn;
    uint16_t    len;
    uint16_t    opcode;
    uint8_t     payload[HCI_CMD_PACK_PAYLOAD_MAX_SIZE];
    uint8_t     check_sum;
} HCI_PackTypeDef;

/**
  * @brief HCI transmission layer data structure
  */
typedef struct
{
    HCI_PackTypeDef hci_recv_pack;
    uint8_t hci_tx_seqn;
    uint8_t hci_rx_seqn;
} HCI_TransInfoTypeDef;
#pragma pack ()

/**
 * @brief Send respone data Msg to task function type
 */

typedef void (*HCI_EVENT_CB)(HCI_PackTypeDef *pPacket);


/**
  * @brief  HCI exported function
  */

void hci_retrans_detect_timer_stop(void);
void DataTrans_hci_data_struct_init(void);
void DataTrans_AssemblePacket(uint8_t *pSendBuf, uint16_t opcode,
                              uint8_t *payLoadBuf, uint16_t len);
void DataTrans_send(uint8_t *pbuf, uint16_t len);

bool DataTrans_HandleRX(void);
#ifdef __cplusplus
}
#endif

#endif /*__DATATRANS_HCI_H*/

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

