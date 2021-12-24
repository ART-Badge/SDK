#pragma once

#include "pm_rect.h"
#include "pm_string.h"
#include "pm_color.h"
#include "pm_font.h"
#include "pm_image.h"
#include "pm_widget.h"
#include <pm_translator.h>
#include <pm_layout.h>

class PldTypeParserPersimmon
{
public:
    int integer(const uint8_t *&data);
    float decimal(const uint8_t *&data);
    int alignment(const uint8_t *&data);
    persim::Size size(const uint8_t *&data);
    persim::Rect rectangle(const uint8_t *&data);
    persim::Color color(const uint8_t *&data);
    persim::String string(const uint8_t *&data);
    persim::Vector<persim::String> stringList(const uint8_t *&data);
    persim::Font font(const uint8_t *&data);
    persim::Image image(const uint8_t *&data);
    persim::Direction direction(const uint8_t *&data);
    persim::AxialLayout *AxialLayout(const uint8_t *&data);
    bool existing(const uint8_t *&data);
    int zero(const uint8_t *&data);

    static inline int int16(const uint8_t *&data)
    {
        int16_t i = (data[1] << 8) | data[0];
        data += 2;
        return i;
    }

    static inline int uint16(const uint8_t *&data)
    {
        uint16_t i = (data[1] << 8) | data[0];
        data += 2;
        return i;
    }
};
