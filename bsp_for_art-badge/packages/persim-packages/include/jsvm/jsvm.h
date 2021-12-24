#pragma once

#include "pm_string.h"
#include "pm_vector.h"
#include "pm_bytearray.h"

class JsValue;
class JsCallContext;
class JsPropertyDescriptor;

using persim::ByteArray;
using persim::String;
typedef persim::Vector<JsValue> JsValueList;
typedef JsValue (*JsNativeFunction)(const JsCallContext &context);

class JsFunction
{
public:
    virtual ~JsFunction();
    virtual JsValue operator()(const JsCallContext &context) = 0;
};

class JsNativeClass
{
public:
    virtual ~JsNativeClass() {}

private:
    JsNativeClass() {}
    virtual intptr_t _typeid() const = 0;
    template<class> friend class JsNativeClassT;
};

template<class T> class JsNativeClassT : public JsNativeClass
{
public:
    JsNativeClassT() : JsNativeClass() {}
    static inline T *cast(JsNativeClass *p)
    {
        return isInstance(p) ? static_cast<T *>(p) : nullptr;
    }
    static inline const T *cast(const JsNativeClass *p)
    {
        return isInstance(p) ? static_cast<const T *>(p) : nullptr;
    }

private:
    static inline bool isInstance(const JsNativeClass *p)
    {
        return p && p->_typeid() == T::_typeid_s();
    }
    static intptr_t _typeid_s()
    {
        static const char _static_id = 0;
        return intptr_t(&_static_id);
    }
    virtual intptr_t _typeid() const { return _typeid_s(); }
    friend class JsNativeClass;
};

class JsValue
{
public:
    enum DestoryMode
    {
        Constant,
        AutoDelete
    };
    enum ValueType
    {
        Undefined,
        Null,
        Boolean,
        Number,
        String,
        Object,
        Function,
        Error
    };
    JsValue();
    JsValue(int value);
    JsValue(double value);
    JsValue(bool value);
    JsValue(const ::String &value);
    JsValue(const char *value);
    JsValue(JsFunction *value);
    JsValue(const JsNativeFunction value);
    JsValue(const JsValue &value);
    ~JsValue();

    ValueType type() const;
    bool isUndefined() const;
    bool isNull() const;
    bool isBoolean() const;
    bool isNumber() const;
    bool isString() const;
    bool isArray() const;
    bool isObject() const;
    bool isFunction() const;
    bool isError() const;

    double toNumber() const;
    int toInt() const { return int(toNumber()); }
    float toFloat() const { return float(toNumber()); }
    bool toBoolean() const;
    ::String toString() const;
    JsValue errorValue() const;
    JsValue stringfyJSON() const;
    void print() const;
    bool reportError() const;

    JsValue prototype() const;
    bool setPrototype(const JsValue &value);
    bool setProperty(const JsValue &name, const JsValue &value);
    bool defineProperty(const JsValue &name, const JsPropertyDescriptor &descriptor);
    bool deleteProperty(const JsValue &name);
    bool hasProperty(const JsValue &name) const;
    bool hasOwnProperty(const JsValue &name) const;
    JsValue property(const JsValue &name) const;
    void setIndex(int index, const JsValue &value);
    JsValue indexOf(int index) const;
    size_t arrayLength() const;
    size_t stringLength() const;

    JsNativeClass *nativeClass() const;
    bool setNativeClass(JsNativeClass *object, DestoryMode mode = Constant);

    JsValue call(const JsValue &thisValue, const JsValueList &args = JsValueList()) const;
    JsValue call(const JsValue &thisValue, const JsValue *args, int argc) const;
    JsValue callMethod(const JsValue &name, const JsValueList &args = JsValueList()) const;
    JsValue callMethod(const JsValue &name, const JsValue *args, int argc) const;
    JsValue construct(const JsValueList &args = JsValueList()) const;
    JsValue construct(const JsValue *args, int argc) const;

