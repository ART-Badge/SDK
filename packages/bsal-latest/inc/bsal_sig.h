/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-28     Supperthomas the first version
 */




#ifndef __BSAL_SIG_H__
#define __BSAL_SIG_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>

//=====================uuid===================

//=====================uuid=PROFILE==================
/**
 * Assigned numbers from www.bluetooth.com/specifications/gatt/declarations
 */
#define BSAL_GATT_UUID_PRIMARY_SERVICE                                     0x2800 // Primary Service
#define BSAL_GATT_UUID_SECONDARY_SERVICE                                   0x2801 // Secondary Service
#define BSAL_GATT_UUID_INCLUDE                                             0x2802 // Include        
#define BSAL_GATT_UUID_CHARACTERISTIC                                      0x2803 // Characteristic Declaration

/**
 * Assigned numbers from www.bluetooth.com/specifications/gatt/services
 */
#define BSAL_GATT_SERVICE_ALERT_NOTIFICATION                                         0x1811 // Alert Notification Service
#define BSAL_GATT_SERVICE_AUTOMATION_IO                                              0x1815 // Automation IO
#define BSAL_GATT_SERVICE_BATTERY_SERVICE                                            0x180F // Battery Service
#define BSAL_GATT_SERVICE_BLOOD_PRESSURE                                             0x1810 // Blood Pressure
#define BSAL_GATT_SERVICE_BODY_COMPOSITION                                           0x181B // Body Composition
#define BSAL_GATT_SERVICE_BOND_MANAGEMENT                                            0x181E // Bond Management Service
#define BSAL_GATT_SERVICE_CONTINUOUS_GLUCOSE_MONITORING                              0x181F // Continuous Glucose Monitoring
#define BSAL_GATT_SERVICE_CURRENT_TIME                                               0x1805 // Current Time Service
#define BSAL_GATT_SERVICE_CYCLING_POWER                                              0x1818 // Cycling Power
#define BSAL_GATT_SERVICE_CYCLING_SPEED_AND_CADENCE                                  0x1816 // Cycling Speed and Cadence
#define BSAL_GATT_SERVICE_DEVICE_INFORMATION                                         0x180A // Device Information
#define BSAL_GATT_SERVICE_ENVIRONMENTAL_SENSING                                      0x181A // Environmental Sensing
#define BSAL_GATT_SERVICE_FITNESS_MACHINE                                            0x1826 // Fitness Machine
#define BSAL_GATT_SERVICE_GENERIC_ACCESS                                             0x1800 //  Generic Access
#define BSAL_GATT_SERVICE_GENERIC_ATTRIBUTE                                          0x1801 // Generic Attribute
#define BSAL_GATT_SERVICE_GLUCOSE                                                    0x1808 // Glucose
#define BSAL_GATT_SERVICE_HEALTH_THERMOMETER                                         0x1809 // Health Thermometer
#define BSAL_GATT_SERVICE_HEART_RATE                                                 0x180D // Heart Rate
#define BSAL_GATT_SERVICE_HTTP_PROXY                                                 0x1823 // HTTP Proxy
#define BSAL_GATT_SERVICE_HUMAN_INTERFACE_DEVICE                                     0x1812 // Human Interface Device
#define BSAL_GATT_SERVICE_IMMEDIATE_ALERT                                            0x1802 // Immediate Alert
#define BSAL_GATT_SERVICE_INDOOR_POSITIONING                                         0x1821 // Indoor Positioning
#define BSAL_GATT_SERVICE_INSULIN_DELIVERY                                           0x183A // Insulin Delivery
#define BSAL_GATT_SERVICE_INTERNET_PROTOCOL_SUPPORT                                  0x1820 // Internet Protocol Support Service
#define BSAL_GATT_SERVICE_LINK_LOSS                                                  0x1803 // Link Loss
#define BSAL_GATT_SERVICE_LOCATION_AND_NAVIGATION                                    0x1819 // Location and Navigation
#define BSAL_GATT_SERVICE_MESH_PROVISIONING                                          0x1827 // Mesh Provisioning Service
#define BSAL_GATT_SERVICE_MESH_PROXY                                                 0x1828 // Mesh Proxy Service
#define BSAL_GATT_SERVICE_NEXT_DST_CHANGE                                            0x1807 // Next DST Change Service
#define BSAL_GATT_SERVICE_OBJECT_TRANSFER                                            0x1825 // Object Transfer Service
#define BSAL_GATT_SERVICE_PHONE_ALERT_STATUS                                         0x180E // Phone Alert Status Service
#define BSAL_GATT_SERVICE_PULSE_OXIMETER                                             0x1822 // Pulse Oximeter Service
#define BSAL_GATT_SERVICE_RECONNECTION_CONFIGURATION                                 0x1829 // Reconnection Configuration
#define BSAL_GATT_SERVICE_REFERENCE_TIME_UPDATE                                      0x1806 // Reference Time Update Service
#define BSAL_GATT_SERVICE_RUNNING_SPEED_AND_CADENCE                                  0x1814 // Running Speed and Cadence
#define BSAL_GATT_SERVICE_SCAN_PARAMETERS                                            0x1813 // Scan Parameters
#define BSAL_GATT_SERVICE_TRANSPORT_DISCOVERY                                        0x1824 // Transport Discovery
#define BSAL_GATT_SERVICE_TX_POWER                                                   0x1804 // Tx Power
#define BSAL_GATT_SERVICE_USER_DATA                                                  0x181C // User Data
#define BSAL_GATT_SERVICE_WEIGHT_SCALE                                               0x181D // Weight Scale
/**
 * Assigned numbers from www.bluetooth.com/specifications/gatt/characteristics
 */
