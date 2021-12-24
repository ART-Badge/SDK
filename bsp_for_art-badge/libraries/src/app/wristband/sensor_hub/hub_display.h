#ifndef _RTL876X_MODULE_DISPLAY_H_
#define _RTL876X_MODULE_DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "board.h"
#include "app_msg.h"



#include "hub_clock.h"

#define WHITE                   0xffff
#define BLACK                   0x0000
#define BLUE                    0x001f
#define GREEN                   0x07e0
#define RED                     0xf800
#define YELLOW                  0xffe0
#define BROWN                   0xbc40

/* the color config from UC GUI*/
#define RGB565(R, G, B)                 ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3)
#define RGB565_RED              0xF800
#define RGB565_GREEN            0x07E0
#define RGB565_BLUE             0x001F
#define RGB565_WHITE            0xFFFF
#define RGB565_BLACK            0x0000
#define RGB565_CYAN             0x07FF
#define RGB565_YELLOW           0xFFE0
#define RGB565_PINK             0xF81F
#define PRESSING                0x07EF

#define HIGHLIGHT               RGB565(204, 204, 204)
#define LIGHT                   RGB565(77, 77, 77)
#define EXERCISE                RGB565(0x3a, 0x98, 0x1c)
#define PROGRESS                RGB565(204, 204, 204)
/* the color config from UC GUI*/

/****
unsigned LCD_Color2Index_M565(LCD_COLOR Color) {
  int r,g,b;
  r = (Color>> (8  - R_BITS)) & R_MASK;
  g = (Color>> (16 - G_BITS)) & G_MASK;
  b = (Color>> (24 - B_BITS)) & B_MASK;
  return b + (g << B_BITS) + (r << (G_BITS + B_BITS));
}
****/
#define GUI_BLUE                0xFF0000
#define GUI_GREEN               0x00FF00
#define GUI_RED                 0x0000FF
#define GUI_CYAN                0xFFFF00
#define GUI_MAGENTA             0xFF00FF
#define GUI_YELLOW              0x00FFFF
#define GUI_LIGHTBLUE           0xFF8080
#define GUI_LIGHTGREEN          0x80FF80
#define GUI_LIGHTRED            0x8080FF
#define GUI_LIGHTCYAN           0xFFFF80
#define GUI_LIGHTMAGENTA        0xFF80FF
#define GUI_LIGHTYELLOW         0x80FFFF
#define GUI_DARKBLUE            0x800000
#define GUI_DARKGREEN           0x008000
#define GUI_DARKRED             0x000080
#define GUI_DARKCYAN            0x808000
#define GUI_DARKMAGENTA         0x800080
#define GUI_DARKYELLOW          0x008080
#define GUI_WHITE               0xFFFFFF
#define GUI_LIGHTGRAY           0xD3D3D3
#define GUI_GRAY                0x808080
#define GUI_DARKGRAY            0x404040
#define GUI_BLACK               0x000000

#define GUI_BROWN               0x2A2AA5


typedef enum
{
    displayClock,
    displaySetting,
    displayStep,
    displayHeartRate,
    displayBloodPressure,
    displaySleep,
    displayHeadset,
    displayMessageBusiness,
    displayMessage,
    displayPowerOff,
    displayReset,
    displayDNDM,
    displayDeviceInfo,
    displayStopwatch,
    displayCountDown,
    displayFindPhone,
    displaySettingReturn,
    displayExercise,
    displayRunOutDoor,
    displayRideOutDoor,
    displayWalkOutDoor,
    displayRunInDoor,
    displayFreeTrain,
    displayPlank,
    displayExerciseReturn,
    displayLock,
    displayClockInfo,
} displayMenu;

typedef struct
{
    displayMenu cur_display_menu;
    displayMenu last_display_menu;
    uint8_t ActiveTime;
    bool DisplayActiveFlag;
} MenuStatus;

typedef struct
{
    //void (* display_init)(void);
    void (* display_power_on)(void);
    void (* display_power_off)(void);
    void (* display_reset)(void);
    void (* display_enable)(void);
    void (* display_clear)(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
} display_driver;



void wristband_display_driver_init(void);
void wristband_display_power_on(void);
void wristband_display_power_off(void);
void wristband_display_enter_dlps(void);
void wristband_display_exit_dlps(void);
bool wristband_lcd_backlight_allowed_enter_dlps_check(void);
void wristband_display_reset(void);
void wristband_display_update(void);
void wristband_display_enable(void);
void wristband_display_clear(uint16_t col, uint16_t row, uint16_t col_size, uint16_t row_size,
                             uint16_t color);
void display_add_hub_task(void);
void display_event_handler(T_IO_MSG msg);

extern MenuStatus wristbandMenuStatus;

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_MODULE_DISPLAY_H_ */
