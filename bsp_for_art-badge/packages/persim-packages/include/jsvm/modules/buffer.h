#pragma once

#include "jsvm/jsvm.h"

namespace jsvm
{

class Buffer : public JsNativeClassT<Buffer>
{
public:
    enum EncodeType
    {
        Utf8,
        Ascii,
        Hex,
        Base64,
        Binary
    };
    class Encode
    {
        EncodeType _enc;

    public:
        Encode(EncodeType e = Utf8) : _enc(e) {}
        Encode(const Encode &e) : _enc(e._enc) {}
        bool isText() const { return _enc <= Ascii; }
        bool isBinary() const { return !isText(); }
        bool operator==(Encode rhs) { return _enc == rhs._enc; }
        bool operator!=(Encode rhs) { return !(*this == rhs); }
    };

    void setString(const String &str, size_t pos = 0);
    void setString(const char *str, size_t pos = 0);
    void setString(String::const_iterator first, String::const_iterator last, size_t pos = 0);
    void setHex(const String &str, size_t pos = 0);
    void setArray(const JsValue &array, size_t pos = 0);
    const String &data() const { return _data; }

    static Encode parseEncode(const String &name);
    static JsValue newBuffer(size_t size);
    static JsValue newBuffer(const String &str, Encode encode = Utf8);
    static JsValue newBuffer(const JsValue &value, Encode encode = Utf8);
    static Buffer *toBuffer(const JsValue &value);
    static bool isBuffer(const JsValue &value) { return toBuffer(value); }
    static void concat(JsValue dst, const String &data);
    static void concat(JsValue dst, String::const_iterator first, String::const_iterator last);

protected:
    Buffer(size_t size);

private:
    String _data;
    friend class JsBufferPrivate;
};

} // namespace jsvm