#define BSAL_UUID_CHAR_AEROBIC_HEART_RATE_LOWER_LIMIT                      0x2A7E // Aerobic Heart Rate Lower Limit
#define BSAL_UUID_CHAR_AEROBIC_HEART_RATE_UPPER_LIMIT                      0x2A84 // Aerobic Heart Rate Upper Limit
#define BSAL_UUID_CHAR_AEROBIC_THRESHOLD                                   0x2A7F // Aerobic Threshold
#define BSAL_UUID_CHAR_AGE                                                 0x2A80 // Age
#define BSAL_UUID_CHAR_AGGREGATE                                           0x2A5A // Aggregate
#define BSAL_UUID_CHAR_ALERT_CATEGORY_ID                                   0x2A43 // Alert Category ID
#define BSAL_UUID_CHAR_ALERT_CATEGORY_ID_BIT_MASK                          0x2A42 // Alert Category ID Bit Mask
#define BSAL_UUID_CHAR_ALERT_LEVEL                                         0x2A06 // Alert Level
#define BSAL_UUID_CHAR_ALERT_NOTIFICATION_CONTROL_POINT                    0x2A44 // Alert Notification Control Point
#define BSAL_UUID_CHAR_ALERT_STATUS                                        0x2A3F // Alert Status
#define BSAL_UUID_CHAR_ALTITUDE                                            0x2AB3 // Altitude
#define BSAL_UUID_CHAR_ANAEROBIC_HEART_RATE_LOWER_LIMIT                    0x2A81 // Anaerobic Heart Rate Lower Limit
#define BSAL_UUID_CHAR_ANAEROBIC_HEART_RATE_UPPER_LIMIT                    0x2A82 // Anaerobic Heart Rate Upper Limit
#define BSAL_UUID_CHAR_ANAEROBIC_THRESHOLD                                 0x2A83 // Anaerobic Threshold
#define BSAL_UUID_CHAR_ANALOG                                              0x2A58 // Analog
#define BSAL_UUID_CHAR_ANALOG_OUTPUT                                       0x2A59 // Analog Output
#define BSAL_UUID_CHAR_APPARENT_WIND_DIRECTION                             0x2A73 // Apparent Wind Direction
#define BSAL_UUID_CHAR_APPARENT_WIND_SPEED                                 0x2A72 // Apparent Wind Speed
#define BSAL_UUID_CHAR_BAROMETRIC_PRESSURE_TREND                           0x2AA3 // Barometric Pressure Trend
#define BSAL_UUID_CHAR_BATTERY_LEVEL                                       0x2A19 // Battery Level
#define BSAL_UUID_CHAR_BATTERY_LEVEL_STATE                                 0x2A1B // Battery Level State
#define BSAL_UUID_CHAR_BATTERY_POWER_STATE                                 0x2A1A // Battery Power State
#define BSAL_UUID_CHAR_BLOOD_PRESSURE_FEATURE                              0x2A49 // Blood Pressure Feature
#define BSAL_UUID_CHAR_BLOOD_PRESSURE_MEASUREMENT                          0x2A35 // Blood Pressure Measurement
#define BSAL_UUID_CHAR_BODY_COMPOSITION_FEATURE                            0x2A9B // Body Composition Feature
#define BSAL_UUID_CHAR_BODY_COMPOSITION_MEASUREMENT                        0x2A9C // Body Composition Measurement
#define BSAL_UUID_CHAR_BODY_SENSOR_LOCATION                                0x2A38 // Body Sensor Location
#define BSAL_UUID_CHAR_BOND_MANAGEMENT_CONTROL_POINT                       0x2AA4 // Bond Management Control Point
#define BSAL_UUID_CHAR_BOND_MANAGEMENT_FEATURE                             0x2AA5 // Bond Management Features
#define BSAL_UUID_CHAR_BOOT_KEYBOARD_INPUT_REPORT                          0x2A22 // Boot Keyboard Input Report
#define BSAL_UUID_CHAR_BOOT_KEYBOARD_OUTPUT_REPORT                         0x2A32 // Boot Keyboard Output Report
#define BSAL_UUID_CHAR_BOOT_MOUSE_INPUT_REPORT                             0x2A33 // Boot Mouse Input Report
#define BSAL_UUID_CHAR_CGM_FEATURE                                         0x2AA8 // CGM Feature
#define BSAL_UUID_CHAR_CGM_MEASUREMENT                                     0x2AA7 // CGM Measurement
#define BSAL_UUID_CHAR_CGM_SESSION_RUN_TIME                                0x2AAB // CGM Session Run Time
#define BSAL_UUID_CHAR_CGM_SESSION_START_TIME                              0x2AAA // CGM Session Start Time
#define BSAL_UUID_CHAR_CGM_SPECIFIC_OPS_CONTROL_POINT                      0x2AAC // CGM Specific Ops Control Point
#define BSAL_UUID_CHAR_CGM_STATUS                                          0x2AA9 // CGM Status
#define BSAL_UUID_CHAR_CROSS_TRAINER_DATA                                  0x2ACE // Cross Trainer Data
#define BSAL_UUID_CHAR_CSC_FEATURE                                         0x2A5C // CSC Feature
#define BSAL_UUID_CHAR_CSC_MEASUREMENT                                     0x2A5B // CSC Measurement
#define BSAL_UUID_CHAR_CURRENT_TIME                                        0x2A2B // Current Time
#define BSAL_UUID_CHAR_CYCLING_POWER_CONTROL_POINT                         0x2A66 // Cycling Power Control Point
#define BSAL_UUID_CHAR_CYCLING_POWER_FEATURE                               0x2A65 // Cycling Power Feature
#define BSAL_UUID_CHAR_CYCLING_POWER_MEASUREMENT                           0x2A63 // Cycling Power Measurement
#define BSAL_UUID_CHAR_CYCLING_POWER_VECTOR                                0x2A64 // Cycling Power Vector
#define BSAL_UUID_CHAR_DATABASE_CHANGE_INCREMENT                           0x2A99 // Database Change Increment
#define BSAL_UUID_CHAR_DATE_OF_BIRTH                                       0x2A85 // Date of Birth
#define BSAL_UUID_CHAR_DATE_OF_THRESHOLD_ASSESSMENT                        0x2A86 // Date of Threshold Assessment
#define BSAL_UUID_CHAR_DATE_TIME                                           0x2A08 // Date Time
#define BSAL_UUID_CHAR_DATE_UTC                                            0x2AED // Date UTC
#define BSAL_UUID_CHAR_DAY_DATE_TIME                                       0x2A0A // Day Date Time
#define BSAL_UUID_CHAR_DAY_OF_WEEK                                         0x2A09 // Day of Week
#define BSAL_UUID_CHAR_DESCRIPTOR_VALUE_CHANGED                            0x2A7D // Descriptor Value Changed
#define BSAL_UUID_CHAR_DEW_POINT                                           0x2A7B // Dew Point
#define BSAL_UUID_CHAR_DIGITAL                                             0x2A56 // Digital
#define BSAL_UUID_CHAR_DIGITAL_OUTPUT                                      0x2A57 // Digital Output
#define BSAL_UUID_CHAR_DST_OFFSET                                          0x2A0D // DST Offset
#define BSAL_UUID_CHAR_ELEVATION                                           0x2A6C // Elevation
#define BSAL_UUID_CHAR_EMAIL_ADDRESS                                       0x2A87 // Email Address
#define BSAL_UUID_CHAR_EXACT_TIME_100                                      0x2A0B // Exact Time 100
#define BSAL_UUID_CHAR_EXACT_TIME_256                                      0x2A0C // Exact Time 256
#define BSAL_UUID_CHAR_FAT_BURN_HEART_RATE_LOWER_LIMIT                     0x2A88 // Fat Burn Heart Rate Lower Limit
#define BSAL_UUID_CHAR_FAT_BURN_HEART_RATE_UPPER_LIMIT                     0x2A89 // Fat Burn Heart Rate Upper Limit
#define BSAL_UUID_CHAR_FIRMWARE_REVISION_STRING                            0x2A26 // Firmware Revision String
#define BSAL_UUID_CHAR_FIRST_NAME                                          0x2A8A // First Name
#define BSAL_UUID_CHAR_FITNESS_MACHINE_CONTROL_POINT                       0x2AD9 // Fitness Machine Control Point
#define BSAL_UUID_CHAR_FITNESS_MACHINE_FEATURE                             0x2ACC // Fitness Machine Feature
#define BSAL_UUID_CHAR_FITNESS_MACHINE_STATUS                              0x2ADA // Fitness Machine Status
#define BSAL_UUID_CHAR_FIVE_ZONE_HEART_RATE_LIMITS                         0x2A8B // Five Zone Heart Rate Limits
#define BSAL_UUID_CHAR_FLOOR_NUMBER                                        0x2AB2 // Floor Number
#define BSAL_UUID_CHAR_GAP_APPEARANCE                                      0x2A01 // Appearance
#define BSAL_UUID_CHAR_GAP_CENTRAL_ADDRESS_RESOLUTION                      0x2AA6 // Central Address Resolution
#define BSAL_UUID_CHAR_GAP_DEVICE_NAME                                     0x2A00 // Device Name
#define BSAL_UUID_CHAR_GAP_PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS      0x2A04 // Peripheral Preferred Connection Parameters
#define BSAL_UUID_CHAR_GAP_PERIPHERAL_PRIVACY_FLAG                         0x2A02 // Peripheral Privacy Flag
#define BSAL_UUID_CHAR_GAP_RECONNECTION_ADDRESS                            0x2A03 // Reconnection Address
#define BSAL_UUID_CHAR_GATT_SERVICE_CHANGED                                0x2A05 // Service Changed
#define BSAL_UUID_CHAR_GENDER                                              0x2A8C // Gender
#define BSAL_UUID_CHAR_GLUCOSE_FEATURE                                     0x2A51 // Glucose Feature
#define BSAL_UUID_CHAR_GLUCOSE_MEASUREMENT                                 0x2A18 // Glucose Measurement
#define BSAL_UUID_CHAR_GLUCOSE_MEASUREMENT_CONTEXT                         0x2A34 // Glucose Measurement Context
#define BSAL_UUID_CHAR_GUST_FACTOR                                         0x2A74 // Gust Factor
#define BSAL_UUID_CHAR_HARDWARE_REVISION_STRING                            0x2A27 // Hardware Revision String
#define BSAL_UUID_CHAR_HEART_RATE_CONTROL_POINT                            0x2A39 // Heart Rate Control Point
#define BSAL_UUID_CHAR_HEART_RATE_MAX                                      0x2A8D // Heart Rate Max
#define BSAL_UUID_CHAR_HEART_RATE_MEASUREMENT                              0x2A37 // Heart Rate Measurement
#define BSAL_UUID_CHAR_HEAT_INDEX                                          0x2A7A // Heat Index
#define BSAL_UUID_CHAR_HEIGHT                                              0x2A8E // Height
#define BSAL_UUID_CHAR_HID_CONTROL_POINT                                   0x2A4C // HID Control Point
#define BSAL_UUID_CHAR_HID_INFORMATION                                     0x2A4A // HID Information
#define BSAL_UUID_CHAR_HIP_CIRCUMFERENCE                                   0x2A8F // Hip Circumference
#define BSAL_UUID_CHAR_HTTP_CONTROL_POINT                                  0x2ABA // HTTP Control Point
#define BSAL_UUID_CHAR_HTTP_ENTITY_BODY                                    0x2AB9 // HTTP Entity Body
#define BSAL_UUID_CHAR_HTTP_HEADERS                                        0x2AB7 // HTTP Headers
#define BSAL_UUID_CHAR_HTTP_STATUS_CODE                                    0x2AB8 // HTTP Status Code
#define BSAL_UUID_CHAR_HTTPS_SECURITY                                      0x2ABB // HTTPS Security
#define BSAL_UUID_CHAR_HUMIDITY                                            0x2A6F // Humidity
#define BSAL_UUID_CHAR_IDD_ANNUNCIATION_STATUS                             0x2B22 // IDD Annunciation Status
#define BSAL_UUID_CHAR_IDD_COMMAND_CONTROL_POINT                           0x2B25 // IDD Command Control Point
#define BSAL_UUID_CHAR_IDD_COMMAND_DATA                                    0x2B26 // IDD Command Data
#define BSAL_UUID_CHAR_IDD_FEATURES                                        0x2B23 // IDD Features
#define BSAL_UUID_CHAR_IDD_HISTORY_DATA                                    0x2B28 // IDD History Data
#define BSAL_UUID_CHAR_IDD_RECORD_ACCESS_CONTROL_POINT                     0x2B27 // IDD Record Access Control Point
#define BSAL_UUID_CHAR_IDD_STATUS                                          0x2B21 // IDD Status
#define BSAL_UUID_CHAR_IDD_STATUS_CHANGED                                  0x2B20 // IDD Status Changed
#define BSAL_UUID_CHAR_IDD_STATUS_READER_CONTROL_POINT                     0x2B24 // IDD Status Reader Control Point
#define BSAL_UUID_CHAR_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST 0x2A2A // IEEE 11073-20601 Regulatory Certification Data List
#define BSAL_UUID_CHAR_INDOOR_BIKE_DATA                                    0x2AD2 // Indoor Bike Data
#define BSAL_UUID_CHAR_INDOOR_POSITIONING_CONFIGURATION                    0x2AAD // Indoor Positioning Configuration
#define BSAL_UUID_CHAR_INTERMEDIATE_CUFF_PRESSURE                          0x2A36 // Intermediate Cuff Pressure
#define BSAL_UUID_CHAR_INTERMEDIATE_TEMPERATURE                            0x2A1E // Intermediate Temperature
#define BSAL_UUID_CHAR_IRRADIANCE                                          0x2A77 // Irradiance
#define BSAL_UUID_CHAR_LANGUAGE                                            0x2AA2 // Language
#define BSAL_UUID_CHAR_LAST_NAME                                           0x2A90 // Last Name
#define BSAL_UUID_CHAR_LATITUDE                                            0x2AAE // Latitude
#define BSAL_UUID_CHAR_LN_CONTROL_POINT                                    0x2A6B // LN Control Point
#define BSAL_UUID_CHAR_LN_FEATURE                                          0x2A6A // LN Feature
#define BSAL_UUID_CHAR_LOCAL_EAST_COORDINATE                               0x2AB1 // Local East Coordinate
#define BSAL_UUID_CHAR_LOCAL_NORTH_COORDINATE                              0x2AB0 // Local North Coordinate
#define BSAL_UUID_CHAR_LOCAL_TIME_INFORMATION                              0x2A0F // Local Time Information
#define BSAL_UUID_CHAR_LOCATION_AND_SPEED                                  0x2A67 // Location and Speed Characteristic
#define BSAL_UUID_CHAR_LOCATION_NAME                                       0x2AB5 // Location Name
#define BSAL_UUID_CHAR_LONGITUDE                                           0x2AAF // Longitude
#define BSAL_UUID_CHAR_MAGNETIC_DECLINATION                                0x2A2C // Magnetic Declination
#define BSAL_UUID_CHAR_MAGNETIC_FLUX_DENSITY_2D                            0x2AA0 // Magnetic Flux Density - 2D
#define BSAL_UUID_CHAR_MAGNETIC_FLUX_DENSITY_3D                            0x2AA1 // Magnetic Flux Density - 3D
#define BSAL_UUID_CHAR_MANUFACTURER_NAME_STRING                            0x2A29 // Manufacturer Name String
#define BSAL_UUID_CHAR_MAXIMUM_RECOMMENDED_HEART_RATE                      0x2A91 // Maximum Recommended Heart Rate
#define BSAL_UUID_CHAR_MEASUREMENT_INTERVAL                                0x2A21 // Measurement Interval
#define BSAL_UUID_CHAR_MODEL_NUMBER_STRING                                 0x2A24 // Model Number String
#define BSAL_UUID_CHAR_NAVIGATION                                          0x2A68 // Navigation
#define BSAL_UUID_CHAR_NETWORK_AVAILABILITY                                0x2A3E // Network Availability
#define BSAL_UUID_CHAR_NEW_ALERT                                           0x2A46 // New Alert
#define BSAL_UUID_CHAR_OBJECT_ACTION_CONTROL_POINT                         0x2AC5 // Object Action Control Point
#define BSAL_UUID_CHAR_OBJECT_CHANGED                                      0x2AC8 // Object Changed
#define BSAL_UUID_CHAR_OBJECT_FIRST_CREATED                                0x2AC1 // Object First-Created
#define BSAL_UUID_CHAR_OBJECT_ID                                           0x2AC3 // Object ID
#define BSAL_UUID_CHAR_OBJECT_LAST_MODIFIED                                0x2AC2 // Object Last-Modified
#define BSAL_UUID_CHAR_OBJECT_LIST_CONTROL_POINT                           0x2AC6 // Object List Control Point
#define BSAL_UUID_CHAR_OBJECT_LIST_FILTER                                  0x2AC7 // Object List Filter
#define BSAL_UUID_CHAR_OBJECT_NAME                                         0x2ABE // Object Name
#define BSAL_UUID_CHAR_OBJECT_PROPERTIES                                   0x2AC4 // Object Properties
#define BSAL_UUID_CHAR_OBJECT_SIZE                                         0x2AC0 // Object Size
#define BSAL_UUID_CHAR_OBJECT_TYPE                                         0x2ABF // Object Type
#define BSAL_UUID_CHAR_OTS_FEATURE                                         0x2ABD // OTS Feature
#define BSAL_UUID_CHAR_PLX_CONTINUOUS_MEASUREMENT                          0x2A5F // PLX Continuous Measurement Characteristic
#define BSAL_UUID_CHAR_PLX_FEATURES                                        0x2A60 // PLX Features
#define BSAL_UUID_CHAR_PLX_SPOT_CHECK_MEASUREMENT                          0x2A5E // PLX Spot-Check Measurement
#define BSAL_UUID_CHAR_PNP_ID                                              0x2A50 // PnP ID
#define BSAL_UUID_CHAR_POLLEN_CONCENTRATION                                0x2A75 // Pollen Concentration
#define BSAL_UUID_CHAR_POSITION_2D                                         0x2A2F // Position 2D
#define BSAL_UUID_CHAR_POSITION_3D                                         0x2A30 // Position 3D
#define BSAL_UUID_CHAR_POSITION_QUALITY                                    0x2A69 // Position Quality
#define BSAL_UUID_CHAR_PRESSURE                                            0x2A6D // Pressure
#define BSAL_UUID_CHAR_PROTOCOL_MODE                                       0x2A4E // Protocol Mode
#define BSAL_UUID_CHAR_PULSE_OXIMETRY_CONTROL_POINT                        0x2A62 // Pulse Oximetry Control Point
#define BSAL_UUID_CHAR_RAINFALL                                            0x2A78 // Rainfall
#define BSAL_UUID_CHAR_RC_FEATURE                                          0x2B1D // RC Feature
#define BSAL_UUID_CHAR_RC_SETTINGS                                         0x2B1E // RC Settings
#define BSAL_UUID_CHAR_RECONNECTION_CONFIGURATION_CONTROL_POINT            0x2B1F // Reconnection Configuration Control Point
#define BSAL_UUID_CHAR_RECORD_ACCESS_CONTROL_POINT                         0x2A52 // Record Access Control Point
#define BSAL_UUID_CHAR_REFERENCE_TIME_INFORMATION                          0x2A14 // Reference Time Information
#define BSAL_UUID_CHAR_REMOVABLE                                           0x2A3A // Removable
#define BSAL_UUID_CHAR_REPORT                                              0x2A4D // Report
#define BSAL_UUID_CHAR_REPORT_MAP                                          0x2A4B // Report Map
#define BSAL_UUID_CHAR_RESOLVABLE_PRIVATE_ADDRESS_ONLY                     0x2AC9 // Resolvable Private Address Only
#define BSAL_UUID_CHAR_RESTING_HEART_RATE                                  0x2A92 // Resting Heart Rate
#define BSAL_UUID_CHAR_RINGER_CONTROL_POINT                                0x2A40 // Ringer Control point
#define BSAL_UUID_CHAR_RINGER_SETTING                                      0x2A41 // Ringer Setting
#define BSAL_UUID_CHAR_ROWER_DATA                                          0x2AD1 // Rower Data
#define BSAL_UUID_CHAR_RSC_FEATURE                                         0x2A54 // RSC Feature
#define BSAL_UUID_CHAR_RSC_MEASUREMENT                                     0x2A53 // RSC Measurement
#define BSAL_UUID_CHAR_SC_CONTROL_POINT                                    0x2A55 // SC Control Point
#define BSAL_UUID_CHAR_SCAN_INTERVAL_WINDOW                                0x2A4F // Scan Interval Window
#define BSAL_UUID_CHAR_SCAN_REFRESH                                        0x2A31 // Scan Refresh
#define BSAL_UUID_CHAR_SCIENTIFIC_TEMPERATURE_CELSIUS                      0x2A3C // Scientific Temperature Celsius
#define BSAL_UUID_CHAR_SECONDARY_TIME_ZONE                                 0x2A10 // Secondary Time Zone
#define BSAL_UUID_CHAR_SENSOR_LOCATION                                     0x2A5D // Sensor Location
#define BSAL_UUID_CHAR_SERIAL_NUMBER_STRING                                0x2A25 // Serial Number String
#define BSAL_UUID_CHAR_SERVICE_REQUIRED                                    0x2A3B // Service Required
#define BSAL_UUID_CHAR_SOFTWARE_REVISION_STRING                            0x2A28 // Software Revision String
#define BSAL_UUID_CHAR_SPORT_TYPE_FOR_AEROBIC_AND_ANAEROBIC_THRESHOLDS     0x2A93 // Sport Type for Aerobic and Anaerobic Thresholds
#define BSAL_UUID_CHAR_STAIR_CLIMBER_DATA                                  0x2AD0 // Stair Climber Data
#define BSAL_UUID_CHAR_STEP_CLIMBER_DATA                                   0x2ACF // Step Climber Data
#define BSAL_UUID_CHAR_STRING                                              0x2A3D // String
#define BSAL_UUID_CHAR_SUPPORTED_HEART_RATE_RANGE                          0x2AD7 // Supported Heart Rate Range
#define BSAL_UUID_CHAR_SUPPORTED_INCLINATION_RANGE                         0x2AD5 // Supported Inclination Range
#define BSAL_UUID_CHAR_SUPPORTED_NEW_ALERT_CATEGORY                        0x2A47 // Supported New Alert Category
#define BSAL_UUID_CHAR_SUPPORTED_POWER_RANGE                               0x2AD8 // Supported Power Range
#define BSAL_UUID_CHAR_SUPPORTED_RESISTANCE_LEVEL_RANGE                    0x2AD6 // Supported Resistance Level Range
#define BSAL_UUID_CHAR_SUPPORTED_SPEED_RANGE                               0x2AD4 // Supported Speed Range
#define BSAL_UUID_CHAR_SUPPORTED_UNREAD_ALERT_CATEGORY                     0x2A48 // Supported Unread Alert Category
#define BSAL_UUID_CHAR_SYSTEM_ID                                           0x2A23 // System ID
#define BSAL_UUID_CHAR_TDS_CONTROL_POINT                                   0x2ABC // TDS Control Point
#define BSAL_UUID_CHAR_TEMPERATURE                                         0x2A6E // Temperature
#define BSAL_UUID_CHAR_TEMPERATURE_CELSIUS                                 0x2A1F // Temperature Celsius
#define BSAL_UUID_CHAR_TEMPERATURE_FAHRENHEIT                              0x2A20 // Temperature Fahrenheit
#define BSAL_UUID_CHAR_TEMPERATURE_MEASUREMENT                             0x2A1C // Temperature Measurement
#define BSAL_UUID_CHAR_TEMPERATURE_TYPE                                    0x2A1D // Temperature Type
#define BSAL_UUID_CHAR_THREE_ZONE_HEART_RATE_LIMITS                        0x2A94 // Three Zone Heart Rate Limits
#define BSAL_UUID_CHAR_TIME_ACCURACY                                       0x2A12 // Time Accuracy
#define BSAL_UUID_CHAR_TIME_BROADCAST                                      0x2A15 // Time Broadcast
#define BSAL_UUID_CHAR_TIME_SOURCE                                         0x2A13 // Time Source
#define BSAL_UUID_CHAR_TIME_UPDATE_CONTROL_POINT                           0x2A16 // Time Update Control Point
#define BSAL_UUID_CHAR_TIME_UPDATE_STATE                                   0x2A17 // Time Update State
#define BSAL_UUID_CHAR_TIME_WITH_DST                                       0x2A11 // Time with DST
#define BSAL_UUID_CHAR_TIME_ZONE                                           0x2A0E // Time Zone
#define BSAL_UUID_CHAR_TRAINING_STATUS                                     0x2AD3 // Training Status
#define BSAL_UUID_CHAR_TREADMILL_DATA                                      0x2ACD // Treadmill Data
#define BSAL_UUID_CHAR_TRUE_WIND_DIRECTION                                 0x2A71 // True Wind Direction
#define BSAL_UUID_CHAR_TRUE_WIND_SPEED                                     0x2A70 // True Wind Speed
#define BSAL_UUID_CHAR_TWO_ZONE_HEART_RATE_LIMIT                           0x2A95 // Two Zone Heart Rate Limit
#define BSAL_UUID_CHAR_TX_POWER_LEVEL                                      0x2A07 // Tx Power Level
#define BSAL_UUID_CHAR_UNCERTAINTY                                         0x2AB4 // Uncertainty
#define BSAL_UUID_CHAR_UNREAD_ALERT_STATUS                                 0x2A45 // Unread Alert Status
#define BSAL_UUID_CHAR_URI                                                 0x2AB6 // URI
#define BSAL_UUID_CHAR_USER_CONTROL_POINT                                  0x2A9F // User Control Point
#define BSAL_UUID_CHAR_USER_INDEX                                          0x2A9A // User Index
#define BSAL_UUID_CHAR_UV_INDEX                                            0x2A76 // UV Index
#define BSAL_UUID_CHAR_VO2_MAX                                             0x2A96 // VO2 Max
#define BSAL_UUID_CHAR_WAIST_CIRCUMFERENCE                                 0x2A97 // Waist Circumference
#define BSAL_UUID_CHAR_WEIGHT                                              0x2A98 // Weight
#define BSAL_UUID_CHAR_WEIGHT_MEASUREMENT                                  0x2A9D // Weight Measurement
#define BSAL_UUID_CHAR_WEIGHT_SCALE_FEATURE                                0x2A9E // Weight Scale Feature
#define BSAL_UUID_CHAR_WIND_CHILL                                          0x2A79 // Wind Chill

