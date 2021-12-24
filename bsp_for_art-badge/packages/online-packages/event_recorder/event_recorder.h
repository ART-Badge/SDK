/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-09     armink       the first version
 */

#ifndef _EVENT_RECORDER_H_
#define _EVENT_RECORDER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdint.h>

#ifndef EVENT_SOURCE_MAX_NUM
#define EVENT_SOURCE_MAX_NUM 4
#endif

#ifndef EVENT_VALUE_MAX_SIZE
#define EVENT_VALUE_MAX_SIZE 32
#endif

int event_recorder_init(void);
int event_recorder_control(void);
int event_recorder_start(const char *recorder_dir, const char *recorder_name);
int event_recorder_stop(void);
int event_source_register(uint8_t id, const char *name,
        int (*replay)(uint8_t id, const uint8_t *value, size_t value_len));
int event_source_trigger(uint8_t event_id, uint8_t *event_value, size_t value_len);
int event_replay_start(const char *dir, const char *name, uint32_t time_s);
int event_replay_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* _EVENT_RECORDER_H_ */
