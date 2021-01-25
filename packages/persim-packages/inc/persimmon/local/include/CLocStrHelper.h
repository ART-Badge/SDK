#ifndef __CLOC_STR_HELPER__
#define __CLOC_STR_HELPER__

#include <string>
#include <vector>

typedef enum
{
    LOC_APP_CHANGE,
    LOC_LANG_CHANGE,
} LOC_EVENT;

class CSystemLang;

class ILocObserver
{
public:
    ILocObserver();
    virtual ~ILocObserver();
    virtual void on_local_change(LOC_EVENT event)  = 0;
};

class CLocString : public ILocObserver
{
public:
    CLocString();
    explicit CLocString(const char* str);
    virtual ~CLocString();
    virtual void on_local_change(LOC_EVENT event);
    const char* c_str(void);
    void SetStrName(const char* str);
    const char* GetStrName() const;
    int IsValid(void);

private:
    unsigned int str_id;
    const char* save_text;
    std::string str_name;
    std::string str_loc;
    int valid;
    std::string PatternReplace(const char* pattern, int pattern_len, const char* data, int data_len);

};

typedef struct
{
    unsigned int id;
    int local_str_offset;
    int local_str_length;
//    int default_str_offset;
//    int default_str_length;
} local_str_entity;

#include <rtgui/filerw.h>

class CLocStrHelper
{
private:
    int num_local_enities;
    rtgui_filerw_t* local_frw;
    local_str_entity* local_enities_cache;

    std::vector<ILocObserver*> loc_observer_list;
    CSystemLang* sysLang;

    void LoadLocalTable(void);
    void LoadLocalTable(void* res_list);

public:
    CLocStrHelper();
    CLocStrHelper(void* res_list);
    virtual ~CLocStrHelper();
    static CLocStrHelper* getInstance();
    void addILocObserver(ILocObserver *cLocStr);
    void rmILocObserver(ILocObserver *cLocStr);
    void notifyLocalChange(LOC_EVENT event);
    int findLocString(unsigned int id, std::string& loc_str);
    int findLocString(const std::string& id_str, std::string& loc_str);
    CSystemLang* GetCSystemLang(void);
};

void notifyLocalChange(LOC_EVENT event);

#endif
