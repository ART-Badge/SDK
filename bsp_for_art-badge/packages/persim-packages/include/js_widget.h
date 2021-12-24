#pragma once

#include "pm_widget.h"

void pm_widget_set_data(persim::Widget *widget, JsPage *page, const JsValue &data);
void pm_widget_get_data(const persim::Widget *widget, JsValue &ret, const persim::String &name);
