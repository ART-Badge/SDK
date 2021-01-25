/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file    overlay_mgr.c
   * @brief   overlay manager
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2016 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <string.h>
#include "overlay_mgr.h"


/* linker generates symbols for overlay a section */
extern uint32_t Load$$OVERLAY_A$$RO$$Base;
extern uint32_t Load$$OVERLAY_A$$RW$$Base;

extern uint32_t Image$$OVERLAY_A$$RO$$Base;
extern uint32_t Image$$OVERLAY_A$$RW$$Base;
extern uint32_t Image$$OVERLAY_A$$ZI$$Base;

extern uint32_t Image$$OVERLAY_A$$RO$$Length;
extern uint32_t Image$$OVERLAY_A$$RW$$Length;
extern uint32_t Image$$OVERLAY_A$$ZI$$Length;

/* linker generates symbols for overlay b section */
extern uint32_t Load$$OVERLAY_B$$RO$$Base;
extern uint32_t Load$$OVERLAY_B$$RW$$Base;

extern uint32_t Image$$OVERLAY_B$$RO$$Base;
extern uint32_t Image$$OVERLAY_B$$RW$$Base;
extern uint32_t Image$$OVERLAY_B$$ZI$$Base;

extern uint32_t Image$$OVERLAY_B$$RO$$Length;
extern uint32_t Image$$OVERLAY_B$$RW$$Length;
extern uint32_t Image$$OVERLAY_B$$ZI$$Length;

/* linker generates symbols for overlay c section */
extern uint32_t Load$$OVERLAY_C$$RO$$Base;
extern uint32_t Load$$OVERLAY_C$$RW$$Base;

extern uint32_t Image$$OVERLAY_C$$RO$$Base;
extern uint32_t Image$$OVERLAY_C$$RW$$Base;
extern uint32_t Image$$OVERLAY_C$$ZI$$Base;

extern uint32_t Image$$OVERLAY_C$$RO$$Length;
extern uint32_t Image$$OVERLAY_C$$RW$$Length;
extern uint32_t Image$$OVERLAY_C$$ZI$$Length;

char scenario_name[8];

#if defined ( __CC_ARM   )
#pragma push
#pragma diag_suppress 1296
#endif
static T_OVERLAY_SECTION overlay_sections[OVERLAY_SCENARIO_NUM] =
{
    {
        "BootOnce",

        &Load$$OVERLAY_A$$RO$$Base,
        &Load$$OVERLAY_A$$RW$$Base,

        &Image$$OVERLAY_A$$RO$$Base,
        &Image$$OVERLAY_A$$RW$$Base,
        &Image$$OVERLAY_A$$ZI$$Base,

        /* warning 1296(extended constant initialiser used) */
        (uint32_t) &Image$$OVERLAY_A$$RO$$Length,
        (uint32_t) &Image$$OVERLAY_A$$RW$$Length,
        (uint32_t) &Image$$OVERLAY_A$$ZI$$Length,
    },
    {
        "Scene_B",

        &Load$$OVERLAY_B$$RO$$Base,
        &Load$$OVERLAY_B$$RW$$Base,

        &Image$$OVERLAY_B$$RO$$Base,
        &Image$$OVERLAY_B$$RW$$Base,
        &Image$$OVERLAY_B$$ZI$$Base,

        /* warning 1296(extended constant initialiser used) */
        (uint32_t) &Image$$OVERLAY_B$$RO$$Length,
        (uint32_t) &Image$$OVERLAY_B$$RW$$Length,
        (uint32_t) &Image$$OVERLAY_B$$ZI$$Length,
    },
    {
        "Scene_C",

        &Load$$OVERLAY_C$$RO$$Base,
        &Load$$OVERLAY_C$$RW$$Base,

        &Image$$OVERLAY_C$$RO$$Base,
        &Image$$OVERLAY_C$$RW$$Base,
        &Image$$OVERLAY_C$$ZI$$Base,

        /* warning 1296(extended constant initialiser used) */
        (uint32_t) &Image$$OVERLAY_C$$RO$$Length,
        (uint32_t) &Image$$OVERLAY_C$$RW$$Length,
        (uint32_t) &Image$$OVERLAY_C$$ZI$$Length,
    }
};
#if defined ( __CC_ARM   )
#pragma pop
#endif

/**
 * @brief  load overlay
 * @param  scenario_idx selected overlay section index
 * @return none
 */
bool load_overlay(T_OVERLAY_SCENARIO_IDX scenario_idx)
{
    const T_OVERLAY_SECTION *selected_scenario;

    if (scenario_idx >= OVERLAY_SCENARIO_NUM)
    {
        return false;
    }

    selected_scenario = &overlay_sections[scenario_idx];

    if (memcmp(selected_scenario->signature, scenario_name, 8) == 0)
    {
        return true;
    }

    /* load code */
    memcpy(selected_scenario->image_ro_base, selected_scenario->load_ro_base,
           selected_scenario->ro_length);
    /* load rw data */
    memcpy(selected_scenario->image_rw_base, selected_scenario->load_rw_base,
           selected_scenario->rw_length);
    /* clear zi data */
    memset(selected_scenario->image_zi_base, 0x0, selected_scenario->zi_length);

    memcpy(scenario_name, selected_scenario->signature, 8);

    return true;
}

T_OVERLAY_SCENARIO_IDX get_current_scenario_index(void)
{
    const T_OVERLAY_SECTION *selected_scenario;

    for (int i = 0; i < (int)OVERLAY_SCENARIO_NUM; ++i)
    {
        selected_scenario = &overlay_sections[i];
        if (memcmp(selected_scenario->signature, scenario_name, 8) == 0)
        {
            return (T_OVERLAY_SCENARIO_IDX)i;
        }
    }

    return OVERLAY_SCENARIO_NUM; //not found valid scenario
}