/**
 * Assigned numbers from www.bluetooth.com/specifications/gatt/descriptors
 */
#define BSAL_UUID_DESCRIPTOR_ES_CONFIGURATION                                        0x290B // Environmental Sensing Configuration
#define BSAL_UUID_DESCRIPTOR_ES_MEASUREMENT                                          0x290C // Environmental Sensing Measurement
#define BSAL_UUID_DESCRIPTOR_ES_TRIGGER_SETTING                                      0x290D // Environmental Sensing Trigger Setting
#define BSAL_UUID_DESCRIPTOR_EXTERNAL_REPORT_REFERENCE                               0x2907 // External Report Reference
#define BSAL_UUID_DESCRIPTOR_GATT_CHARACTERISTIC_AGGREGATE_FORMAT                    0x2905 // Characteristic Aggregate Format
#define BSAL_UUID_DESCRIPTOR_GATT_CHARACTERISTIC_EXTENDED_PROPERTIES                 0x2900 // Characteristic Extended Properties
#define BSAL_UUID_DESCRIPTOR_GATT_CHARACTERISTIC_PRESENTATION_FORMAT                 0x2904 // Characteristic Presentation Format
#define BSAL_UUID_DESCRIPTOR_GATT_CHARACTERISTIC_USER_DESCRIPTION                    0x2901 // Characteristic User Description
#define BSAL_UUID_DESCRIPTOR_GATT_CLIENT_CHARACTERISTIC_CONFIGURATION                0x2902 // Client Characteristic Configuration
#define BSAL_UUID_DESCRIPTOR_GATT_SERVER_CHARACTERISTIC_CONFIGURATION                0x2903 // Server Characteristic Configuration
#define BSAL_UUID_DESCRIPTOR_NUMBER_OF_DIGITALS                                      0x2909 // Number of Digitals
#define BSAL_UUID_DESCRIPTOR_REPORT_REFERENCE                                        0x2908 // Report Reference
#define BSAL_UUID_DESCRIPTOR_TIME_TRIGGER_SETTING                                    0x290E // Time Trigger Setting
#define BSAL_UUID_DESCRIPTOR_VALID_RANGE                                             0x2906 // Valid Range
#define BSAL_UUID_DESCRIPTOR_VALUE_TRIGGER_SETTING                                   0x290A // Value Trigger Setting

