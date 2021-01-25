/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     dataTrans_hci.c
* @brief    This file provides hci protocol driver.
* @details
* @author   elliot_chen
* @date     2018-11-16
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "dataTrans_hci.h"

/* Internal defines ----------------------------------------------------------*/
#define HCI_PACK_CTRL_FILED_WIDTH           ((uint8_t)4)
#define HCI_PACK_MIN_WIDTH                  ((uint8_t)7)
#define HCI_PACK_MAX_SIZE                   ((uint16_t)(sizeof(HCI_PackTypeDef)))
#define HCI_SYNC_WORD                       ((uint8_t)0xAA)
#define HCI_PACK_SINGLE_DISACRD_SIZE        ((uint8_t)30)

/* HCI length field index */
#define HCI_PACKET_LEN_INDEX                ((uint8_t)2)
/* HCI sync word field width */
#define HCI_SYNC_WORD_WIDTH                 ((uint8_t)1)
/* HCI seqn field width */
#define HCI_SEQN_WIDTH                      ((uint8_t)1)
/* HCI length field width */
#define HCI_PACKET_LEN_WIDTH                ((uint8_t)2)
/* HCI opcode field length */
#define HCI_OPCODE_WIDTH                    ((uint8_t)2)
/* HCI check sum field length */
#define HCI_CHECK_SUM_WIDTH                 ((uint8_t)1)
/* HCI sync word, seqn, length and check sum total length */
#define HCI_HEADER_AND_CHECK_SUM_LEN        ((uint8_t)(HCI_SYNC_WORD_WIDTH + \
                                                       HCI_SEQN_WIDTH + \
                                                       HCI_PACKET_LEN_WIDTH + \
                                                       HCI_CHECK_SUM_WIDTH))

/* Gloabls -------------------------------------------------------------------*/
/* Store HCI transmission packet and control infomation */
HCI_TransInfoTypeDef hci_trans_info;

/**
  * @brief  initialize hci data struct which store hci information.
  * @param  none.
  * @retval none.
  */
void DataTrans_hci_data_struct_init(void)
{
    memset(&hci_trans_info, 0, sizeof(HCI_TransInfoTypeDef));
}

/**
  * @brief  check packet.
  * @param  dataPtr: buffer addr.
  * @param  len: buffer length.
  * @retval Check status of a entire packet.
  */
uint8_t DataTrans_calc_checksum(uint8_t *dataPtr, uint16_t len)
{
    uint8_t check_sum;

    check_sum = 0;
    while (len)
    {
        check_sum += *dataPtr;
        dataPtr++;
        len--;
    }
    return (0xff - check_sum + 1); //((~check_sum)+1);
}

/**
  * @brief  Check seqn field in HCI packet.
  * @param  pPacket: point to HCI packet struct.
  * @retval Check status of seqn field.
  */
static bool DataTrans_CheckSeqn(HCI_PackTypeDef *pPacket)
{
    bool check_seqn_flag = true;

    if (hci_trans_info.hci_rx_seqn == pPacket->seqn)
    {
        check_seqn_flag = false;
    }

    hci_trans_info.hci_rx_seqn = pPacket->seqn;
    return check_seqn_flag;
}

/**
  * @brief  Fill in data to HCI packet.
  * @param  pPacket: point to HCI packet struct.
  * @retval Check status of a entire packet.
  */
