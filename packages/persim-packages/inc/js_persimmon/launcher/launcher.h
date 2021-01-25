#ifndef __LAUNCHER_H__
#define __LAUNCHER_H__

#ifdef __cplusplus
extern "C" {
#endif

const char *get_script_path(void);
int launch_file(const char *fn);
int launcher_app(const char *url);
int launcher(const char *url);
int launcher_init(void);
int launcher_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