//=============================UUID   END============================================

//=================================ATT===============================================

// Attribute Property Flags
#define BSAL_ATT_P_BROADCAST                  0x01
#define BSAL_ATT_P_READ                       0x02
#define BSAL_ATT_P_WRITE_WITHOUT_RESPONSE     0x04
#define BSAL_ATT_P_WRITE                      0x08
#define BSAL_ATT_P_NOTIFY                     0x10
#define BSAL_ATT_P_INDICATE                   0x20
#define BSAL_ATT_P_AUTHENTICATED_SIGNED_WRITE 0x40
#define BSAL_ATT_P_EXTENDED_PROPERTIES        0x80


// Attribute NOTIFY FLAG
#define BSAL_GATT_CCC_NONE                              0x0000 /**< The Characteristic Value shall be neither indicated nor notified. */
#define BSAL_GATT_CCC_NOTIFY                            0x0001 /**< The Characteristic Value shall be notified. */
#define BSAL_GATT_CCC_INDICATE                          0x0002 /**< The Characteristic Value shall be indicated. */
#define BSAL_GATT_CCC_NOTIFY_INDICATE                   0x0003 /**< The Characteristic Value shall be both indicated and notified. */

//==========================ADV==============================

/** @defgroup ADV_PARAM_FILTER_POLICY  adv filter policy
  * @{
  */
