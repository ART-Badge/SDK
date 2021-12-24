/*
 * File      : pm_qrcode.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-21     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include "pm_pixmap.h"

namespace persim
{

class QRCode : public Widget
{
public:
    QRCode();
    virtual ~QRCode();

    void setVersion(int version);
    void setEclevel(int eclevel);
    int setQRCode(const String &source);

    virtual void paintEvent(PaintEvent *event);

private:
    Pixmap *pixmap;
    int version, eclevel;
};

} // namespace persim
