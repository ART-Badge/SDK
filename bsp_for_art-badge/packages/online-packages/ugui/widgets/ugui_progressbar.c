/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-16     RTT       the first version
 */

#include "ugui_progressbar.h"

extern UG_OBJECT* _UG_SearchObject( UG_WINDOW* wnd, UG_U8 type, UG_U8 id );
extern UG_OBJECT* _UG_GetFreeObject( UG_WINDOW* wnd );

/* -------------------------------------------------------------------------------- */
/* -- PROGRESS BAR FUNCTIONS                                                     -- */
/* -------------------------------------------------------------------------------- */
static void _UG_ProgressBarPutText(UG_PROGRESS_BAR* pro, UG_TEXT* txt)
{
   UG_U16 sl,rc;
   UG_S16 xp,yp;
   UG_S16 xs=txt->a.xs;
   UG_S16 ys=txt->a.ys;
   UG_S16 xe=txt->a.xe;
   UG_S16 ye=txt->a.ye;
   UG_U8  align=txt->align;
   UG_S16 char_width=txt->font->char_width;
   UG_S16 char_height=txt->font->char_height;
   UG_S16 char_h_space=txt->h_space;
   UG_S16 char_v_space=txt->v_space;
   UG_U16 i,j,k,xo,yo,cw,bn;
   UG_U8  b,bt;
   UG_COLOR read = 0x00U;

   unsigned char* p;

   char* str = txt->str;
   char* c = str;

   if ( txt->font->p == NULL ) return;
   if ( str == NULL ) return;
   if ( (ye - ys) < txt->font->char_height ) return;

   rc=1;
   c=str;
   while ( *c != 0 )
   {
      if ( *c == '\n' ) rc++;
      c++;
   }

   yp = 0;
   if ( align & (ALIGN_V_CENTER | ALIGN_V_BOTTOM) )
   {
      yp = ye - ys + 1;
      yp -= char_height*rc;
      yp -= char_v_space*(rc-1);
      if ( yp < 0 ) return;
   }
   if ( align & ALIGN_V_CENTER ) yp >>= 1;
   yp += ys;

   while( 1 )
   {
      sl=0;
      c=str;
      while( (*c != 0) && (*c != '\n') )
      {
         c++;
         sl++;
      }

      xp = xe - xs + 1;
      xp -= char_width*sl;
      xp -= char_h_space*(sl-1);
      if ( xp < 0 ) return;

      if ( align & ALIGN_H_LEFT ) xp = 0;
      else if ( align & ALIGN_H_CENTER ) xp >>= 1;
      xp += xs;

      while( (*str != '\n') )
      {
         if ( *str == 0 ) return;
         /*----------------------------------*/
         /* Draw one char                    */
         /*----------------------------------*/
         bt = (UG_U8)*str;
         switch ( bt )
         {
            case 0xF6: bt = 0x94; break; // ö
            case 0xD6: bt = 0x99; break; // Ö
            case 0xFC: bt = 0x81; break; // ü
            case 0xDC: bt = 0x9A; break; // Ü
            case 0xE4: bt = 0x84; break; // ä
            case 0xC4: bt = 0x8E; break; // Ä
            case 0xB5: bt = 0xE6; break; // µ
            case 0xB0: bt = 0xF8; break; // °
         }
         yo = yp;
         bn = char_width;
         bn >>= 3;
         if ( char_width % 8 ) bn++;
         p = txt->font->p;
         p+= bt * char_height * bn;
         for( j=0;j<char_height;j++ )
         {
            xo = xp;
            cw=char_width;
            for( i=0;i<bn;i++ )
            {
               b = *p++;
               for( k=0;(k<8) && cw;k++ )
               {
                  if( b & 0x01 )
                  {
                     UG_GetGui()->pset(xo, yo, pro->font_color);
                  }
                  else
                  {
                     read = UG_ReadPixel(xo, yo);
                     if((read > 0) && (read != pro->font_color))
                     {
                         txt->bc = pro->fill_color;
                     }
                     else
                     {
                         txt->bc = pro->bc;
                     }
                     UG_GetGui()->pset(xo,yo,txt->bc);
                  }
                  b >>= 1;
                  xo++;
                  cw--;
               }
            }
            yo++;
         }
         /*----------------------------------*/
         xp += char_width + char_h_space;
         str++;
      }
      str++;
      yp += char_height + char_v_space;
   }
}

