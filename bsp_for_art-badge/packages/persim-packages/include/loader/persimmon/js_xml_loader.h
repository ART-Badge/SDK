#pragma once

#include "loader/js_xml_loader.h"

// widgets headers
#include "pm_widget.h"
#include "js_animatedimage.h"
#include "js_arcbar.h"
#include "js_arcbar.h"
#include "js_barcode.h"
#include "js_button.h"
#include "js_canvaslite.h"
#include "js_card.h"
#include "js_checkbox.h"
#include "js_clock.h"
#include "js_dotindicator.h"
#include "js_imagebox.h"
#include "js_label.h"
#include "js_listctrl.h"
#include "js_multitextbox.h"
#include "js_page.h"
#include "js_panel.h"
#include "js_qrcode.h"
#include "js_slider.h"
#include "js_switch.h"
#include "js_textbox.h"
#include "js_wheelstring.h"
#include "js_wheelimage.h"
#include "js_wheelwidget.h"
#include "js_scrollbar.h"

USING_PERSIMMON_NAMESPACE;

extern "C" void UiXmlLoaderRegisterPersimmon();

class UiXmlLoaderWidget : public UiXmlLoader {
    typedef UiXmlLoader Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsAnimatedImage : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsArcBar : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsArcDotIndicator : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsBarCode : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsButton : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsCanvasLite : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsCard : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsCheckbox : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsClock : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsDotIndicator : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsImageBox : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsLabel : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsListCtrl : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsMultiTextBox : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsPage : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsPanel : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsQRCode : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsSlider : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsSwitch : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsTextBox : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsWheelString : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsWheelImage : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsWheelArcWidget : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

class UiXmlLoaderJsScrollBar : public UiXmlLoaderWidget {
    typedef UiXmlLoaderWidget Super;
public:
    virtual const char* typeName() const;
    virtual Widget* parse(const JsValue &obj, ezxml *xml);
protected:
    virtual void parseBind(const JsValue &obj, persim::Widget *w, const char *name, const char *value);
    virtual void parseProperty(Widget *w, const char *name, ezxml *xml);
};

