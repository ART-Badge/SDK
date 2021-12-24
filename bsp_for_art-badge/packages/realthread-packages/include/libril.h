/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-01     zhangsz      Init first
 */

#ifndef __PDU_H__
#define __PDU_H__

#include <rtthread.h>

#if defined(PKG_USING_RIL)

#define SMS_PDU_MAX_LEN                     175
#define SMS_PDU_USER_DATA_MAX_LEN           140 /* 140 octets */
#define TRUNCATED_PDU_DATA_LEN              134
#define SMS_GSM7BIT_MAX_LEN                 160
#define TRUNCATED_GSM_DATA_LEN              153
#define ADDR_OCTET_MAX_LEN                  20 /* 10 Octets */
#define TIME_STAMP_OCTET_MAX_LEN            14 /* 7 Octets */
#define UTF8_CHAR_LEN                       4  /* 4 bytes */
#define GSM_7BIT                            0x00
#define ANSI_8BIT                           0x01
#define UCS2_16BIT                          0x02
#define USER_DATA_HEADER_INDICATION         0x40
#define STATUS_REPORT_INDICATOR             0x20
#define MSG_REF_NO_DEFAULT                  0x00
#define UDH_CONCATENATED_MSG_LEN            0x05
#define IE_CONCATENATED_MSG_LEN             0x03
#define MORE_MSG_TO_SEND                    0x04
#define TRUE                                1
#define FALSE                               0
#define LONG_SMS_TEXT_MAX_LEN               700

//###########################################################################
// @ENUMERATOR
//###########################################################################
/* Error Values */
enum
{
    ERR_MSG_TYPE = 0,
    ERR_CHAR_SET,
    ERR_PHONE_TYPE_OF_ADDR,
    ERR_PHONE_NUM_PLAN,
    ERR_PROTOCOL_ID,
    ERR_DATA_CODE_SCHEME
};

/* Message Type indication */
enum    
{
    MSG_TYPE_SMS_DELIVER = 0x00,
    MSG_TYPE_SMS_SUBMIT = 0x01,
    MSG_TYPE_SMS_STATUS_REPORT = 0x02
};

/* Type of Number */
enum
{
   NUM_TYPE_UNKNOWN = 0x00,
   NUM_TYPE_INTERNATIONAL = 0x01,
   NUM_TYPE_NATIONAL = 0x02,
   NUM_TYPE_ALPHANUMERIC = 0x05
};

/* Numbering Plan Indicator */
enum
{
   NUM_PLAN_UNKNOWN = 0x00,
   NUM_PLAN_ISDN = 0x01
};

/* Validity Period type */
enum
{
   VLDTY_PERIOD_DEFAULT = 0x00,
   VLDTY_PERIOD_RELATIVE = 0x10,
   VLDTY_PERIOD_ABSOLUTE = 0x18
};

/* User Data Header Information Element identifier */
enum
{
   IE_CONCATENATED_MSG = 0x00,
   IE_PORT_ADDR_8BIT = 0x04,
   IE_PORT_ADDR_16BIT = 0x05
};

/* Message State */
enum
{
    MSG_DELIVERY_FAIL = 0,
    MSG_DELIVERY_SUCCESS
};

//###########################################################################
// @DATATYPE
//###########################################################################
typedef struct
{
    rt_uint8_t day;
    rt_uint8_t month;
    rt_uint8_t year;

} DATE_DESC;

typedef struct
{
    rt_uint8_t hour;
    rt_uint8_t minute;
    rt_uint8_t second;

} TIME_DESC;