typedef enum
{
    BSAL_GAP_ADV_FILTER_ANY = 0,
    BSAL_GAP_ADV_FILTER_WHITE_LIST_SCAN,
    BSAL_GAP_ADV_FILTER_WHITE_LIST_CONN,
    BSAL_GAP_ADV_FILTER_WHITE_LIST_ALL,
} T_BSAL_GAP_ADV_FILTER_POLICY;
/**
  * @}
  */

/** @defgroup BTCHANNEL_MAP  BT ADVERTISING CHANNEL MAP
  * @{
  */
#define BSAL_GAP_ADVCHAN_37                          0x01  //!< Advertisement Channel 37
#define BSAL_GAP_ADVCHAN_38                          0x02  //!< Advertisement Channel 38
#define BSAL_GAP_ADVCHAN_39                          0x04  //!< Advertisement Channel 39
#define BSAL_GAP_ADVCHAN_ALL (BSAL_GAP_ADVCHAN_37 | BSAL_GAP_ADVCHAN_38 | BSAL_GAP_ADVCHAN_39) //!< All Advertisement Channels Enabled
/**
  * @}
  */

//BSAL_GAP_TYPE_FLAGS BIT CSS
#define BSAL_GAP_ADTYPE_FLAGS_LIMITED                            0x01 //!< Discovery Mode: LE Limited Discoverable Mode
#define BSAL_GAP_ADTYPE_FLAGS_GENERAL                            0x02 //!< Discovery Mode: LE General Discoverable Mode
#define BSAL_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED                0x04 //!< Discovery Mode: BR/EDR Not Supported
#define BSAL_GAP_ADTYPE_FLAGS_SIMULTANEOUS_LE_BREDR_CONTROLLER   0x08 //!< Discovery Mode: Simultaneous LE and BR/EDR Controller Supported
#define BSAL_GAP_ADTYPE_FLAGS_SIMULTANEOUS_LE_BREDR_HOST         0x10 //!< Discovery Mode: Simultaneous LE and BR/EDR Host Supported


