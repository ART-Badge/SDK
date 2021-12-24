#pragma once

#include "loader/js_pld_loader.h"

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

extern "C" void UiPldLoaderRegisterPersimmon();

class UiPldLoaderWidget : public UiPldLoader {
    typedef UiPldLoader Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsAnimatedImage : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsArcBar : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsArcDotIndicator : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsBarCode : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsButton : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsCanvasLite : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsCard : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsCheckbox : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsClock : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsDotIndicator : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsImageBox : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsLabel : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsListCtrl : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsMultiTextBox : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsPage : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsPanel : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsQRCode : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsSlider : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsSwitch : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsTextBox : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsWheelString : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsWheelImage : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsWheelArcWidget : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

class UiPldLoaderJsScrollBar : public UiPldLoaderWidget {
    typedef UiPldLoaderWidget Super;
public:
    virtual int typeId() const;
    virtual Widget *parse(const JsValue &object, const uint8_t *&data);
protected:
    virtual void parseBind(const JsValue &object, Widget *w, const String &name, const String &value);
    virtual void parseProperty(Widget *w, int id, const uint8_t *&data);
};

