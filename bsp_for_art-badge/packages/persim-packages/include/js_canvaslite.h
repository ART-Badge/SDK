#ifndef __JS_CANVASLITE_H
#define __JS_CANVASLITE_H

#include <pm_widget.h>
#include <jsvm/jsvm.h>

namespace persim
{

class CanvasContext;

class JsCanvasLite : public Widget
{
public:
    PM_DECLARE_CLASSNAME(JsCanvasLite)

    JsCanvasLite();
    virtual ~JsCanvasLite();

    CanvasContext *context() { return m_ctx; }
    void draw();
    void setCanvasBuffer(bool enabled);
    virtual void paintEvent(PaintEvent *event);
    virtual void setData(JsPage *page, const JsValue &data);

private:
    void drawOnBuffer();
    void updateCanvasBuffer();

private:
    CanvasContext *m_ctx, *m_drawCtx;
};

}; // namespace persim

#endif // ! __JS_CANVASLITE_H
