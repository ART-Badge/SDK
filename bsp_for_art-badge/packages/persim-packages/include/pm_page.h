/*
 * File      : pm_page.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-03-06     realthread   the first version
 */

#pragma once

#include <pm_widget.h>
#include <pm_pixmapanimator.h>

namespace persim
{

class Page : public PixmapAnimator
{
public:
    PM_DECLARE_CLASSNAME(Page)

    Page(Widget *parent = nullptr);
    virtual ~Page();

    void setPriority(int priority) { m_priority = priority; }
    int priority() const { return m_priority; }

private:
    int m_priority;
};

} // namespace persim
