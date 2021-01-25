/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _UPPERSTACK_CONFIG_H_
#define _UPPERSTACK_CONFIG_H_
#define F_BT_LE_SUPPORT                     1       /* support BT Low Energy */

/*============================================================================*
 *               BLE Feature Flags
 *============================================================================*/
#define F_BT_LE_GAP_CENTRAL_SUPPORT              (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_GAP_SCAN_SUPPORT                 (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_GAP_SCAN_FILTER_SUPPORT          (F_BT_LE_GAP_SCAN_SUPPORT && 0)
#define F_BT_LE_GAP_PERIPHERAL_SUPPORT           (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_GATT_CLIENT_SUPPORT              (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_GATT_SERVER_SUPPORT              (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_SMP_OOB_SUPPORT                  (F_BT_LE_SUPPORT && 1)

//BT 4.0
#define F_BT_LE_READ_REMOTE_FEATS                (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_ATT_SIGNED_WRITE_SUPPORT         (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_4_0_DTM_SUPPORT                  (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_READ_ADV_TX_POWRE_SUPPORT        (F_BT_LE_GAP_PERIPHERAL_SUPPORT && 1)
#define F_BT_LE_READ_CHANN_MAP                   (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_READ_REMOTE_VERSION_INFO_SUPPORT (F_BT_LE_SUPPORT && 1)

//BT 4.2
#define F_BT_LE_4_2_SUPPORT                      (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_4_2_SC_SUPPORT                   (F_BT_LE_4_2_SUPPORT && 1)
#define F_BT_LE_4_2_SC_OOB_SUPPORT               (F_BT_LE_SMP_OOB_SUPPORT && F_BT_LE_4_2_SC_SUPPORT && 1)
#define F_BT_LE_4_2_KEY_PRESS_SUPPORT            (F_BT_LE_4_2_SUPPORT && 1)
#define F_BT_LE_4_2_DATA_LEN_EXT_SUPPORT         (F_BT_LE_4_2_SUPPORT && 1)
#define F_BT_LE_PRIVACY_SUPPORT                  (F_BT_LE_4_2_SUPPORT && 1)

//BT 5
#define F_BT_LE_5_0_SUPPORT                      (F_BT_LE_SUPPORT && 1)
#define F_BT_LE_5_0_AE_ADV_SUPPORT               (F_BT_LE_5_0_SUPPORT && 1)
#define F_BT_LE_5_0_AE_SCAN_SUPPORT              (F_BT_LE_5_0_SUPPORT && 1)
#define F_BT_LE_5_0_DTM_SUPPORT                  (F_BT_LE_5_0_SUPPORT && F_BT_LE_4_0_DTM_SUPPORT && 1)
#define F_BT_LE_5_0_SET_PHYS_SUPPORT             (F_BT_LE_5_0_SUPPORT && 1)
#define F_BT_LE_5_0_READ_POWER_SUPPORT           (F_BT_LE_5_0_SUPPORT && 1)

/*============================================================================*
 *               Function Configuration Flags
 *============================================================================*/
#define F_BT_LE_FIX_CHANN_SUPPORT                (F_BT_LE_SUPPORT && 0)
#define F_BT_LE_LOCAL_IRK_SETTING_SUPPORT        (F_BT_LE_SUPPORT && 1)
#define F_BT_OOB_SUPPORT                         (F_BT_LE_SMP_OOB_SUPPORT || F_BT_LE_4_2_SC_OOB_SUPPORT)

#endif /* _UPPERSTACK_CONFIG_H_ */
