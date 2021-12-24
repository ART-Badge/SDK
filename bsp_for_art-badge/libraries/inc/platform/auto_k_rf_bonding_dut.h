#ifndef _AUTO_K_RF_BONDING_DUT_H
#define _AUTO_K_RF_BONDING_DUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>


/**
 * @brief get information from the DUT.
 * @param  golden_info golden information
 * @param  dut_info DUT information
 * @return  Status of Operation.
 * @retval true success
 * @retval false fail
 * @note the function will firstly check the golden_info, if check success return the dut_info.
*/
bool dut_info_get(uint8_t golden_info[32], uint8_t dut_info[32]);

/**
 * @brief check the verify result.
 * @param  result_info result information
 * @return  Status of Operation.
 * @retval true success
 * @retval false fail
*/
bool check_verify_result(uint8_t result_info[16]);



#ifdef __cplusplus
}
#endif

#endif /*_AUTO_K_RF_BONDING_DUT_H*/
