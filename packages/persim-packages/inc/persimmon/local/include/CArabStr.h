#ifndef __C_ARAB_STR_H__
#define __C_ARAB_STR_H__

class CArabStr
{
private:
    char* out_str;
    //revert arabic text
    void reorder(const char* in, char* out, int len);
    //handle arabic joint based on characters position
    void process_joint(const char* in, char* out, int len);
public:
    CArabStr(const char* str, int joint = 1);
    virtual ~CArabStr();
    const char* c_str();
};

#endif