    JsValue &operator=(const JsValue &value);

    // only for C++ container
    bool operator==(const JsValue &value) const { return _value == value._value; }
    bool operator!=(const JsValue &value) const { return !(*this == value); }
    bool operator<(const JsValue &value) const { return _value < value._value; }
    bool operator>(const JsValue &value) const { return value < *this; }
    bool operator<=(const JsValue &value) const { return !(*this > value); }
    bool operator>=(const JsValue &value) const { return !(*this < value); }

private:
    // Constructing from ambiguous pointers is not allowed,
    // otherwise `JsValue(bool)` will generally be called and cause bugs.
    JsValue(const void *);

private:
    uint32_t _value; // js value
    friend class JsValuePrivate;
};

class JsCallContext
{
public:
    const JsValue &function() const { return _func; }
    const JsValue &thisObject() const { return _this; }
    JsValue arg(int index) const;
    int argc() const { return _argc; }

private:
    JsValue _func, _this;
    const uint32_t *_args;
    int _argc;
    friend class JsValuePrivate;
};

class JsPropertyIterator
{
public:
    JsPropertyIterator(const JsValue &object);
    ~JsPropertyIterator();
    bool hasNext() const { return _data.values; }
    void next();
    JsValue name() const;
    JsValue value() const;

private:
    struct PrivateData
    {
        void *object, *names, *values;
    } _data;
};

class JsPropertyDescriptor
{
public:
    JsPropertyDescriptor();
    ~JsPropertyDescriptor();
    bool hasValue() const { return mode.hasValue; }
    bool hasGetter() const { return mode.hasGetter; }
    bool hasSetter() const { return mode.hasSetter; }
    bool hasWriteable() const { return mode.hasWriteable; }
    bool isWriteable() const { return mode.writeable; }
    bool hasEnumerable() const { return mode.hasEnumerable; }
    bool isEnumerable() const { return mode.enumerable; }
    bool hasConfigurable() const { return mode.hasConfigurable; }
    bool isConfigurable() const { return mode.configurable; }
    const JsValue &value() const { return _value; }
    const JsValue &setter() const { return _setter; }
    const JsValue &getter() const { return _getter; }
    void setWriteable(bool writeable);
    void setEnumerable(bool enumerable);
    void setConfigurable(bool configurable);
    void setValue(const JsValue &value);
    void setSetter(const JsValue &setter);
    void setGetter(const JsValue &getter);

private:
    struct
    {
        uint32_t hasValue : 1;
        uint32_t hasGetter : 1;
        uint32_t hasSetter : 1;
        uint32_t hasWriteable : 1;
        uint32_t writeable : 1;
        uint32_t hasEnumerable : 1;
        uint32_t enumerable : 1;
        uint32_t hasConfigurable : 1;
        uint32_t configurable : 1;
    } mode;
    JsValue _value, _setter, _getter;
};

class JsVM
{
public:
    JsVM() {}
    JsValue parse(const char *source, int length, const String &source_name = String());
    JsValue parse(const ByteArray &source, const String &source_name = String());
    JsValue parse(const String &source, const String &source_name = String());
    JsValue eval(const char *source, int length);
    JsValue eval(const String &source);
    JsValue run(const JsValue &code);
    JsValue parseJSON(const char *source, int length);
    JsValue parseJSON(const ByteArray &source);
    JsValue parseJSON(const String &source);
    JsValue globalObject() const;
    JsValue newNull();
    JsValue newObject();
    JsValue newArray(size_t size = 0);
    JsValue newError(const char *message);
    JsValue newError(const String &message) { return newError(message.c_str()); }
    void backtrace() const;
    static bool isBytecode(const String &source);
    static bool isBytecode(const ByteArray &source);
    static bool isBytecode(const char *source, size_t length);
    static JsValue makeValue(uint32_t value);
    static uint32_t takeValue(const JsValue &value);
};
