#pragma once

#include "pm_rect.h"
#include "pm_string.h"
#include "pm_color.h"
#include "pm_font.h"
#include "pm_image.h"
#include "pm_widget.h"
#include <pm_translator.h>
#include <pm_layout.h>

struct ezxml;

class XmlTypeParserPersimmon
{
public:
    int integer(ezxml *xml);
    float decimal(ezxml *xml);
    int alignment(ezxml *xml);
    persim::Size size(ezxml *xml);
    persim::Rect rectangle(ezxml *xml);
    persim::Color color(ezxml *xml);
    persim::String string(ezxml *xml);
    persim::Vector<persim::String> stringList(ezxml *xml);
    persim::Font font(ezxml *xml);
    persim::Image image(ezxml *xml);
    persim::Direction direction(ezxml *xml);
    persim::AxialLayout *AxialLayout(ezxml *xml);
    bool existing(ezxml *xml);
    int zero(ezxml *xml);
};
