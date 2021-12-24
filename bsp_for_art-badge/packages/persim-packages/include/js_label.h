#pragma once

#include <pm_label.h>
#include <pm_timer.h>

class JsLabel : public persim::Label
{
public:
    JsLabel();
    JsLabel(const JsLabel &other);
    virtual ~JsLabel() {}

    void setPlainText(const persim::String &text);
    void setElided(bool enabled);

    virtual void setData(JsPage *page, const JsValue &data);
    virtual void getData(JsValue &ret, const persim::String &name) const;

    void enableScroll(bool scroll);
    void setScroll(int pitch, int ms);

protected:
    virtual void paintEvent(persim::PaintEvent *event);

private:
    void onTimeout();
    void triggerScroll();

private:
    bool m_elided, m_scrolled;
    int m_scrollPitch, m_textLength, m_scrollOffset, m_scrollInterval;
    persim::Timer m_timer;
};
