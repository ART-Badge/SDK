#pragma once

#include "pm_string.h"
#include "pm_rect.h"
#include "pm_vector.h"

PERSIMMON_NAMESPACE_BEGIN

class Font;
class Texture;
class FontDriver;

class FontDrawer
{
    struct LineInfo
    {
        int offset; // offset of the first character of the line
        // ....... CHAR ... CHAR ... CHAR .......
        //   ^           ^
        //   left        space  (Q26.6 pixels)
        int left, space;
        LineInfo() : offset(0), left(0), space(0) {}
        LineInfo(int l, int off, int spc) : offset(off), left(l), space(spc) {}
    };

public:
    class DrawIterator
    {
    public:
        DrawIterator();
        DrawIterator(const FontDrawer *drawer, const String &text, const Rect &box, int shift);
        int x() const { return m_paintX; }
        int y() const { return m_paintY; }
        Point topLeft() const;
        const Texture *bitmap() const { return m_bitmap; }
        bool next();
        uint32_t code() const { return m_code; }

    private:
        int m_offset;
        int16_t m_paintX, m_paintY;
        int m_layoutX, m_layoutY, m_shift;
        const FontDrawer *drawer;
        unicode_iterator m_uit, m_uend;
        const Texture *m_bitmap;
        uint32_t m_code;
        Vector<LineInfo>::const_iterator m_lineiter;
        int xbase;
    };

    FontDrawer(const Font &font, int flags, int lineHeight);
    void layoutRightToLeft();
    DrawIterator layout(const String &text, const Rect &box);
    int lineCount() const { return int(m_lines.size()); }

private:
    void pushLine(int offset, int space, int box, int tailspace);
    int layoutMultiLine(class Tokenizer *tokenizer, int box);
    int layoutSingleLine(const String &text, int box);

private:
    int m_lineHeight;
    FontDriver *driver;
    uint32_t m_flags : 16;
    uint32_t m_rtl : 1;
    Vector<LineInfo> m_lines;
};

PERSIMMON_NAMESPACE_END