static void _UG_ProgressBarUpdate(UG_WINDOW* wnd, UG_OBJECT* obj)
{
   UG_PROGRESS_BAR* pro;
   UG_AREA a;
   UG_TEXT txt;
   int value = 0x00;
   int progress = 0x00;

   /* Get object-specific data */
   pro = (UG_PROGRESS_BAR*)(obj->data);

   /* -------------------------------------------------- */
   /* Object touch section                               */
   /* -------------------------------------------------- */

   /* Textbox doesn't support touch */

   /* -------------------------------------------------- */
   /* Object update section                              */
   /* -------------------------------------------------- */
   if ( obj->state & OBJ_STATE_UPDATE )
   {
      if ( obj->state & OBJ_STATE_VISIBLE )
      {
         /* Full redraw necessary? */
         if ( obj->state & OBJ_STATE_REDRAW )
         {
            UG_WindowGetArea(wnd,&a);
            obj->a_abs.xs = obj->a_rel.xs + a.xs;
            obj->a_abs.ys = obj->a_rel.ys + a.ys;
            obj->a_abs.xe = obj->a_rel.xe + a.xs;
            obj->a_abs.ye = obj->a_rel.ye + a.ys;
            if ( obj->a_abs.ye >= wnd->ye ) return;
            if ( obj->a_abs.xe >= wnd->xe ) return;

            txt.bc = pro->bc;
            txt.fc = C_WHITE;

            /* Draw Textbox text */
            txt.a.xs = obj->a_abs.xs + 1;
            txt.a.ys = obj->a_abs.ys + 1;
            txt.a.xe = obj->a_abs.xe - 1;
            txt.a.ye = obj->a_abs.ye - 1;
            txt.align = pro->align;
            txt.font = pro->font;
            txt.h_space = pro->h_space;
            txt.v_space = pro->v_space;
            txt.str = pro->value_str;
            
            UG_DrawFrame(obj->a_abs.xs, obj->a_abs.ys, obj->a_abs.xe, obj->a_abs.ye, pro->border_color);
            UG_FillFrame(obj->a_abs.xs + 1, obj->a_abs.ys + 1, obj->a_abs.xe - 1, obj->a_abs.ye - 1, pro->bc);
            
            _UG_ProgressBarPutText(pro, &txt);
            obj->state &= ~OBJ_STATE_REDRAW;
         }
         else
         {
            UG_DrawFrame(obj->a_abs.xs, obj->a_abs.ys, obj->a_abs.xe, obj->a_abs.ye, pro->border_color);
            UG_FillFrame(obj->a_abs.xs + 1, obj->a_abs.ys + 1, obj->a_abs.xe - 1, obj->a_abs.ye - 1, pro->bc);
            /* Draw progress */
            value = ((obj->a_abs.xe - 1) - (obj->a_abs.xs + 1));
            progress = (int)(((float)pro->value_s16 / (float)100) * (float)value);
            progress = progress + obj->a_abs.xs + 1;
            UG_FillFrame(obj->a_abs.xs + 1, obj->a_abs.ys + 1, progress, obj->a_abs.ye - 1, pro->fill_color);
            /* Draw Textbox text */
            txt.bc = pro->bc;
            txt.fc = pro->font_color;
            txt.a.xs = obj->a_abs.xs + 1;
            txt.a.ys = obj->a_abs.ys + 1;
            txt.a.xe = obj->a_abs.xe - 1;
            txt.a.ye = obj->a_abs.ye - 1;
            txt.align = pro->align;
            txt.font = pro->font;
            txt.h_space = pro->h_space;
            txt.v_space = pro->v_space;
            txt.str = pro->value_str;
            _UG_ProgressBarPutText(pro, &txt);
         }
      }
      else
      {
         UG_FillFrame(obj->a_abs.xs, obj->a_abs.ys, obj->a_abs.xe, obj->a_abs.ye, wnd->bc);
      }
      obj->state &= ~OBJ_STATE_UPDATE;
   }
}

