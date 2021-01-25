#pragma once

#include <persimmon.h>
#include "js_page.h"
#include <ezxml.h>

using namespace Persimmon;

int parseNumber(ezxml_t xml);
int parseRect(ezxml_t xml, Rect &rect);
rtgui_color_t parseColor(ezxml_t xml);
int parseFont(ezxml_t xml, char *family, int *size);
int parseAlign(const char *alignStr);
void parseAnim(ezxml_t xml, rt_uint32_t *animType, rt_uint32_t *frames, rt_uint32_t *interval);
rtgui_font_t *getFontFromXml(ezxml_t property);
void parseAttribute(ezxml_t xml, Widget* widget);
Widget* parseBase(JsPage *page, ezxml_t xml);

Widget* getPanel(js_value_t object);
void setDataForeach(const js_value_t data, Widget* widget);

class XmlRenderPrivate;

class XmlLoadWidget
{
public:
    XmlLoadWidget(JsPage *page);
    XmlLoadWidget(js_value_t object);
    XmlLoadWidget(JsPage *page, const char* filename, bool isLoadPage = true);
    ~XmlLoadWidget();

    Widget *loadJsWidget(const char* filename);
    Widget *loadJsPanel(Widget *panel, const char* filename);
    Widget *getJsWidget(void)
    {
        return jsWidget;
    }

    friend class XmlRenderPrivate;

protected:
    JsPage *jsPage;
    Widget *jsWidget;
    js_value_t object;
};
