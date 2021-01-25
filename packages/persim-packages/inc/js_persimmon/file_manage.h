#ifndef FILE_MANAGE_H__
#define FILE_MANAGE_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef void(*ls_func)(const char *path, const char *name, void *parameter);

int file_len(const char *name);
rt_bool_t make_dir(const char *path);
int directory_dirs(const char *path, rt_bool_t recursive);
int directory_files(const char *path, rt_bool_t recursive);
rt_bool_t dir_is_empty(const char *path);
int directory_ls_dirs(const char *path, ls_func ls, void *parameter);
int directory_ls_files(const char *path, ls_func ls, void *parameter);
rt_bool_t directory_delete(const char *path);

#ifdef __cplusplus
}
#endif

#endif