UG_RESULT UG_ProgressBarGreate(UG_WINDOW* wnd, UG_PROGRESS_BAR* pro, UG_U8 id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye)
{
    UG_OBJECT* obj;

    obj = _UG_GetFreeObject( wnd );
    if ( obj == NULL ) return UG_RESULT_FAIL;

    /* Initialize object-specific parameters */
    memset(pro->value_str, 0x00, sizeof(pro->value_str));
    snprintf(pro->value_str, sizeof(pro->value_str), "%d%%", 0);
    pro->v_space = 2;
    pro->h_space = 2;
    pro->align = ALIGN_CENTER;
    pro->bc = wnd->bc;
    pro->fc = wnd->fc;
    pro->font_color = wnd->bc;
    pro->fill_color = wnd->bc;
    pro->border_color = wnd->fc;
    pro->style = BTN_STYLE_2D;
    pro->font = NULL;
    pro->value_s16 = 0x00;

    /* Initialize standard object parameters */
    obj->update = _UG_ProgressBarUpdate;
    obj->touch_state = OBJ_TOUCH_STATE_INIT;
    obj->type = OBJ_TYPE_PROGRESSBAR;
    obj->event = OBJ_EVENT_NONE;
    obj->a_rel.xs = xs;
    obj->a_rel.ys = ys;
    obj->a_rel.xe = xe;
    obj->a_rel.ye = ye;
    obj->a_abs.xs = -1;
    obj->a_abs.ys = -1;
    obj->a_abs.xe = -1;
    obj->a_abs.ye = -1;
    obj->id = id;
    obj->state |= OBJ_STATE_VISIBLE | OBJ_STATE_REDRAW | OBJ_STATE_VALID;
    obj->data = (void*)pro;

    /* Update function: Do your thing! */
    obj->state &= ~OBJ_STATE_FREE;

    return UG_RESULT_OK;
}

UG_RESULT UG_ProgressBarSetValue(UG_WINDOW* wnd, UG_U8 id, UG_S16 value)
{
    UG_OBJECT* obj = NULL;
    UG_PROGRESS_BAR* pro = NULL;
    
    obj = _UG_SearchObject( wnd, OBJ_TYPE_PROGRESSBAR, id);
    if ( obj != NULL )
    {
       pro = (UG_PROGRESS_BAR*)(obj->data);
       pro->value_s16 = value;
       snprintf(pro->value_str, sizeof(pro->value_str), "%d%%", value);
       obj->state = obj->state | OBJ_STATE_UPDATE;
    }
    else
    {
        return UG_RESULT_FAIL;
    }
    return UG_RESULT_OK;
}

UG_RESULT UG_ProgressBarSetFont(UG_WINDOW* wnd, UG_U8 id, const UG_FONT* font)
{
    UG_OBJECT* obj = NULL;
    UG_PROGRESS_BAR* pro = NULL;
    
    obj = _UG_SearchObject( wnd, OBJ_TYPE_PROGRESSBAR, id);
    if ( obj != NULL )
    {
       pro = (UG_PROGRESS_BAR*)(obj->data);
       pro->font = font;
       obj->state = obj->state | OBJ_STATE_UPDATE;
    }
    else
    {
        return UG_RESULT_FAIL;
    }
    return UG_RESULT_OK;
}

UG_RESULT UG_ProgressBarSetFontColor(UG_WINDOW* wnd, UG_U8 id, UG_COLOR c)
{
    UG_OBJECT* obj = NULL;
    UG_PROGRESS_BAR* pro = NULL;
    
    obj = _UG_SearchObject( wnd, OBJ_TYPE_PROGRESSBAR, id);
    if ( obj != NULL )
    {
       pro = (UG_PROGRESS_BAR*)(obj->data);
       pro->font_color = c;
       obj->state = obj->state | OBJ_STATE_UPDATE;
    }
    else
    {
        return UG_RESULT_FAIL;
    }
    return UG_RESULT_OK;
}

UG_RESULT UG_ProgressBarSetBorderColor(UG_WINDOW* wnd, UG_U8 id, UG_COLOR c)
{
    UG_OBJECT* obj = NULL;
    UG_PROGRESS_BAR* pro = NULL;
    
    obj = _UG_SearchObject( wnd, OBJ_TYPE_PROGRESSBAR, id);
    if ( obj != NULL )
    {
       pro = (UG_PROGRESS_BAR*)(obj->data);
       pro->border_color = c;
       obj->state = obj->state | OBJ_STATE_UPDATE;
    }
    else
    {
        return UG_RESULT_FAIL;
    }
    return UG_RESULT_OK;
}

UG_RESULT UG_ProgressBarSetFillColor(UG_WINDOW* wnd, UG_U8 id, UG_COLOR c)
{
    UG_OBJECT* obj = NULL;
    UG_PROGRESS_BAR* pro = NULL;
    
    obj = _UG_SearchObject( wnd, OBJ_TYPE_PROGRESSBAR, id);
    if ( obj != NULL )
    {
       pro = (UG_PROGRESS_BAR*)(obj->data);
       pro->fill_color = c;
       obj->state = obj->state | OBJ_STATE_UPDATE;
    }
    else
    {
        return UG_RESULT_FAIL;
    }
    return UG_RESULT_OK;
}