/* PDU Decode Descriptor */
typedef struct
{
    rt_uint8_t smscAddrLen;                         /* Length of Service Center Number  */
    rt_uint8_t smscNpi;                             /* Numbering Plan Indicactor */
    rt_uint8_t smscTypeOfAddr;                      /* Type of Address of Service Center Number */
    char smscAddr[ADDR_OCTET_MAX_LEN + 1];          /* Service Center Number */
    rt_uint8_t firstOct;                            /* First octet of PDU SMS */
    rt_uint8_t isHeaderPrsnt;                       /* User data header indicator */
    rt_uint8_t msgRefNo;                            /* Message Reference Number */
    
    rt_uint8_t phoneAddrLen;                        /* Lenght of Phone Number */
                    
    rt_uint8_t phoneTypeOfAddr;                     /* Type of Address of Phone Number */
    
    char phoneAddr[ADDR_OCTET_MAX_LEN + 1];         /* Phone Number */
    
    rt_uint8_t protocolId;                          /* Protocol Identifier */
    rt_uint8_t dataCodeScheme;                      /* Data Coding scheme */
    rt_uint8_t msgType;                             /* Message Type */
    rt_uint8_t isWapPushMsg;                        /* WAP-PUSH SMS */
    rt_uint8_t isFlashMsg;                          /* FLASH SMS */
    rt_uint8_t isStsReportReq;                      /* Staus Report Flag */
    rt_uint8_t isMsgWait;                           /* Message Waiting */
    
    rt_uint8_t usrDataFormat;                       /* User Data Coding Format */
    
    char timeStamp[TIME_STAMP_OCTET_MAX_LEN + 1];   /* Service Center Time Stamp */
    char dischrgTimeStamp[TIME_STAMP_OCTET_MAX_LEN + 1]; /* Discharge Time Stamp */
    
    rt_uint8_t vldtPrd;                             /* Validity Period */
    rt_uint8_t vldtPrdFrmt;                         /* Validity Period Format */
    
    rt_uint8_t usrDataLen;                          /* User Data Length */
    rt_uint8_t usrData[SMS_GSM7BIT_MAX_LEN * UTF8_CHAR_LEN + 1];    /* User Data for GSM_7bit, ANSI_8bit & UCS2_16bit*/
    rt_uint8_t udhLen;                              /* User Data Header Length */
    rt_uint8_t udhInfoType;                         /* Type of User Data Header */
    rt_uint8_t udhInfoLen;                          /* User Data Header information length */
    rt_uint8_t concateMsgRefNo;                     /* Concatenated Message Reference Number */
    rt_uint8_t concateTotalParts;                   /* Maximum Number of concatenated messages */
    rt_uint8_t concateCurntPart;                    /* Sequence Number of concatenated messages */
    rt_uint8_t isConcatenatedMsg;                   /* Concatenated Msg or Not */
    rt_uint8_t smsSts;                              /* Status of SMS */
    uint16_t srcPortAddr;                           /* Source Port Address */
    uint16_t destPortAddr;                          /* Destination Port Address */
    rt_uint8_t isDeliveryReq;
    DATE_DESC date;
    TIME_DESC time;
    
} PDU_DESC;

//###########################################################################
// @PROTOTYPE
//###########################################################################
rt_uint8_t DecodePduData(char *pGsmPduStr, PDU_DESC *pPduDecodeDesc, rt_uint8_t *pError);
rt_uint8_t EncodePduData(PDU_DESC *pPduEncodeDesc, rt_uint8_t *pGsmPduStr, uint16_t *gsmPduStrLen);
rt_uint16_t HexBuf2AsciiBuf(rt_uint8_t *pHexBuf, rt_uint8_t hexBufLen, char *pAsciiStrng);
rt_uint8_t AsciiBuf2HexBuf(char *pAsciiStrng, rt_uint8_t *pHexBuf);
int smsc_decode(char *buf_in, int ilen, char *out, int olen);

void print_decoded_pdu(PDU_DESC *pPduDecodeDesc);
int ucs2_to_utf8(char *buf_in, int ilen, char *out, int olen);
int utf8_to_ucs2(char *buf_in, int ilen, char *out, int olen);

#endif //PKG_USING_RIL

#endif    // PDU_H