// https://www.bluetooth.com/specifications/assigned-numbers/generic-access-profile/
//AD TYPE
#define BSAL_GAP_TYPE_FLAGS                                              0x01 // Flags
#define BSAL_GAP_TYPE_INCOMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS      0x02 // Incomplete List of 16-bit Service Class UUIDs
#define BSAL_GAP_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS        0x03 // Complete List of 16-bit Service Class UUIDs
#define BSAL_GAP_TYPE_INCOMPLETE_LIST_OF_32_BIT_SERVICE_CLASS_UUIDS      0x04 // Incomplete List of 32-bit Service Class UUIDs
#define BSAL_GAP_TYPE_COMPLETE_LIST_OF_32_BIT_SERVICE_CLASS_UUIDS        0x05 // Complete List of 32-bit Service Class UUIDs
#define BSAL_GAP_TYPE_INCOMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS     0x06 // Incomplete List of 128-bit Service Class UUIDs
#define BSAL_GAP_TYPE_COMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS       0x07 // Complete List of 128-bit Service Class UUIDs
#define BSAL_GAP_TYPE_SHORTENED_LOCAL_NAME                               0x08 // Shortened Local Name
#define BSAL_GAP_TYPE_COMPLETE_LOCAL_NAME                                0x09 // Complete Local Name
#define BSAL_GAP_TYPE_TX_POWER_LEVEL                                     0x0A // Tx Power Level
#define BSAL_GAP_TYPE_CLASS_OF_DEVICE                                    0x0D // Class of Device
#define BSAL_GAP_TYPE_SIMPLE_PAIRING_HASH_C                              0x0E // Simple Pairing Hash C
#define BSAL_GAP_TYPE_SIMPLE_PAIRING_HASH_C_192                          0x0E // Simple Pairing Hash C-192
#define BSAL_GAP_TYPE_SIMPLE_PAIRING_RANDOMIZER_R                        0x0F // Simple Pairing Randomizer R
#define BSAL_GAP_TYPE_SIMPLE_PAIRING_RANDOMIZER_R_192                    0x0F // Simple Pairing Randomizer R-192
#define BSAL_GAP_TYPE_DEVICE_ID                                          0x10 // Device ID
#define BSAL_GAP_TYPE_SECURITY_MANAGER_TK_VALUE                          0x10 // Security Manager TK Value
#define BSAL_GAP_TYPE_SECURITY_MANAGER_OUT_OF_BAND_FLAGS                 0x11 // Security Manager Out of Band Flags
#define BSAL_GAP_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE                    0x12 // Slave Connection Interval Range
#define BSAL_GAP_TYPE_LIST_OF_16_BIT_SERVICE_SOLICITATION_UUIDS          0x14 // List of 16-bit Service Solicitation UUIDs
#define BSAL_GAP_TYPE_LIST_OF_128_BIT_SERVICE_SOLICITATION_UUIDS         0x15 // List of 128-bit Service Solicitation UUIDs
#define BSAL_GAP_TYPE_SERVICE_DATA                                       0x16 // Service Data
#define BSAL_GAP_TYPE_SERVICE_DATA_16_BIT_UUID                           0x16 // Service Data - 16-bit UUID
#define BSAL_GAP_TYPE_PUBLIC_TARGET_ADDRESS                              0x17 // Public Target Address
#define BSAL_GAP_TYPE_RANDOM_TARGET_ADDRESS                              0x18 // Random Target Address
#define BSAL_GAP_TYPE_APPEARANCE                                         0x19 // Appearance
#define BSAL_GAP_TYPE_ADVERTISING_INTERVAL                               0x1A // Advertising Interval
#define BSAL_GAP_TYPE_LE_BLUETOOTH_DEVICE_ADDRESS                        0x1B // LE Bluetooth Device Address
#define BSAL_GAP_TYPE_LE_ROLE                                            0x1C // LE Role
#define BSAL_GAP_TYPE_SIMPLE_PAIRING_HASH_C_256                          0x1D // Simple Pairing Hash C-256
#define BSAL_GAP_TYPE_SIMPLE_PAIRING_RANDOMIZER_R_256                    0x1E // Simple Pairing Randomizer R-256
#define BSAL_GAP_TYPE_LIST_OF_32_BIT_SERVICE_SOLICITATION_UUIDS          0x1F // List of 32-bit Service Solicitation UUIDs
#define BSAL_GAP_TYPE_SERVICE_DATA_32_BIT_UUID                           0x20 // Service Data - 32-bit UUID
#define BSAL_GAP_TYPE_SERVICE_DATA_128_BIT_UUID                          0x21 // Service Data - 128-bit UUID
#define BSAL_GAP_TYPE_LE_SECURE_CONNECTIONS_CONFIRMATION_VALUE           0x22 // LE Secure Connections Confirmation Value
#define BSAL_GAP_TYPE_LE_SECURE_CONNECTIONS_RANDOM_VALUE                 0x23 // LE Secure Connections Random Value
#define BSAL_GAP_TYPE_URI                                                0x24 // URI
#define BSAL_GAP_TYPE_INDOOR_POSITIONING                                 0x25 // Indoor Positioning
#define BSAL_GAP_TYPE_TRANSPORT_DISCOVERY_DATA                           0x26 // Transport Discovery Data
#define BSAL_GAP_TYPE_LE_SUPPORTED_FEATURES                              0x27 // LE Supported Features
#define BSAL_GAP_TYPE_CHANNEL_MAP_UPDATE_INDICATION                      0x28 // Channel Map Update Indication
#define BSAL_GAP_TYPE_PB_ADV                                             0x29 // PB-ADV
#define BSAL_GAP_TYPE_MESH_MESSAGE                                       0x2A // Mesh Message
#define BSAL_GAP_TYPE_MESH_BEACON                                        0x2B // Mesh Beacon
#define BSAL_GAP_TYPE_3D_INFORMATION_DATA                                0x3D // 3D Information Data
#define BSAL_GAP_TYPE_MANUFACTURER_SPECIFIC_DATA                         0xFF // Manufacturer Specific Data

