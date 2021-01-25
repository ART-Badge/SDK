/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _OS_INTERFACE_EXTERN_H_
#define _OS_INTERFACE_EXTERN_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief                   get name of a task.
 *
 * \return                  The status of task name get.
 * \retval true             name get successfully.
 * \retval false            name get failed.
 *
 * \param[in]   p_handle    task handle. if NULL it means current task.
 *
 * \param[out]  p_task_name char pointer to save task name.
*/
bool os_task_name_get(void *p_handle, char **p_task_name);

/**
 * \brief                   Determine whether current code run in SwTimer task.
 *
 * \return                  The status deternination.
 * \retval true             code run in SwTimer task.
 * \retval false            code not run in SwTimer task.
*/
bool os_is_in_SwTimer_task(void);

/**
 * \brief                   Determine whether current code run in ISR.
 *
 * \return                  The status deternination.
 * \retval true             code run in ISR.
 * \retval false            code not run ISR.
*/
bool os_is_in_irq(void);

#ifdef __cplusplus
}
#endif

#endif /* _OS_INTERFACE_EXTERN_H_ */
