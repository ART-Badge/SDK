#ifndef _SBC_H_
#define _SBC_H_

/* defines */
#define SBC_SUCCESS            0
#define SBC_NOT_ENOUGH_DATA   -1
#define SBC_BUFFER_TOO_SMALL  -2
#define SBC_NO_SYNCBYTE       -3
#define SBC_CHECKSUM_ERROR    -4

#define SBC_SYNCBYTE           0x9c
#define SBC_MAX_SUBBANDS       8
#define SBC_MAX_CHANNELS       2
#define SBC_TERMINATOR        -1

/* structs, typedefs */
#define SBC_FREQU16000   0x0
#define SBC_FREQU32000   0x1
#define SBC_FREQU44100   0x2
#define SBC_FREQU48000   0x3
typedef unsigned short TSBCSamplingFrequency;

#define SBC_BLOCKS4    0x0
#define SBC_BLOCKS8    0x1
#define SBC_BLOCKS12   0x2
#define SBC_BLOCKS16   0x3
typedef unsigned short TSBCBlockNumber;

#define SBC_MODE_MONO     0x0
#define SBC_MODE_DUAL     0x1
#define SBC_MODE_STEREO   0x2
#define SBC_MODE_JOINT    0x3
typedef unsigned short TSBCChannelMode;

#define SBC_ALLOCLOUDNESS  0x0
#define SBC_ALLOCSNR       0x1
typedef unsigned short TSBCAllocMethod;

#define SBC_SUBBANDS4  0x0
#define SBC_SUBBANDS8  0x1
typedef unsigned short TSBCSubbandNumber;

typedef struct
{
    TSBCSamplingFrequency samplingFrequency;
    TSBCBlockNumber blockNumber;
    TSBCChannelMode channelMode;
    TSBCAllocMethod allocMethod;
    TSBCSubbandNumber subbandNumber;
    unsigned char bitpool;
} T_SBC_PARAMS;

extern T_SBC_PARAMS sbc_enc_params;

void sbc_init_encoder(void);
int sbc_encode(unsigned char *p_input_buff, int input_size, T_SBC_PARAMS *p_params,
               unsigned char *p_output_buff, int *p_output_size);

void sbc_init_decoder(void);
int sbc_decode(unsigned char *p_input_buff, int input_size, unsigned char *p_output_buff,
               int *p_output_size);

int sbc_get_params(unsigned char *p_input_buff, int input_size, T_SBC_PARAMS *p_params);
int sbc_get_encoded_frame_size(T_SBC_PARAMS *p_params);

#endif
