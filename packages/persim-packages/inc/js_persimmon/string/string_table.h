#ifndef __STRING_TABLE_H__
#define __STRING_TABLE_H__

#include <stdint.h>
#include <rtdef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct string_table
{
    char *str;
    int str_len;
    int refer_count;
    void *value;
    struct string_table *next;
};

typedef void(*free_cb)(void *value);

struct string_table *string_table_create(void);
void string_table_destroy(struct string_table *str_table, free_cb callback);

int string_table_insert(struct string_table *str_table, const char *str, void *value);
void *string_table_remove(struct string_table *str_table, const char *str);

void *string_table_find(struct string_table *str_table, const char *str);
int string_table_put(struct string_table *str_table, void *value);

#ifdef __cplusplus
}
#endif

#endif
