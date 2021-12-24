#pragma once

#include <pm_animator.h>

PERSIMMON_NAMESPACE_BEGIN

class Widget;
class Painter;

class AbstractSwitcher
{
public:
    virtual ~AbstractSwitcher();
    Size size() const { return m_size; }
    Widget *front() const { return m_front; }
    Widget *rear() const { return m_rear; }
    Widget *host() const { return m_host; }
    void setSize(const Size &size) { m_size = size; }
    bool setPair(Widget *front, Widget *rear);
    void setProgress(float progress);
    float progress() const { return m_progress; }
    bool isBufferable() { return m_bufferable; }
    bool isBuffered() const { return m_bufferable && m_buffered; }
    virtual void render(Painter &painter);

protected:
    AbstractSwitcher(Widget *host, bool bufferable = false);
    virtual void scroll(float progress) = 0;
    virtual bool bufferRequest();
    virtual bool bufferRelease();

private:
    Size m_size;
    float m_progress;
    const bool m_bufferable;
    bool m_buffered;
    Pointer<Widget> m_front, m_rear, m_host;
};

class ScrollSwitcher : public AbstractSwitcher
{
public:
    enum Direction
    {
        Horizontal = 0,
        Vertical
    };
    ScrollSwitcher(Direction direction, Widget *host);
    Direction direction() const;

protected:
    ScrollSwitcher(Direction direction, Widget *host, bool bufferable);
    virtual void scroll(float progress);

private:
    uint32_t m_direction : 1;
};

class BufferScrollSwitcher : public ScrollSwitcher
{
public:
    BufferScrollSwitcher(Direction direction, Widget *host);
    virtual ~BufferScrollSwitcher();
    virtual void render(Painter &painter);

protected:
    virtual void scroll(float progress);
    virtual bool bufferRequest();
    virtual bool bufferRelease();

protected:
    class Pixmap *m_front, *m_rear;
};

class ArcBufferScrollSwitcher : public BufferScrollSwitcher       /* 相比 BufferScrollSwitcher 多一个矩阵变换运算 */
{
public:
    ArcBufferScrollSwitcher(Direction direction, Widget *host);
    virtual ~ArcBufferScrollSwitcher();
    virtual void render(Painter &painter);

protected:
    virtual bool bufferRequest();

    
};

PERSIMMON_NAMESPACE_END
