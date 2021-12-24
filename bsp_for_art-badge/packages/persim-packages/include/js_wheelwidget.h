/*
 * File      : js_wheelwidget.h
 * COPYRIGHT (C) 2012-2021, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-06     realthread   the first version
 */
#pragma once

#include "js_utility.h"
#include <pm_wheelarc.h>

class JsWheelArcWidget : public persim::WheelArcWidget
{
public:
    JsWheelArcWidget();
    virtual ~JsWheelArcWidget();
    void onBindEvent(unsigned int, unsigned int);
    virtual void setData(JsPage *page, const JsValue &data);
    void bindChangeCallback(const JsEventCallback &cb);
    void enableSignalName(bool enable = true);

private:
    const char *getEventTypeName(unsigned int);
    JsEventCallback _changed;
    bool _enableSignalName;
};
