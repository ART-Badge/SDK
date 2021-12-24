#ifndef _AUTO_K_RF_BONDING_GOLDEN_H
#define _AUTO_K_RF_BONDING_GOLDEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define BEE1_DUT_DEV_TYPE        0x11
#define BEE1_GOLDEN_DEV_TYPE     0x12
#define BEE2_DUT_DEV_TYPE        0x21
#define BEE2_GOLDEN_DEV_TYPE     0x22
#define SBEE2_DUT_DEV_TYPE       0x31
#define SBEE2_GOLDEN_DEV_TYPE    0x32

/**
 * @brief get information from golden sample.
 * @param  golden_info golden sample information
 * @param  dut_ic_type DUT IC type
 * @return  Status of Operation.
 * @retval true success
 * @retval false fail
*/
bool golden_info_get(uint8_t golden_info[32], uint8_t dut_ic_type);

/**
 * @brief verify the information from the DUT.
 * @param  dut_info DUT information
 * @param  dut_ic_type DUT IC type
 * @return  Status of Operation.
 * @retval true verify success
 * @retval false verify fail
*/
bool dut_info_verify(uint8_t dut_info[32], uint8_t dut_ic_type);

/**
 * @brief get verify result information.
 * @param  result_info result information
 * @param  verify_result verify result from dut_info_verify
 * @param  dut_ic_type DUT IC type
 * @return  Status of Operation.
 * @retval true success
 * @retval false fail
*/
bool verify_result_info_get(uint8_t result_info[16], bool verify_result, uint8_t dut_ic_type);

uint32_t platform_random(uint32_t max);


#ifdef __cplusplus
}
#endif

#endif /*_AUTO_K_RF_BONDING_GOLDEN_H*/

