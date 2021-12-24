
#ifndef LANGUAGE_CREATOR_H__
#define LANGUAGE_CREATOR_H__

#ifdef __cplusplus

#include <map>
#include <string>

class AbsLanguage
{
public:
    AbsLanguage() {}
    ~AbsLanguage(){};
    virtual const char *strProcess(const char *str) const = 0;
};

class ArabLanguage : public AbsLanguage
{
public:
    ~ArabLanguage();
    /* convert string */
    virtual const char *strProcess(const char *str) const;
};

class LanguageFactory
{
public:
    static LanguageFactory *getInstance();
    virtual ~LanguageFactory();
    AbsLanguage *findLanguage(std::string language_type);
    void registerLanguage(std::string language_type, AbsLanguage *language);

private:
    static LanguageFactory *factory;
    std::map<std::string, AbsLanguage *> language_map;
};

#endif

#ifdef __cplusplus
extern "C" {
#endif

void registerArabLanguage(void);

#ifdef __cplusplus
}
#endif

#endif
