/*
 * File      : pm_label.h
 * COPYRIGHT (C) 2012-2020, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date             Author          Notes
 * 2017-11-05       realthread      the first version
 */

#pragma once

#include <pm_widget.h>
#include <pm_timer.h>
#include <pm_translator.h>

namespace persim
{

class Label : public Widget
{
public:
    PM_DECLARE_CLASSNAME(Label)

    Label();
    virtual ~Label();
    void setPlainText(const String &text);
    void setExpanded(bool status);

    String getPlainText() const;
    void clear(void);

protected:
    virtual void paintEvent(PaintEvent *event);

private:
    bool m_elided, m_expanded;
    LocalString m_text;
};

} // namespace persim
