#pragma once

#include "pm_uri.h"
#include "jsvm/jsvm.h"

namespace JsUri
{
JsValue fromUri(const persim::Uri &uri);
persim::Uri toUri(const JsValue &value);
} // namespace JsUri
