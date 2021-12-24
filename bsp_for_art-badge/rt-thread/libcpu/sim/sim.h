/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-18     tyx          first implementation
 */

#ifndef __SIM_H__
#define __SIM_H__

#include <sim_type.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SIM_PATH_MAXN        (4096)
#define SIM_INTERRUPT_MAX    (256)

#define SIM_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))
#define SIM_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

#define SIM_WAITING_FOREVER              (-1)
#define SIM_WAITING_NO                   (0)

/* mem api */
sim_pointer_t sim_malloc(sim_size_t size);
void sim_free(sim_pointer_t ptr);

// thread def
typedef sim_object_t sim_thread_t;
// thread api
sim_err_t sim_thread_init(sim_thread_t* thread, int (*func)(sim_pointer_t),
    sim_pointer_t priv, sim_size_t stack_size);
void sim_thread_deinit(sim_thread_t* thread);
sim_err_t sim_thread_suspend(sim_thread_t* thread);
sim_err_t sim_thread_resume(sim_thread_t* thread);
sim_long_t sim_thread_identifier(sim_thread_t* thread);
sim_err_t sim_thread_setaffinity(sim_thread_t* thread, sim_ulong_t affinity_mask);
sim_ulong_t sim_thread_getaffinity(sim_thread_t* thread);
sim_err_t sim_thread_setname(sim_thread_t* thread, sim_char_t const* name);

typedef sim_object_t sim_context_t;
// thread context
sim_err_t sim_context_init(sim_context_t* ctx);
void sim_context_deinit(sim_context_t* ctx);
sim_err_t sim_context_get(sim_context_t* ctx, sim_thread_t* thread);
sim_err_t sim_context_set(sim_context_t* ctx, sim_thread_t* thread);

//time
void sim_usleep(sim_time_t us);
void sim_msleep(sim_time_t ms);
void sim_sleep(sim_time_t s);
sim_time_t sim_mtime(void);
sim_time_t sim_utime(void);

// timer def
typedef sim_object_t sim_timer_t;
//timer api
sim_err_t sim_timer_init(sim_timer_t* timer, sim_time_t time, sim_bool_t periodic,
    void(*func)(sim_pointer_t), sim_pointer_t priv);
void sim_timer_deinit(sim_timer_t* timer);
sim_err_t sim_timer_stop(sim_timer_t* timer);
sim_err_t sim_timer_start(sim_timer_t* timer);
sim_bool_t sim_timer_isactive(sim_timer_t* timer);

//semaphore def
typedef sim_object_t sim_sem_t;
//semaphore api
sim_err_t sim_sem_init(sim_sem_t* sem, sim_size_t value);
void sim_sem_deinit(sim_sem_t* sem);
sim_err_t sim_sem_take(sim_sem_t* sem, sim_time_t timeout);
sim_err_t sim_sem_release(sim_sem_t* sem);

//mutex def
typedef sim_object_t sim_mutex_t;
//mutex api
sim_err_t sim_mutex_init(sim_mutex_t* mutex);
void sim_mutex_deinit(sim_mutex_t* mutex);
sim_err_t sim_mutex_get(sim_mutex_t* mutex, sim_time_t timeout);
sim_err_t sim_mutex_put(sim_mutex_t* mutex);

//critical def
typedef sim_object_t sim_critical_t;
//critical
sim_err_t sim_critical_init(sim_critical_t* critical);
void sim_critical_deinit(sim_critical_t* critical);
sim_err_t sim_critical_enter(sim_critical_t* critical);
sim_err_t sim_critical_leave(sim_critical_t* critical);

//atomic32 def
typedef sim_long_t volatile sim_atomic_t;
//atomic api
sim_long_t sim_atomic_compare_and_swap(sim_atomic_t* des, sim_long_t cmp, sim_long_t exc);

//cpu api
void sim_cpu_yield(void);
sim_size_t sim_cpu_count(void);

//event def
typedef sim_object_t sim_event_t;
//event api
sim_err_t sim_event_init(sim_event_t* event, sim_bool_t auto_clear);
void sim_event_deinit(sim_event_t* event);
sim_err_t sim_event_wait(sim_event_t* event, sim_time_t timeout);
sim_err_t sim_event_set(sim_event_t* event);
sim_err_t sim_event_reset(sim_event_t* event);

//queue def
typedef sim_object_t sim_queue_t;

//queue api
sim_err_t sim_queue_init(sim_queue_t* queue, sim_size_t msg_size);
void sim_queue_deinit(sim_queue_t* queue);
sim_size_t sim_queue_push(sim_queue_t* queue, sim_pointer_t *msg, sim_size_t size);
sim_size_t sim_queue_pop(sim_queue_t* queue, sim_pointer_t* msg, sim_size_t size, sim_time_t timeout);
sim_size_t sim_queue_len(sim_queue_t* queue);

//console def
#define SIM_STDOUT  1
#define SIM_STDIN   2
#define SIM_STDERR  3

