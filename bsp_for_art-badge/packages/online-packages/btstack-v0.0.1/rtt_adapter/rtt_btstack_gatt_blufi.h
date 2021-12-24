
// ancs_client_demo.h generated from ancs_client_demo.gatt for BTstack
// it needs to be regenerated when the .gatt file is updated.

// To generate ancs_client_demo.h:
// E:\src_code\btstack\tool\compile_gatt.py ancs_client_demo.gatt ancs_client_demo.h

// att db format version 1

// binary attribute representation:
// - size in bytes (16), flags(16), handle (16), uuid (16/128), value(...)

#include <stdint.h>

// Reference: https://en.cppreference.com/w/cpp/feature_test
#if __cplusplus >= 200704L
constexpr
#endif
const uint8_t profile_data[] =
{
    // ATT DB Version
    1,

    // 0x0001 PRIMARY_SERVICE-FFFF
    // 0x0001 PRIMARY_SERVICE-0000FFFF-0000-1000-8000-00805F9B34FB
    0x18, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    // 0x0002 CHARACTERISTIC-FF01-READ | WRITE | WRITE_WITHOUT_RESPONSE | NOTIFY | DYNAMIC
    // 0x0002 CHARACTERISTIC-0000FF01-0000-1000-8000-00805F9B34FB-READ | WRITE | WRITE_WITHOUT_RESPONSE | NOTIFY | DYNAMIC
    0x1b, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x1e, 0x03, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00,
    // 0x0003 VALUE-0000FF01-0000-1000-8000-00805F9B34FB-READ | WRITE | WRITE_WITHOUT_RESPONSE | NOTIFY | DYNAMIC-''
    // READ_ANYBODY, WRITE_ANYBODY
    0x16, 0x00, 0x0e, 0x03, 0x03, 0x00, 0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00,
    // 0x0004 CLIENT_CHARACTERISTIC_CONFIGURATION
    // READ_ANYBODY, WRITE_ANYBODY
    0x0a, 0x00, 0x0e, 0x01, 0x04, 0x00, 0x02, 0x29, 0x00, 0x00,

    // END
    0x00, 0x00,
}; // total size 53 bytes


//
// list service handle ranges
//
#define ATT_SERVICE_FFFF_START_HANDLE 0x0001
#define ATT_SERVICE_FFFF_END_HANDLE 0x0004

//
// list mapping between characteristics and handles
//
#define ATT_CHARACTERISTIC_FF01_01_VALUE_HANDLE 0x0003
#define ATT_CHARACTERISTIC_FF01_01_CLIENT_CONFIGURATION_HANDLE 0x0004

void ble_send_data_test(const char *buf,rt_uint16_t length);