//https://www.bluetooth.com/xml-viewer/?src=https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.gap.appearance.xml
/* LE GAP APPERANCE VALUE*/
#define BSAL_GAP_APPEARANCE_UNKNOWN                                0
#define BSAL_GAP_APPEARANCE_GENERIC_PHONE                          64
#define BSAL_GAP_APPEARANCE_GENERIC_COMPUTER                       128
#define BSAL_GAP_APPEARANCE_GENERIC_WATCH                          192
#define BSAL_GAP_APPEARANCE_WATCH_SPORTS_WATCH                     193
#define BSAL_GAP_APPEARANCE_GENERIC_CLOCK                          256
#define BSAL_GAP_APPEARANCE_GENERIC_DISPLAY                        320
#define BSAL_GAP_APPEARANCE_GENERIC_REMOTE_CONTROL                 384
#define BSAL_GAP_APPEARANCE_GENERIC_EYE_GLASSES                    448
#define BSAL_GAP_APPEARANCE_GENERIC_TAG                            512
#define BSAL_GAP_APPEARANCE_GENERIC_KEYRING                        576
#define BSAL_GAP_APPEARANCE_GENERIC_MEDIA_PLAYER                   640
#define BSAL_GAP_APPEARANCE_GENERIC_BARCODE_SCANNER                704
#define BSAL_GAP_APPEARANCE_GENERIC_THERMOMETER                    768
#define BSAL_GAP_APPEARANCE_THERMOMETER_EAR                        769
#define BSAL_GAP_APPEARANCE_GENERIC_HEART_RATE_SENSOR              832
#define BSAL_GAP_APPEARANCE_HEART_RATE_SENSOR_HEART_RATE_BELT      833
#define BSAL_GAP_APPEARANCE_GENERIC_BLOOD_PRESSURE                 896
#define BSAL_GAP_APPEARANCE_BLOOD_PRESSURE_ARM                     897
#define BSAL_GAP_APPEARANCE_BLOOD_PRESSURE_WRIST                   898
#define BSAL_GAP_APPEARANCE_HUMAN_INTERFACE_DEVICE                 960
#define BSAL_GAP_APPEARANCE_KEYBOARD                               961
#define BSAL_GAP_APPEARANCE_MOUSE                                  962
#define BSAL_GAP_APPEARANCE_JOYSTICK                               963
#define BSAL_GAP_APPEARANCE_GAMEPAD                                964
#define BSAL_GAP_APPEARANCE_DIGITIZER_TABLET                       965
#define BSAL_GAP_APPEARANCE_CARD_READER                            966
#define BSAL_GAP_APPEARANCE_DIGITAL_PEN                            967
#define BSAL_GAP_APPEARANCE_BARCODE_SCANNER                        968
#define BSAL_GAP_APPEARANCE_GENERIC_GLUCOSE_METER                  1024
#define BSAL_GAP_APPEARANCE_GENERIC_RUNNING_WALKING_SENSOR         1088
#define BSAL_GAP_APPEARANCE_RUNNING_WALKING_SENSOR_IN_SHOE         1089
#define BSAL_GAP_APPEARANCE_RUNNING_WALKING_SENSOR_ON_SHOE         1090
#define BSAL_GAP_APPEARANCE_RUNNING_WALKING_SENSOR_ON_HIP          1091
#define BSAL_GAP_APPEARANCE_GENERIC_CYCLING                        1152
#define BSAL_GAP_APPEARANCE_CYCLING_CYCLING_COMPUTER               1153
#define BSAL_GAP_APPEARANCE_CYCLING_SPEED_SENSOR                   1154
#define BSAL_GAP_APPEARANCE_CYCLING_CADENCE_SENSOR                 1155
#define BSAL_GAP_APPEARANCE_CYCLING_POWER_SENSOR                   1156
#define BSAL_GAP_APPEARANCE_CYCLING_SPEED_AND_CADENCE_SENSOR       1157
#define BSAL_GAP_APPEARANCE_GENERIC_PULSE_OXIMETER                 3136
#define BSAL_GAP_APPEARANCE_FINGERTIP                              3137
#define BSAL_GAP_APPEARANCE_WRIST_WORN                             3138
#define BSAL_GAP_APPEARANCE_GENERIC_WEIGHT_SCALE                   3200
#define BSAL_GAP_APPEARANCE_GENERIC_OUTDOOR_SPORTS_ACTIVITY        5184
#define BSAL_GAP_APPEARANCE_LOCATION_DISPLAY_DEVICE                5185
#define BSAL_GAP_APPEARANCE_LOCATION_AND_NAVIGATION_DISPLAY_DEVICE 5186
#define BSAL_GAP_APPEARANCE_LOCATION_POD                           5187
#define BSAL_GAP_APPEARANCE_LOCATION_AND_NAVIGATION_POD            5188