static bool DataTrans_CollectHCIPacket(HCI_PackTypeDef *pPacket, uint16_t len)
{
    uint8_t buf[HCI_PACK_MIN_WIDTH];

    memset(buf, 0, sizeof(buf));
    /* check entire packet or not */
    DataTrans_CopyBuf(buf, HCI_PACK_CTRL_FILED_WIDTH);

    /* Little endian */
    if ((pPacket->len = (buf[HCI_PACKET_LEN_INDEX + 1] << 8) + buf[HCI_PACKET_LEN_INDEX]) <=
        (len - HCI_HEADER_AND_CHECK_SUM_LEN))
    {
        DataTrans_ReadQueue(&(pPacket->sync_word), HCI_SYNC_WORD_WIDTH);
        DataTrans_ReadQueue(&(pPacket->seqn), HCI_SEQN_WIDTH);
        DataTrans_ReadQueue((uint8_t *) & (pPacket->len), HCI_PACKET_LEN_WIDTH);
        DataTrans_ReadQueue((uint8_t *) & (pPacket->opcode), HCI_OPCODE_WIDTH);

        if (pPacket->len >= HCI_OPCODE_WIDTH)
        {
            DataTrans_ReadQueue(pPacket->payload, pPacket->len - HCI_OPCODE_WIDTH);
        }
        else
        {
            return false;
        }
        DataTrans_ReadQueue(&(pPacket->check_sum), HCI_CHECK_SUM_WIDTH);

        /* Check sum field */
        if (pPacket->check_sum != DataTrans_calc_checksum(&(pPacket->seqn),
                                                          pPacket->len + HCI_SEQN_WIDTH + HCI_PACKET_LEN_WIDTH))
        {
            return false;
        }

        /* seqn filter */
        if (!DataTrans_CheckSeqn(pPacket))
        {
            memset(pPacket, 0, sizeof(HCI_PackTypeDef));
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;
}

/**
  * @brief  Get a entire HCI commandpacket.
  * @param  pPacket: point to HCI packet struct.
  * @retval The decoded status of a entire packet.
  */
static bool DataTrans_GetHCIPacket(HCI_PackTypeDef *pPacket, uint16_t len)
{
    uint16_t pos = 0;

    /* Check data length which can read */
    if (len > HCI_PACK_MAX_SIZE)
    {
        len = HCI_PACK_MAX_SIZE;
    }

    while (len)
    {
        /* Find header */
        pos = DataTrans_FindData(HCI_SYNC_WORD, len);

        /* Check packet */
        if (pos == 1)
        {
            bool ret = DataTrans_CollectHCIPacket(pPacket, len);

            //APP_PRINT_INFO3("[talkband][DataTrans_GetHCIPacket] ret = %d, pos = %d, len = %d!", ret, pos, DataTrans_GetFIFOLen());
            return ret;
        }
        else
        {
            /* Error handle: discard error data */
            uint8_t buf[HCI_PACK_SINGLE_DISACRD_SIZE];
            uint8_t discard_len = 0;

            if (pos > 1)
            {
                discard_len = ((pos - 1) > HCI_PACK_SINGLE_DISACRD_SIZE) ? HCI_PACK_SINGLE_DISACRD_SIZE : (pos - 1);
                DataTrans_ReadQueue(buf, discard_len);

                /* Update buffer length */
                if (len > discard_len)
                {
                    len -= discard_len;
                }
                else
                {
                    break;
                }
            }
            else
            {
                /* No header in current queue */
                discard_len = (len > HCI_PACK_SINGLE_DISACRD_SIZE) ? HCI_PACK_SINGLE_DISACRD_SIZE : len;
                DataTrans_ReadQueue(buf, discard_len);
                break;
            }
        }
    }

    return false;
}

/**
  * @brief  Parse dataTrans HCI packet.
  * @param  pPacket: point to HCI packet struct.
  * @retval The decoded status of a entire packet.
  */
bool DataTrans_Parse(HCI_PackTypeDef *pPacket)
{
    uint16_t len = DataTrans_GetFIFOLen();

    if (len >= HCI_PACK_MIN_WIDTH)
    {
        return DataTrans_GetHCIPacket(pPacket, len);
    }

    return false;
}

/**
  * @brief  Assemble HCI packet which to be sent.
  * @param  pSendBuf: buffer address whose data will need to send.
  * @param  opcode: command opcode.
  * @param  status: communication status.
  * @param  payLoadBuf: payload buffer address.
  * @retval Check status of a entire packet.
  */
void DataTrans_AssemblePacket(uint8_t *pSendBuf, uint16_t opcode,
                              uint8_t *payLoadBuf, uint16_t len)
{
    uint8_t *pBuf = pSendBuf + HCI_SYNC_WORD_WIDTH;
    uint16_t check_len = len + HCI_OPCODE_WIDTH + HCI_PACKET_LEN_WIDTH + HCI_SEQN_WIDTH;

    *pSendBuf++ = HCI_SYNC_WORD;
    *pSendBuf++ = (hci_trans_info.hci_tx_seqn++) & 0xFF;
    *pSendBuf++ = (len + HCI_OPCODE_WIDTH) & 0xFF;
    *pSendBuf++ = ((len + HCI_OPCODE_WIDTH) >> 8) & 0xFF;
    *pSendBuf++ = opcode & 0xFF;
    *pSendBuf++ = (opcode >> 8) & 0xFF;
    while (len--)
    {
        *pSendBuf++ = *payLoadBuf++;
    }
    *pSendBuf = DataTrans_calc_checksum(pBuf, check_len);
}

/**
 * @brief send packet by HCI layer.
 * @param pbuf: packet address.
 * @param len: packet length.
 * @return none
*/
void DataTrans_send(uint8_t *pbuf, uint16_t len)
{
    /* Check parameters */
    if ((len < HCI_PACK_MIN_WIDTH) || (len > HCI_PACK_MAX_SIZE))
    {
        APP_PRINT_ERROR1("[talkband] packet len = 0x%x!", len);
        return ;
    }

    if ((pbuf[4] == 0x00) && (pbuf[5] == 0x00))
    {
        APP_PRINT_INFO1("[talkband][->hci send ack]: %b", TRACE_BINARY(len, pbuf));
    }
    else
    {
        APP_PRINT_INFO1("[talkband][->hci send cmd]: %b", TRACE_BINARY(len, pbuf));
    }

    return DataTrans_uart_send(pbuf, len);
}

HCI_EVENT_CB bbpro_hci_event_cb __attribute__((weak)) = NULL;
/**
  * @brief  Fill in data to HCI packet.
  * @param  pPacket: point to HCI packet struct.
  * @retval Check status of a entire packet.
  */
bool DataTrans_HandleHCIPacket(HCI_PackTypeDef *pPacket)
{
    /* Parse packet */
    if (!DataTrans_Parse(pPacket))
    {
        return false;
    }

    /* Notify to upper layer */
    if (bbpro_hci_event_cb != NULL)
    {
        bbpro_hci_event_cb(pPacket);
    }
    return true;
}

bool DataTrans_HandleRX(void)
{
    return DataTrans_HandleHCIPacket(&(hci_trans_info.hci_recv_pack));
}

/******************* (C) COPYRIGHT 2018 Realtek Semiconductor Corporation *****END OF FILE****/

