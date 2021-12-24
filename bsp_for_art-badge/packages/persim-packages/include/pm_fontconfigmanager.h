#pragma once

#include "pm_fontdriver.h"
#include "pm_string.h"
#include "pm_vector.h"
#include "pm_hashmap.h"

PERSIMMON_NAMESPACE_BEGIN

class FontConfigMap
{
public:
    enum Weight
    {
        Regular = FontDriver::Regular
    };
    enum Style
    {
        NormalStyle  = FontDriver::NormalStyle,
        ItalicStyle  = FontDriver::ItalicStyle,
        ObliqueStyle = FontDriver::ObliqueStyle
    };

    struct DefaultFace
    {
        String family;
        uint8_t style;
        uint8_t weight;
        uint16_t size;
    };
    class Face
    {
    public:
        Face(const String &family, Style style = NormalStyle, int weight = Regular);
        void pushPath(const String &path);
        void pushPath(const Vector<String> paths);
        const String &family() const { return data.family; }
        Style style() const { return Style(data.style); }
        int weight() const { return data.weight; }
        const Vector<String> &path() const { return data.paths; }
        uint32_t hashcode() const { return data.hash; }
        bool operator==(const Face &face) const;
        bool operator!=(const Face &face) const { return !(*this == face); }

    private:
        struct
        {
            String family;
            Vector<String> paths;
            uint16_t hash;
            uint8_t style;
            uint8_t weight;
            void *next;
        } data;
        friend class FontManagerHashMapHelper;
    };

    FontConfigMap();
    ~FontConfigMap();
    Face *pushFace(const Face &face);
    void setDefaultFont(const DefaultFace &face);
    const DefaultFace &defaultFace() const { return m_defaultFace; }
    const Face *select(const String &family, Style style, int weight) const;
    bool empty() const { return m_faces.empty(); }

private:
    HashMap<Face> m_faces;
    DefaultFace m_defaultFace;
    friend class FontConfigManager;
};

class FontConfigManager
{
public:
    const FontConfigMap::Face *select(const String &family, FontConfigMap::Style style,
                                      int weight) const;
    const FontConfigMap::DefaultFace *defaultFace() const;
    FontConfigMap *localConfig() const { return _localCfg; }
    FontConfigMap *globalConfig() const { return _globalCfg; }
    void setLocalConfig(FontConfigMap *map) { _localCfg = map; }
    void setGlobalConfig(FontConfigMap *map) { _globalCfg = map; }

    static FontConfigManager *instance();

protected:
    FontConfigManager();

private:
    FontConfigMap *_localCfg, *_globalCfg;
    static FontConfigManager *_instance;
};

PERSIMMON_NAMESPACE_END