//AD_TYPE
#define BSAL_GAP_ADTYPE_ADV_IND                                               0x00      //!< Connectable undirected advertisement
#define BSAL_GAP_ADTYPE_ADV_DIRECT_IND                                        0x01      //!< Connectable high duty cycle directed advertisement #define BSAL_GAP_ADTYPE_ADV_SCAN_IND                                          0x02      //!< Scannable undirected advertisement
#define BSAL_GAP_ADTYPE_ADV_NONCONN_IND                                       0x03      //!< Non-Connectable undirected advertisement
#define BSAL_GAP_ADTYPE_SCAN_RSP                                              0x04      //!< Connectable low duty cycle directed advertisement


//BD_ADDR_TYPE
typedef enum
{
    BSAL_GAP_REMOTE_ADDR_LE_PUBLIC          = 0x00, /**< LE Public device address type. */
    BSAL_GAP_REMOTE_ADDR_LE_RANDOM          = 0x01, /**< LE Random device address type. */
    BSAL_GAP_REMOTE_ADDR_LE_PUBLIC_IDENTITY = 0x02, /**< LE Public identity address type. */
    BSAL_GAP_REMOTE_ADDR_LE_RANDOM_IDENTITY = 0x03, /**< LE Random identity address type. */
} BSAL_BD_ADDR_TYPE;


//bond===
/** @defgroup BSAL_GAP_AUTH_TYPE GAP Common Macros
  * @{
  */
#define BSAL_GAP_AUTHEN_BIT_NO_BONDING            0      //!<  No authentication required.
#define BSAL_GAP_AUTHEN_BIT_BONDING_FLAG          0x0001 //!<  Bonding is required   
#define BSAL_GAP_AUTHEN_BIT_MITM_FLAG             0x0004 //!<  Mitm is preferred
#define BSAL_GAP_AUTHEN_BIT_SC_FLAG               0x0008 //!<  Secure connection is preferred
#define BSAL_GAP_AUTHEN_BIT_KEYPRESS_FLAG         0x0010 //!<  keypress

/** @brief I/O Capabilities */
typedef enum
{
    BSAL_GAP_IO_CAP_DISPLAY_ONLY,        //!<  Only a Display present, no Keyboard or Yes/No Keys.
    BSAL_GAP_IO_CAP_DISPLAY_YES_NO,      //!<  Display and Yes/No Keys present.
    BSAL_GAP_IO_CAP_KEYBOARD_ONLY,       //!<  Only a Keyboard present, no Display.
    BSAL_GAP_IO_CAP_NO_INPUT_NO_OUTPUT,  //!<  No input/output capabilities.
    BSAL_GAP_IO_CAP_KEYBOARD_DISPLAY,    //!<  Keyboard and Display present.
} BSAL_GAP_IO_CAP;

#ifdef __cplusplus
}
#endif
#endif

