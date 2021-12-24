#pragma once

#include <pm_string.h>
#include <pm_vector.h>
#include <pm_file.h>


class CLocString 
{
public:
    CLocString();
    explicit CLocString(const persim::String &str);
    virtual ~CLocString();
    const persim::String name_str(void) const { return str_name; }
    const persim::String loc_str(void);
    void SetStrName(const persim::String &str);
    const persim::String &GetStrName() const { return str_name; }
    void setValid(bool valid) 
    { 
        this->valid = valid;
    }
private:
    unsigned int str_id;
    persim::ResPack _res;
    persim::String save_text;
    persim::String str_name, str_loc;
    bool valid;
    static persim::String PatternReplace(const char *pattern, int pattern_len, const char *data,
                                         int data_len);
};

typedef struct
{
    unsigned int id;
    int local_str_offset;
    int local_str_length;
} local_str_entity;

class CLocStrHelper
{
public:
    CLocStrHelper();
    virtual ~CLocStrHelper();
    void addCLocString(CLocString *cLocStr);
    void removeCLocString(CLocString *cLocStr);
    void notifyLocalChange(void);
    std::pair<persim::String, bool> findLocString(int id);
    std::pair<persim::String, bool> findLocString(const persim::String &id, bool isId = false);
    void LoadLocalTable(const persim::ResPack &res);
    void clearLocalTable();

private:
    uint32_t num_local_enities;
    char *data_cache;
    persim::Vector<CLocString *> loc_string_list;
};

std::pair<persim::String, bool> findLocString(const persim::ResPack &res, const persim::String &id);