typedef sim_object_t sim_console_t;

//console api
sim_err_t sim_console_redirect(sim_console_t* console, sim_type_t type);
sim_size_t sim_console_read(sim_console_t* console, sim_byte_t* data, sim_size_t max_size);
sim_size_t sim_console_write(sim_console_t* console, sim_byte_t* data, sim_size_t size);

//std api
sim_size_t sim_stdout(sim_byte_t* data, sim_size_t size);
sim_size_t sim_stderr(sim_byte_t* data, sim_size_t size);
sim_size_t sim_stdint(sim_byte_t* data, sim_size_t max_size);

//file def
#define SIM_FILE_MODE_IFMT   0xF000 // File type mask
#define SIM_FILE_MODE_IFDIR  0x4000 // Directory
#define SIM_FILE_MODE_IFCHR  0x2000 // Character special
#define SIM_FILE_MODE_IFIFO  0x1000 // Pipe
#define SIM_FILE_MODE_IFREG  0x8000 // Regular
#define SIM_FILE_MODE_IREAD  0x0100 // Read permission, owner
#define SIM_FILE_MODE_IWRITE 0x0080 // Write permission, owner
#define SIM_FILE_MODE_IEXEC  0x0040 // Execute/search permission, owner

#define SIM_FILE_SEEK_CUR    1
#define SIM_FILE_SEEK_END    2
#define SIM_FILE_SEEK_SET    0

#define SIM_FILE_O_RDONLY      0x0000  // open for reading only
#define SIM_FILE_O_WRONLY      0x0001  // open for writing only
#define SIM_FILE_O_RDWR        0x0002  // open for reading and writing
#define SIM_FILE_O_APPEND      0x0008  // writes done at eof
#define SIM_FILE_O_CREAT       0x0100  // create and open file
#define SIM_FILE_O_TRUNC       0x0200  // open and truncate
#define SIM_FILE_O_EXCL        0x0400  // open only if file doesn't already exist

typedef struct __sim_file_stat_t
{
    sim_flag_t mode;
    sim_uint64_t size;
    sim_time_t atime;
    sim_time_t mtime;
    sim_time_t ctime;
}sim_file_stat_t;

typedef sim_object_t sim_file_t;
//file api

sim_err_t sim_file_open(sim_file_t* file, sim_char_t const* path, sim_flag_t oflags);
void sim_file_close(sim_file_t* file);
sim_int64_t sim_file_read(sim_file_t* file, sim_pointer_t buff, sim_size_t size);
sim_int64_t sim_file_write(sim_file_t* file, sim_cpointer_t buff, sim_size_t size);
sim_int64_t sim_file_seek(sim_file_t* file, sim_off_t offset, sim_flag_t mode);
sim_err_t sim_file_flush(sim_file_t* file);
sim_err_t sim_file_rename(sim_char_t const* new_path, sim_char_t const* old_path);
sim_err_t sim_file_remove(sim_char_t const* path);
sim_err_t sim_file_stat(sim_char_t const* path, sim_file_stat_t* stat);

//directory def
typedef struct __sim_directory_space_t
{
    sim_uint64_t blocks;
    sim_uint64_t bfree;
    sim_uint64_t bsize;
}sim_dir_space_t;

typedef sim_bool_t(*sim_dir_walk_func_t)(sim_char_t const*, sim_file_stat_t*, sim_pointer_t);

//directory api
sim_err_t sim_directory_create(sim_char_t const* path, sim_bool_t recursion);
sim_err_t sim_directory_delete(sim_char_t const* path);
sim_err_t sim_directory_space(sim_char_t const* path, sim_dir_space_t* space);
sim_err_t sim_directory_getfiles(sim_char_t const* path, sim_dir_walk_func_t func, sim_pointer_t priv);

//spilock
typedef struct __sim_spinlock_t
{
    sim_atomic_t lock;
}sim_spinlock_t;

void sim_spinlock_init(sim_spinlock_t* lock);
void sim_spinlock_lock(sim_spinlock_t* lock);
void sim_spinlock_unlock(sim_spinlock_t* lock);
sim_bool_t sim_spinlock_islock(sim_spinlock_t* lock);
sim_bool_t sim_spinlock_trylock(sim_spinlock_t* lock);

//systick api
void sim_systick_init(sim_time_t tick_per);
void sim_systick_deinit(void);

//interrupt api
void sim_interrupt_server_run(void);
void sim_interrupt_server_stop(void);
void sim_interrupt_enable(long level);
long sim_interrupt_disable(void);
void sim_interrupt_trigger(long index);
void sim_interrupt_schedule_trigger(void);
void sim_interrupt_systick_trigger(void);
void sim_interrupt_schedule_handler(void);
void sim_interrupt_dispatch(long vector);
void sim_interrupt_systick(void);

void sim_interrupt_context_load(sim_handle_t handle[64]);
void sim_interrupt_context_store(sim_handle_t handle[64]);

void sim_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif
