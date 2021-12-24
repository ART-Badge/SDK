/**
*****************************************************************************************
*               Copyright(c) 2005, Colin Percival. All rights reserved.
*****************************************************************************************
   * @file      sha256.h
   * @brief     APIs for using SHA256
   **************************************************************************************
   * @attention
   * Redistribution and use in source and binary forms, with or without
   * modification, are permitted provided that the following conditions
   * are met:
   * 1. Redistributions of source code must retain the above copyright
   *    notice, this list of conditions and the following disclaimer.
   * 2. Redistributions in binary form must reproduce the above copyright
   *    notice, this list of conditions and the following disclaimer in the
   *    documentation and/or other materials provided with the distribution.
   *
   * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
   * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
   * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
   * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
   * SUCH DAMAGE.
   *
   * $FreeBSD$
   **************************************************************************************
  */

/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _SHA256_H_
#define _SHA256_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/** @defgroup  SHA256_API Sha256
    * @brief APIs for using SHA256
    * @{
    */

/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup SHA256_API_Exported_Macros SHA256 APIs Exported Macros
    * @{
    */

#define SHA256_BLOCK_LENGTH         64
#define SHA256_DIGEST_LENGTH        32
#define SHA256_DIGEST_STRING_LENGTH (SHA256_DIGEST_LENGTH * 2 + 1)

/** End of SHA256_API_Exported_Macros
    * @}
    */

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup SHA256_API_Exported_Types SHA256 APIs Exported Types
    * @{
    */

/**  @brief Context structure to store SHA256 algorithm intermediate information */
typedef struct SHA256Context
{
    uint32_t state[8];
    uint64_t count;
    uint8_t buf[SHA256_BLOCK_LENGTH];
} SHA256_CTX;

/** End of SHA256_API_Exported_Types
    * @}
    */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup SHA256_API_Exported_Functions SHA256 APIs Exported Functions
    * @brief
    * @{
    */

/**
    * @brief    SHA-256 initialization, begins a SHA-256 operation.
    * @param    ctx     A context pointer used to store algorithm information
    * @return   void
    */
void    SHA256_Init(SHA256_CTX *);
/**
    * @brief    Add bytes into the hash.
    * @param    ctx     A context pointer used to store algorithm information
    * @param    in      A pointer which point to the array of the source data
    * @param    len     Length of input data, shouldn't be larger than the length of the 'in' array.
    * @return   void
    * @note     This is called to input the source data, and can be called several times to load
    *           a large amount of data sequentially.
*/
void    SHA256_Update(SHA256_CTX *, const void *, size_t);
/**
    * @brief    SHA-256 finalization, called to retrieve the final calculating result.
                It pads the input data, exports the hash value, and clears the context state.
    * @param    ctx     A context pointer used to store algorithm information
    * @param    digest  A pointer which point to the array storing calculating result data
    * @return   void
    * @note     The result is output to an array named 'digest', and the array length is
    *           fixed to 32, which replaces by an understandable macro @ref SHA256_DIGEST_LENGTH.
    */
void    SHA256_Final(SHA256_CTX *, unsigned char [SHA256_DIGEST_LENGTH]);
char   *SHA256_End(SHA256_CTX *, char *);
char   *SHA256_Data(const void *, unsigned int, char *);
char   *SHA256_File(const char *, char *);
bool SHA256_Alloc(SHA256_CTX **ctx);
void SHA256_Free(SHA256_CTX *ctx);
void SHA256(const void *in, size_t len, uint8_t *result);


/** End of SHA256_API_Exported_Functions
    * @}
    */


/** End of SHA256_API
    * @}
    */



#ifdef __cplusplus
}
#endif

#endif /* !_SHA256_H_ */
