#ifndef _SPIC_1_PSRAM_
#define _SPIC_1_PSRAM_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void psram_add_hub_task(void);
void spic_1_psram_init(void);
void spic_1_psram_enter_dpls(void);
void spic_1_psram_exit_dlps(void);
bool psram_allowed_enter_dlps_check(void);


#ifdef __cplusplus
}
#endif

#endif /* _SPIC_1_PSRAM_ */
