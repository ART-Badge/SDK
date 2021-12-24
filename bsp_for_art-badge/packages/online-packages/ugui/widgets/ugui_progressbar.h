/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-16     RTT       the first version
 */

#ifndef __UGUI_PROGRESSBAR_H
#define __UGUI_PROGRESSBAR_H

#include "ugui.h"
#include "ugui_port.h"

/* Currently supported objects */
#define OBJ_TYPE_PROGRESSBAR                          4

/* -------------------------------------------------------------------------------- */
/* -- PROGRESSBAR OBJECT                                                             -- */
/* -------------------------------------------------------------------------------- */
/* Progress bar structure */
typedef struct
{
   const UG_FONT* font;
   UG_U8 style;
   UG_COLOR fc;
   UG_COLOR bc;
   UG_COLOR font_color;
   UG_COLOR border_color;
   UG_COLOR fill_color;
   UG_U8 align;
   UG_S8 h_space;
   UG_S8 v_space;
   UG_S16 value_s16;
   char value_str[32];
} UG_PROGRESS_BAR;

/* Default progress bar IDs */
#define PROGRESS_BAR_ID_0                                      OBJ_ID_0
#define PROGRESS_BAR_ID_1                                      OBJ_ID_1
#define PROGRESS_BAR_ID_2                                      OBJ_ID_2
#define PROGRESS_BAR_ID_3                                      OBJ_ID_3
#define PROGRESS_BAR_ID_4                                      OBJ_ID_4
#define PROGRESS_BAR_ID_5                                      OBJ_ID_5
#define PROGRESS_BAR_ID_6                                      OBJ_ID_6
#define PROGRESS_BAR_ID_7                                      OBJ_ID_7
#define PROGRESS_BAR_ID_8                                      OBJ_ID_8
#define PROGRESS_BAR_ID_9                                      OBJ_ID_9
#define PROGRESS_BAR_ID_10                                     OBJ_ID_10
#define PROGRESS_BAR_ID_11                                     OBJ_ID_11
#define PROGRESS_BAR_ID_12                                     OBJ_ID_12
#define PROGRESS_BAR_ID_13                                     OBJ_ID_13
#define PROGRESS_BAR_ID_14                                     OBJ_ID_14
#define PROGRESS_BAR_ID_15                                     OBJ_ID_15
#define PROGRESS_BAR_ID_16                                     OBJ_ID_16
#define PROGRESS_BAR_ID_17                                     OBJ_ID_17
#define PROGRESS_BAR_ID_18                                     OBJ_ID_18
#define PROGRESS_BAR_ID_19                                     OBJ_ID_19

/* -------------------------------------------------------------------------------- */
/* -- PROTOTYPES                                                                 -- */
/* -------------------------------------------------------------------------------- */
/* Progress Bar functions */
UG_RESULT UG_ProgressBarGreate(UG_WINDOW* wnd, UG_PROGRESS_BAR* pro, UG_U8 id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye);
UG_RESULT UG_ProgressBarSetValue(UG_WINDOW* wnd, UG_U8 id, UG_S16 value);
UG_RESULT UG_ProgressBarSetFont(UG_WINDOW* wnd, UG_U8 id, const UG_FONT* font);
UG_RESULT UG_ProgressBarSetFontColor(UG_WINDOW* wnd, UG_U8 id, UG_COLOR c);
UG_RESULT UG_ProgressBarSetBorderColor(UG_WINDOW* wnd, UG_U8 id, UG_COLOR c);
UG_RESULT UG_ProgressBarSetFillColor(UG_WINDOW* wnd, UG_U8 id, UG_COLOR c);

#endif
