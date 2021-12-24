/*
 * File      : pm_barcode.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-05     realthread   the first version
 * 2021-02-02     realthread   Update to Persim v4.1.0
 */
#ifndef __PM_BARCODE_H
#define __PM_BARCODE_H

#include <pm_widget.h>
#include <pm_translator.h>

PERSIMMON_NAMESPACE_BEGIN

class BarCode : public Widget
{
public:
    PM_DECLARE_CLASSNAME(BarCode)

    BarCode();
    virtual ~BarCode();

    void setBarCode(const String &code);
    virtual void paintEvent(PaintEvent *event);

private:
    String data;
};

PERSIMMON_NAMESPACE_END

#endif
