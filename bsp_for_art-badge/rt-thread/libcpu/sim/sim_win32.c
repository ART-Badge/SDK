/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-18     tyx          first implementation
 */

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <io.h>
#include <fcntl.h>
#include <sim.h>

/*
 * @param des           Des pointer
 * @param cmp           Compare values, and exchange occurs when the values are the same
 * @param exc           Exchange value
 * 
 * @return              Error code, SIM_OK indicates success
 */
sim_long_t sim_atomic_compare_and_swap(sim_atomic_t *des, sim_long_t cmp, sim_long_t exc)
{
    return InterlockedCompareExchange(des, exc, cmp);
}

#define WIN32_PIPE_SIZE     (16384)
#define WIN32_PIPE_WRITE    (1)
#define WIN32_PIPE_READ     (0)

typedef struct __win32_std_object_t
{
    int fd;
    int pipe[2];
    sim_console_t* owner;
    sim_type_t type;
}win32_std_t;

static win32_std_t win32_stdout;
static win32_std_t win32_stdin;
static win32_std_t win32_stderr;

void sim_console_lowlevel_init(void)
{
    AllocConsole();
    // get std fd
    win32_stdout.fd = _dup(_fileno(stdout));
    win32_stdout.type = SIM_STDOUT;
    win32_stdin.fd = _dup(_fileno(stdin));
    win32_stdin.type = SIM_STDIN;
    win32_stderr.fd = _dup(_fileno(stderr));
    win32_stderr.type = SIM_STDERR;
}

void sim_console_lowlevel_deinit(void)
{

}

static win32_std_t* win32_std_object_get(sim_type_t type)
{
    // get std object
    switch (type)
    {
    case SIM_STDOUT:
        return &win32_stdout;
    case SIM_STDIN:
        return &win32_stdin;
    case SIM_STDERR:
        return &win32_stderr;
    default:
        return NULL;
    }
}

sim_err_t sim_console_redirect(sim_console_t* console, sim_type_t type)
{
    win32_std_t* std_object = win32_std_object_get(type);
    int res;

    if (std_object == NULL)
    {
        return -SIM_INVAL;
    }
    if (console == NULL)
    {
        if (std_object->owner == NULL)
        {
            return -SIM_ERROR;
        }
        //set default console handle
        res = _dup2(std_object->fd, _fileno(stdout));
        assert(res == 0);
        //close pipe
        if (std_object->pipe[0] > 0)
        {
            res = _close(std_object->pipe[WIN32_PIPE_READ]);
            assert(res == 0);
            std_object->pipe[0] = 0;
        }
        if (std_object->pipe[WIN32_PIPE_WRITE] > 0)
        {
            res = _close(std_object->pipe[WIN32_PIPE_WRITE]);
            assert(res == 0);
            std_object->pipe[WIN32_PIPE_WRITE] = 0;
        }
        std_object->owner = NULL;
        return SIM_OK;
    }
    else
    {
        if (std_object->owner != NULL)
        {
            return -SIM_BUSY;
        }
        std_object->owner = console;
        res = _pipe(std_object->pipe, WIN32_PIPE_SIZE, O_BINARY | O_NOINHERIT);
        assert(res == 0);
        //set pipe to console handle
        if (std_object->type == SIM_STDOUT)
        {
            fflush(stdout);
            res = _dup2(std_object->pipe[WIN32_PIPE_WRITE], _fileno(stdout));
        }
        else if (std_object->type == SIM_STDERR)
        {
            fflush(stderr);
            res = _dup2(std_object->pipe[WIN32_PIPE_WRITE], _fileno(stderr));
        }
        else if (std_object->type == SIM_STDIN)
        {
            fflush(stdin);
            res = _dup2(std_object->pipe[WIN32_PIPE_READ], _fileno(stdin));
        }
        else
        {
            res = -1;
        }
        assert(res == 0);
        //init console object
        console->parent = (sim_handle_t)std_object;
        return SIM_OK;
    }
}

sim_size_t sim_console_read(sim_console_t* console, sim_byte_t* data, sim_size_t max_size)
{
    sim_size_t r_size = 0;
    win32_std_t* std_object;

    if (max_size == 0)
    {
        return 0;
    }
    if (data && console && console->parent)
    {
        std_object = (win32_std_t *)console->parent;
        r_size = _read(std_object->pipe[WIN32_PIPE_READ], data, max_size);
    }
    return r_size;
}

sim_size_t sim_console_write(sim_console_t* console, sim_byte_t* data, sim_size_t size)
{
    sim_size_t w_size = 0;
    win32_std_t* std_object;

    if (size == 0)
    {
        return 0;
    }
    if (data && console && console->parent)
    {
        std_object = (win32_std_t*)console->parent;
        w_size = _write(std_object->pipe[WIN32_PIPE_WRITE], data, size);
    }
    return w_size;
}

sim_size_t sim_stdout(sim_byte_t* data, sim_size_t size)
{
    sim_size_t w_size = 0;

    if (data && size)
    {
        w_size = _write(win32_stdout.fd, data, size);
    }
    return w_size;
}

sim_size_t sim_stderr(sim_byte_t* data, sim_size_t size)
{
    sim_size_t w_size = 0;

    if (data && size)
    {
        w_size = _write(win32_stderr.fd, data, size);
    }
    return w_size;
}

sim_size_t sim_stdint(sim_byte_t* data, sim_size_t max_size)
{
    sim_size_t r_size = 0;

    if (data && max_size)
    {
        r_size = _read(win32_stdin.fd, data, max_size);
    }
    return r_size;
}


/*
 * @param ctx           The context object to be initialized cannot be empty
 *
 * @return              Error code, SIM_OK indicates success
 */
sim_err_t sim_context_init(sim_context_t* ctx)
{
    LPCONTEXT context;

    if (ctx == NULL)
    {
        return -SIM_INVAL;
    }
    context = sim_malloc(sizeof(*context));
    memset(context, 0, sizeof(*context));
    ctx->parent = context;
    return SIM_OK;
}

/*
 * @param ctx           Initialized context object
 */
void sim_context_deinit(sim_context_t* ctx)
{
    if (ctx && ctx->parent)
    {
        sim_free(ctx->parent);
        ctx->parent = NULL;
    }
}

/*
 * @param ctx           Initialized context object
 * @param thread        Thread object initialized
 * 
 * @return              Error code, SIM_OK indicates success
 */
sim_err_t sim_context_get(sim_context_t* ctx, sim_thread_t* thread)
{
    BOOL res;

    if (ctx == NULL || ctx->parent == NULL)
    {
        return -SIM_INVAL;
    }
    if (thread == NULL || thread->parent == NULL)
    {
        return -SIM_INVAL;
    }
    res = GetThreadContext((HANDLE)thread->parent, (LPCONTEXT)ctx->parent);
    return res == TRUE ? SIM_OK : -SIM_ERROR;
}

/*
 * @param ctx           Initialized context object
 * @param thread        Thread object initialized
 * 
 * @return              Error code, SIM_OK indicates success
 */
sim_err_t sim_context_set(sim_context_t* ctx, sim_thread_t* thread)
{
    BOOL res;

    if (ctx == NULL || ctx->parent == NULL)
    {
        return -SIM_INVAL;
    }
    if (thread == NULL || thread->parent == NULL)
    {
        return -SIM_INVAL;
    }
    res = SetThreadContext((HANDLE)thread->parent, (LPCONTEXT)ctx->parent);
    return res == TRUE ? SIM_OK : -SIM_ERROR;
}


#if defined(_M_AMD64) || defined(_M_IX86)
#pragma intrinsic(_mm_pause)
#define __cpu_pause()       do { _mm_pause(); } while (0)
#elif defined(_M_IA64)
#pragma intrinsic(__yield)
#define __cpu_pause()       do { __yield(); } while (0)
#else
#define __cpu_pause()       do { YieldProcessor(); } while (0)
#endif

void sim_cpu_yield(void)
{
    __cpu_pause();
}

sim_size_t sim_cpu_count(void)
{
    static sim_size_t cpu_count = 0;

    if (cpu_count > 0)
    {
        return cpu_count;
    }
    cpu_count = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
    return cpu_count;
}

/*
 * @param critical         The critical area object to be initialized, and the input parameter is not empty
 * 
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_critical_init(sim_critical_t* critical)
{
    RTL_CRITICAL_SECTION *critical_object;

    if (critical == NULL)
    {
        return -SIM_INVAL;
    }
    critical_object = sim_malloc(sizeof(*critical_object));
    if (critical_object == NULL)
    {
        return -SIM_ERROR;
    }
    InitializeCriticalSection(critical_object);
    critical->parent = (sim_handle_t)critical_object;
    return SIM_OK;
}

/*
 * @param critical              Critical area object, input parameter is not empty
 */
void sim_critical_deinit(sim_critical_t* critical)
{
    RTL_CRITICAL_SECTION *critical_object;

    if (critical && critical->parent)
    {
        critical_object = critical->parent;
        critical->parent = NULL;
        EnterCriticalSection(critical_object);
        DeleteCriticalSection(critical_object);
        sim_free(critical_object);
    }
}

/*
 * @param critical             Critical area object, input parameter is not empty
 * 
 * @return                     Error code, SIM_OK indicates success
 */
sim_err_t sim_critical_enter(sim_critical_t* critical)
{
    if (critical == NULL || critical->parent == NULL)
    {
        return -SIM_INVAL;
    }
    EnterCriticalSection((LPCRITICAL_SECTION)critical->parent);
    return SIM_OK;
}

/*
 * @param critical             Critical area object, input parameter is not empty
 * 
 * @return                     Error code, SIM_OK indicates success
 */
sim_err_t sim_critical_leave(sim_critical_t* critical)
{
    if (critical == NULL || critical->parent == NULL)
    {
        return -SIM_INVAL;
    }
    EnterCriticalSection((LPCRITICAL_SECTION)critical->parent);
    return SIM_OK;
}


static sim_err_t sim_make_directory(sim_char_t* path, sim_size_t len)
{
    int index = len - 1;

    if (GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES)
    {
        if (strcmp("/", path) == 0 || strcmp("\\", path) == 0)
            return -1;

        while (index >= 0)
        {
            if (path[index] == '/' || path[index] == '\\')
            {
                path[index] = '\0';
                break;
            }
            index--;
        }
        if (sim_make_directory(path, index) != SIM_OK)
            return -SIM_ERROR;

        path[index] = '/';
        if (CreateDirectory(path, NULL) == FALSE)
        {
            return -SIM_ERROR;
        }
        return SIM_OK;
    }
    return SIM_OK;
}

 /*
  * @param path             Directory path
  * @param recursion        Recursive creation
  *
  * @return                 Error code, SIM_OK indicates success
  */
sim_err_t sim_directory_create(sim_char_t const* path, sim_bool_t recursion)
{
    sim_size_t path_len;
    sim_err_t err;

    if (path == NULL)
    {
        return -SIM_INVAL;
    }
    path_len = strlen(path);
    if (path_len >= SIM_PATH_MAXN)
    {
        // path is too long
        return -SIM_INVAL;
    }
    if (recursion)
    {
        sim_char_t *tmp_path = sim_malloc(SIM_PATH_MAXN);
        if (tmp_path == NULL)
        {
            return -SIM_ERROR;
        }
        memcpy(tmp_path, path, path_len + 1);
        err = sim_make_directory(tmp_path, path_len);
        sim_free(tmp_path);
        return err;
    }
    else
    {
        if (CreateDirectory(path, NULL) == FALSE)
        {
            return -SIM_ERROR;
        }
        else
        {
            return SIM_OK;
        }
    }
}

sim_err_t sim_directory_delete(sim_char_t const* path)
{
    if (path == NULL)
    {
        return -SIM_INVAL;
    }
    if (RemoveDirectory(path) == FALSE)
    {
        return -SIM_ERROR;
    }
    return SIM_OK;
}

sim_err_t sim_directory_space(sim_char_t const* path, sim_dir_space_t* space)
{
    DWORD TotalClusters, FreeClusters, SectPerClust, BytesPerSect;

    if (path == NULL || space == NULL)
    {
        return -SIM_INVAL;
    }

    if (GetDiskFreeSpace(TEXT(path), &SectPerClust, &BytesPerSect,
        &FreeClusters, &TotalClusters) == FALSE);
    {
        return -SIM_ERROR;
    }
    space->blocks = TotalClusters;
    space->bfree = FreeClusters;
    space->bsize = (DWORD64)SectPerClust * (DWORD64)BytesPerSect;
    return SIM_OK;
}

sim_err_t sim_directory_getfiles(sim_char_t const* path,
    sim_bool_t(*func)(sim_char_t const*, sim_file_stat_t*, sim_pointer_t), sim_pointer_t priv)
{
    WIN32_FIND_DATAA finddata = { 0 };
    sim_file_stat_t stat;
    HANDLE directory;
    sim_char_t* fullpath;

    if (path == NULL || func == NULL)
    {
        return -SIM_INVAL;
    }
    fullpath = sim_malloc(SIM_PATH_MAXN);
    if (fullpath == NULL)
    {
        return -SIM_ERROR;
    }
    // open directory
    directory = FindFirstFile(path, &finddata);
    if (directory == INVALID_HANDLE_VALUE)
    {
        sim_free(fullpath);
        return -SIM_ERROR;
    }
    // read directory
    do
    {
        if (strcmp(finddata.cFileName, ".") && strcmp(finddata.cFileName, ".."))
        {
            snprintf(fullpath, SIM_PATH_MAXN - 1, "%s/%s", path, finddata.cFileName);
            fullpath[SIM_PATH_MAXN - 1] = '\0';
            // get file stat
            memset(&stat, 0, sizeof(stat));
            if (sim_file_stat(fullpath, &stat) == SIM_OK)
            {
                //call walk func
                if (func(finddata.cFileName, &stat, priv) == SIM_FALSE)
                {
                    break;
                }
            }
        }
        memset(&finddata, 0, sizeof(finddata));
    } while (FindNextFile(directory, &finddata) != FALSE);
    // end
    FindClose(directory);
    sim_free(fullpath);
    return SIM_OK;
}

/*
 * @param event           Event object to be initialized, input parameter is not empty
 * @param auto_clear      Auto clear event, SIM_TRUE: Auto clear event
 * 
 * @return                Error code, SIM_OK indicates success
 */
sim_err_t sim_event_init(sim_event_t *event, sim_bool_t auto_clear)
{
    HANDLE handle;

    if (event == NULL)
    {
        return -SIM_INVAL;
    }
    auto_clear = !auto_clear;
    handle = CreateEvent(NULL, (BOOL)auto_clear, FALSE, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        return -SIM_ERROR;
    }
    event->parent = (sim_handle_t)handle;
    return SIM_OK;
}

/*
 * @param event           Event object, input parameter is not empty
 */
void sim_event_deinit(sim_event_t *event)
{
    HANDLE handle;
    BOOL result;

    if (event && (HANDLE)event->parent != INVALID_HANDLE_VALUE)
    {
        handle = (HANDLE)event->parent;
        if (handle != INVALID_HANDLE_VALUE)
        {
            event->parent = (sim_handle_t)INVALID_HANDLE_VALUE;
            result = CloseHandle(handle);
            assert(result == TRUE);
        }
    }
}

/*
 * @param event           Event object, input parameter is not empty
 * @param timeout         Timeout
 * 
 * @return                Error code, SIM_OK indicates success
 */
sim_err_t sim_event_wait(sim_event_t *event, sim_time_t timeout)
{
    DWORD result;

    if (event == NULL || (HANDLE)event->parent == INVALID_HANDLE_VALUE)
    {
        return -SIM_INVAL;
    }
    if (timeout > SIM_ULONG_MAX / 2)
    {
        timeout = (sim_time_t)INFINITE;
    }
    result = WaitForSingleObject((HANDLE)event->parent, (DWORD)timeout);
    if (result == WAIT_OBJECT_0)
    {
        return SIM_OK;
    }
    else if (result == WAIT_FAILED)
    {
        return -SIM_ERROR;
    }
    else
    {
        return -SIM_TIMEOUT;
    }
}

/*
 * @param event           Event object, input parameter is not empty
 *
 * @return                Error code, SIM_OK indicates success
 */
sim_err_t sim_event_set(sim_event_t *event)
{
    if (event == NULL || (HANDLE)event->parent == INVALID_HANDLE_VALUE)
    {
        return -SIM_INVAL;
    }
    if (SetEvent((HANDLE)event->parent) == FALSE)
    {
        return -SIM_ERROR;
    }
    else
    {
        return SIM_OK;
    }
}

/*
 * @param event           Event object, input parameter is not empty
 *
 * @return                Error code, SIM_OK indicates success
 */
sim_err_t sim_event_reset(sim_event_t *event)
{
    if (event == NULL || (HANDLE)event->parent == INVALID_HANDLE_VALUE)
    {
        return -SIM_INVAL;
    }
    if (ResetEvent((HANDLE)event->parent) == FALSE)
    {
        return -SIM_ERROR;
    }
    else
    {
        return SIM_OK;
    }
}


 /*
  * @param file             File object to be initialized, input parameter is not empty
  * @param path             File absolute path
  * @param oflags           Open flag
  * 
  * @return                 Error code, SIM_OK indicates success
  */
sim_err_t sim_file_open(sim_file_t* file, sim_char_t const *path, sim_flag_t oflags)
{
    DWORD access = GENERIC_READ;
    DWORD share = FILE_SHARE_READ | FILE_SHARE_WRITE;
    DWORD attr = FILE_ATTRIBUTE_NORMAL;
    DWORD cflag = 0;
    HANDLE handle;

    // check parameter
    if (file == NULL || path == NULL)
    {
        return -SIM_INVAL;
    }
    // init access
    access = GENERIC_READ;
    if ((oflags & SIM_FILE_O_WRONLY) == SIM_FILE_O_WRONLY)
    {
        access = GENERIC_WRITE;
    }
    if ((oflags & SIM_FILE_O_RDWR) == SIM_FILE_O_RDWR)
    {
        access = GENERIC_WRITE | GENERIC_WRITE;
    }
    // init flag
    if (oflags & SIM_FILE_O_CREAT)
    {
        if (oflags & SIM_FILE_O_TRUNC)
        {
            // always create a new empty file
            cflag |= CREATE_ALWAYS;
        }
        else if (oflags & SIM_FILE_O_APPEND)
        {
            // create or open and append file
            cflag |= OPEN_ALWAYS;
        }
        else
        {
            // create a new file only if file not exists
            cflag |= CREATE_NEW;
        }
    }
    else if (oflags & SIM_FILE_O_TRUNC)
    {
        // open and truncate an existing file
        cflag |= TRUNCATE_EXISTING;
    }
    if (cflag == 0)
    {
        cflag = OPEN_EXISTING;
    }
    // open file
    handle = CreateFile(path, access, share, NULL, cflag, attr, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        return -SIM_ERROR;
    }
    file->parent = (sim_handle_t)handle;
    return SIM_OK;
}

/*
 * @param file             File object, input parameter is not empty
 */
void sim_file_close(sim_file_t* file)
{
    HANDLE handle;

    if (file && file->parent != (sim_handle_t)INVALID_HANDLE_VALUE)
    {
        handle = (HANDLE)file->parent;
        file->parent = (sim_handle_t)INVALID_HANDLE_VALUE;
        if (CloseHandle(handle) == TRUE)
        {
            assert(0);
        }
    }
}

/*
 * @param file             File object, input parameter is not empty
 * @param path             File absolute path
 * @param oflags           Open flag
 *
 * @return                 Error code, SIM_OK indicates success
 */
sim_int64_t sim_file_read(sim_file_t* file, sim_pointer_t buff, sim_size_t size)
{
    DWORD real_size = 0;
    
    // check parameter
    if (file == NULL || file->parent == (sim_handle_t)INVALID_HANDLE_VALUE)
    {
        return -SIM_INVAL;
    }
    if (size > 0 && buff == NULL)
    {
        return -SIM_INVAL;
    }
    if (size == 0)
    {
        return 0;
    }
    if (ReadFile((HANDLE)file->parent, buff, (DWORD)size, &real_size, NULL) == FALSE)
    {
        return -SIM_ERROR;
    }
    return real_size;
}

/*
 * @param file             File object, input parameter is not empty
 * @param path             File absolute path
 * @param oflags           Open flag
 *
 * @return                 Error code, SIM_OK indicates success
 */
sim_int64_t sim_file_write(sim_file_t* file, sim_cpointer_t buff, sim_size_t size)
{
    DWORD real_size = 0;

    // check parameter
    if (file == NULL || file->parent == (sim_handle_t)INVALID_HANDLE_VALUE)
    {
        return -SIM_INVAL;
    }
    if (size > 0 && buff == NULL)
    {
        return -SIM_INVAL;
    }
    if (size == 0)
    {
        return 0;
    }
    if (WriteFile((HANDLE)file->parent, buff, (DWORD)size, &real_size, NULL) == FALSE)
    {
        return -SIM_ERROR;
    }
    return real_size;
}

/*
 * @param file             File object, input parameter is not empty
 * @param offset           Offset address
 * @param mode             Offset mode
 *
 * @return                 Error code, SIM_OK indicates success
 */
sim_int64_t sim_file_seek(sim_file_t* file, sim_off_t offset, sim_flag_t mode)
{
    LARGE_INTEGER des_pos, new_pos;
    DWORD MoveMethod = FILE_BEGIN;

    if (file == NULL || file->parent == (sim_handle_t)INVALID_HANDLE_VALUE)
    {
        return -SIM_INVAL;
    }
    memset(&des_pos, 0, sizeof(des_pos));
    memset(&new_pos, 0, sizeof(new_pos));
    des_pos.QuadPart = (LONGLONG)offset;
    if (mode == SIM_FILE_SEEK_CUR)
    {
        MoveMethod = FILE_CURRENT;
    }
    else if (mode == SIM_FILE_SEEK_END)
    {
        MoveMethod = FILE_END;
    }
    else if (mode == SIM_FILE_SEEK_SET)
    {
        MoveMethod = FILE_BEGIN;
    }
    else
    {
        assert(0);
    }
    if (SetFilePointerEx((HANDLE)file->parent, des_pos, &new_pos, MoveMethod) == FALSE)
    {
        return -SIM_ERROR;
    }
    return new_pos.QuadPart;
}

/*
 * @param file             File object, input parameter is not empty
 *
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_file_flush(sim_file_t* file)
{
    if (file == NULL || file->parent == (sim_handle_t)INVALID_HANDLE_VALUE)
    {
        return -SIM_INVAL;
    }
    if (FlushFileBuffers((HANDLE)file->parent) == FALSE)
    {
        return -SIM_ERROR;
    }
    return SIM_OK;
}

/*
 * @param new_path         Target absolute paths
 * @param new_path         Current absolute path
 *
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_file_rename(sim_char_t const* new_path, sim_char_t const* old_path)
{
    if (new_path == NULL || old_path == NULL)
    {
        return -SIM_INVAL;
    }
    if (MoveFileEx(old_path, new_path, MOVEFILE_REPLACE_EXISTING) == FALSE)
    {
        return -SIM_ERROR;
    }
    return SIM_OK;
}

/*
 * @param path             File absolute path
 *
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_file_remove(sim_char_t const* path)
{
    DWORD attrs;

    if (path == NULL)
    {
        return -SIM_INVAL;
    }
    attrs = GetFileAttributes(path);
    if (attrs == INVALID_FILE_ATTRIBUTES)
    {
        return -SIM_ERROR;
    }
    if (attrs & FILE_ATTRIBUTE_READONLY)
    {
        if (SetFileAttributes(path, attrs & ~FILE_ATTRIBUTE_READONLY) == FALSE)
        {
            return -SIM_ERROR;
        }
    }
    if (DeleteFile(path) == FALSE)
    {
        return -SIM_ERROR;
    }
    return SIM_OK;
}

/*
 * @param path             File absolute path
 *
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_file_stat(sim_char_t const* path, sim_file_stat_t* stat)
{
    WIN32_FILE_ATTRIBUTE_DATA attribute = { 0 };
    LARGE_INTEGER time_tmp;

    if (path == NULL || stat == NULL)
    {
        return -SIM_INVAL;
    }
    if (GetFileAttributesEx(path, GetFileExInfoStandard, &attribute) == FALSE)
    {
        return -SIM_ERROR;
    }
    //file type
    if (attribute.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        stat->mode = SIM_FILE_MODE_IFDIR;
    }
    else if (attribute.dwFileAttributes != 0xFFFFFFFF)
    {
        stat->mode = SIM_FILE_MODE_IFREG;
    }
    //file size
    stat->size = ((sim_uint64_t)attribute.nFileSizeHigh << 32) | (sim_uint64_t)attribute.nFileSizeLow;
    //last access time
    time_tmp.LowPart = attribute.ftLastAccessTime.dwLowDateTime;
    time_tmp.HighPart = attribute.ftLastAccessTime.dwHighDateTime;
    /* removes the diff between 1970 and 1601. */
    time_tmp.QuadPart -= 11644473600000 * 10000;
    /* converts back from 100-nanoseconds to seconds. */
    time_tmp.QuadPart /= 10UL * 1000 * 1000;
    stat->atime = time_tmp.QuadPart;
    //last modify time
    time_tmp.LowPart = attribute.ftLastWriteTime.dwLowDateTime;
    time_tmp.HighPart = attribute.ftLastWriteTime.dwHighDateTime;
    /* removes the diff between 1970 and 1601. */
    time_tmp.QuadPart -= 11644473600000 * 10000;
    /* converts back from 100-nanoseconds to seconds. */
    time_tmp.QuadPart /= 10UL * 1000 * 1000;
    stat->mtime = time_tmp.QuadPart;
    //last create time
    time_tmp.LowPart = attribute.ftCreationTime.dwLowDateTime;
    time_tmp.HighPart = attribute.ftCreationTime.dwHighDateTime;
    /* removes the diff between 1970 and 1601. */
    time_tmp.QuadPart -= 11644473600000 * 10000;
    /* converts back from 100-nanoseconds to seconds. */
    time_tmp.QuadPart /= 10UL * 1000 * 1000;
    stat->ctime = time_tmp.QuadPart;
    return SIM_OK;
}


/*
 * @param mutex            Mutex object to be initialized, input parameter not null
 * 
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_mutex_init(sim_mutex_t* mutex)
{
    HANDLE handle;

    if (mutex == NULL)
    {
        return -SIM_INVAL;
    }
    handle = CreateMutex(NULL, FALSE, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        return -SIM_ERROR;
    }
    mutex->parent = (sim_handle_t)handle;
    return SIM_OK;
}

/*
 * @param mutex              Mutex object, input parameter not null
 */
void sim_mutex_deinit(sim_mutex_t* mutex)
{
    HANDLE handle;

    if (mutex)
    {
        handle = (HANDLE)mutex->parent;
        if (handle != INVALID_HANDLE_VALUE)
        {
            mutex->parent = (sim_handle_t)INVALID_HANDLE_VALUE;
            CloseHandle(handle);
        }
    }
}

/*
 * @param mutex            Mutex object, input parameter not null
 * @param timeout          Wait timeout
 * 
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_mutex_get(sim_mutex_t* mutex, sim_time_t timeout)
{
    DWORD result;

    if (mutex == NULL || (HANDLE)mutex->parent == INVALID_HANDLE_VALUE)
    {
        return -SIM_INVAL;
    }
    if (timeout > SIM_ULONG_MAX / 2)
    {
        timeout = (sim_time_t)INFINITE;
    }
    result = WaitForSingleObject((HANDLE)mutex->parent, (DWORD)timeout);
    if (result == WAIT_OBJECT_0)
    {
        return SIM_OK;
    }
    else if (result == WAIT_FAILED)
    {
        return -SIM_ERROR;
    }
    else
    {
        return -SIM_TIMEOUT;
    }
}

/*
 * @param mutex            Mutex object, input parameter not null
 * 
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_mutex_put(sim_mutex_t* mutex)
{
    if (mutex == NULL || (HANDLE)mutex->parent == INVALID_HANDLE_VALUE)
    {
        return -SIM_INVAL;
    }
    if (ReleaseMutex((HANDLE)mutex->parent) == FALSE)
    {
        return -SIM_ERROR;
    }
    else
    {
        return SIM_OK;
    }
}


/*
 * @param sem              Semaphore object to be initialized, input parameter not null
 * @param value            Initial value of semaphore
 * 
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_sem_init(sim_sem_t* sem, sim_size_t value)
{
    LONG init_count = (LONG)value;
    HANDLE handle;

    if (sem == NULL || init_count < 0)
    {
        return -SIM_INVAL;
    }
    /* create semaphore */
    handle = CreateSemaphore(NULL, init_count, SIM_LONG_MAX, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        return -SIM_ERROR;
    }
    sem->parent = (sim_handle_t)handle;
    return SIM_OK;
}

/*
 * @param sem              Semaphore object, input parameter not null
 */
void sim_sem_deinit(sim_sem_t* sem)
{
    HANDLE handle;

    if (sem)
    {
        handle = (HANDLE)sem->parent;
        if (handle != INVALID_HANDLE_VALUE)
        {
            sem->parent = (sim_handle_t)INVALID_HANDLE_VALUE;
            CloseHandle(handle);
        }
    }
}

/*
 * @param sem              Semaphore object, input parameter not null
 * @param timeout          Wait timeout
 * 
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_sem_take(sim_sem_t* sem, sim_time_t timeout)
{
    DWORD result;

    if (sem == NULL || (HANDLE)sem->parent == INVALID_HANDLE_VALUE)
    {
        return -SIM_INVAL;
    }
    if (timeout > SIM_ULONG_MAX / 2)
    {
        timeout = (sim_time_t)INFINITE;
    }
    result = WaitForSingleObject((HANDLE)sem->parent, (DWORD)timeout);
    if (result == WAIT_OBJECT_0)
    {
        return SIM_OK;
    }
    else if (result == WAIT_FAILED)
    {
        return -SIM_ERROR;
    }
    else
    {
        return -SIM_TIMEOUT;
    }
}

/*
 * @param sem              Semaphore object, input parameter not null
 * 
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_sem_release(sim_sem_t* sem)
{
    if (sem == NULL || (HANDLE)sem->parent == INVALID_HANDLE_VALUE)
    {
        return -SIM_INVAL;
    }
    if (ReleaseSemaphore((HANDLE)sem->parent, 1, NULL) == FALSE)
    {
        return -SIM_ERROR;
    }
    else
    {
        return SIM_OK;
    }
}


/*
 * @param thread        The thread object to be initialized cannot be empty
 * @param func          Thread entry function, cannot be empty
 * @param priv          Thread entry parameters
 * @param stack         The thread stack size. If it is 0, the default size is used
 *
 * @return              Error code, SIM_OK indicates success
 */
sim_err_t sim_thread_init(sim_thread_t *thread, int (*func)(sim_pointer_t),
    sim_pointer_t priv, sim_size_t stack_size)
{
    HANDLE handle;

    if (thread == NULL || func == NULL)
    {
        return -SIM_INVAL;
    }
    /* create thread and run thread */
    handle = CreateThread(NULL, (DWORD)stack_size, (LPTHREAD_START_ROUTINE)func, (LPVOID)priv, 0, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        return -SIM_ERROR;
    }
    thread->parent = (sim_handle_t)handle;
    return SIM_OK;
}

/*
 * @param thread        Thread object after initialization
 */
void sim_thread_deinit(sim_thread_t* thread)
{
    if (thread && thread->parent != INVALID_HANDLE_VALUE)
    {
        CloseHandle((HANDLE)thread->parent);
        thread->parent = NULL;
    }
}

/*
 * @param thread        Thread object after initialization
 *
 * @return              Error code, SIM_OK indicates success
 */
sim_err_t sim_thread_suspend(sim_thread_t* thread)
{
    sim_long_t res;
    HANDLE Handle;

    if (thread == NULL || thread->parent == NULL)
    {
        Handle = GetCurrentThread();
    }
    else
    {
        Handle = (HANDLE)thread->parent;
    }
    res = (sim_long_t)SuspendThread((HANDLE)thread->parent);
    return res < 0 ? -SIM_ERROR : -SIM_OK;
}

/*
 * @param thread        Thread object after initialization
 *
 * @return              Error code, SIM_OK indicates success
 */
sim_err_t sim_thread_resume(sim_thread_t* thread)
{
    sim_long_t res;
    HANDLE Handle;

    if (thread == NULL || thread->parent == NULL)
    {
        Handle = GetCurrentThread();
    }
    else
    {
        Handle = (HANDLE)thread->parent;
    }
    res = (sim_long_t)ResumeThread(Handle);
    return res < 0 ? -SIM_ERROR : -SIM_OK;
}

/*
 * @param thread        Thread object or empty object after initialization
 *
 * @return              Indicates the unique ID value of the thread
 */
sim_long_t sim_thread_identifier(sim_thread_t*thread)
{
    sim_long_t id;

    if (thread == NULL || thread->parent == INVALID_HANDLE_VALUE)
    {
        id = (sim_long_t)GetCurrentThreadId();
    }
    else
    {
        id = (sim_long_t)GetThreadId((HANDLE)thread->parent);
    }
    assert((id & 0xFE000000) == 0);
    return id;
}

/*
 * @param thread        Thread object after initialization
 * @affinity_mask       The CPU mask that runs on the specified core
 * 
 * @return              Error code, SIM_OK indicates success
 */
sim_err_t sim_thread_setaffinity(sim_thread_t* thread, sim_ulong_t affinity_mask)
{
    sim_ulong_t res;
    HANDLE Handle;

    if (affinity_mask == 0)
    {
        return -SIM_INVAL;
    }
    else if (thread == NULL || thread->parent == NULL)
    {
        Handle = GetCurrentThread();
    }
    else
    {
        Handle = (HANDLE)thread->parent;
    }
    res = (sim_ulong_t)SetThreadAffinityMask(Handle, affinity_mask);
    return res == 0 ? -SIM_ERROR : -SIM_OK;
}

/*
 * @param thread        Thread object after initialization
 * 
 * @return              Returns which CPU cores the thread supports to run on
 */
sim_ulong_t sim_thread_getaffinity(sim_thread_t* thread)
{
    sim_ulong_t res;
    DWORD_PTR backup;
    HANDLE Handle;

    if (thread == NULL || thread->parent == NULL)
    {
        Handle = GetCurrentThread();
    }
    else
    {
        Handle = (HANDLE)thread->parent;
    }
    backup = SetThreadAffinityMask(Handle, 0x1);
    if (backup == 0)
    {
        return 0;
    }
    res = SetThreadAffinityMask(Handle, backup);
    return res;
}

/*
 * @param thread        Thread object after initialization
 * @name                Thread Name
 */
sim_err_t sim_thread_setname(sim_thread_t* thread, sim_char_t const *name)
{
    const DWORD MS_VC_EXCEPTION = 0x406D1388;
    HANDLE Handle;
#pragma pack(push,8)
    struct tagTHREADNAME_INFO
    {
        DWORD dwType;       // Must be 0x1000.
        LPCSTR szName;      // Pointer to name (in user addr space).
        DWORD dwThreadID;   // Thread ID (-1=caller thread).
        DWORD dwFlags;      // Reserved for future use, must be zero.
    } info;
#pragma pack(pop)

    if (name == NULL)
    {
        return -SIM_INVAL;
    }
    if (thread == NULL || thread->parent == NULL)
    {
        Handle = GetCurrentThread();
    }
    else
    {
        Handle = (HANDLE)thread->parent;
    }
    info.dwType = 0x1000;
    info.szName = name;
    info.dwThreadID = GetThreadId(Handle);
    info.dwFlags = 0;
    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
    return SIM_OK;
}

/*
 * @param us            Thread hang time in microseconds, non negative input parameter 0
 */
void sim_usleep(sim_time_t us)
{
    HANDLE timer;
    LARGE_INTEGER due;

    timer = CreateWaitableTimer(NULL, FALSE, NULL);
    if (timer == NULL)
    {
        return;
    }
    due.QuadPart = -((long long)us * 1000 / 100);
    if (SetWaitableTimer(timer, &due, FALSE, NULL, NULL, 0) == TRUE)
    {
        if (WaitForSingleObject(timer, INFINITE) != WAIT_OBJECT_0)
        {
            assert(0);
        }
    }
    CloseHandle(timer);
}

/*
 * @param ms            The thread is suspended for milliseconds, and the input parameter is non negative 0
 */
void sim_msleep(sim_time_t ms)
{
    Sleep((DWORD)ms);
}

/*
 * @param s            Thread hang seconds, non negative input parameter 0
 */
void sim_sleep(sim_time_t s)
{
    sim_msleep(s * 1000);
}

/*
 * @return             Returns the millisecond time the program ran
 */
sim_time_t sim_mtime(void)
{
    return (sim_time_t)GetTickCount();
}

/*
 * @return             Returns the microsecond time the program was running
 */
sim_time_t sim_utime(void)
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;

    memset(&frequency, 0, sizeof(frequency));
    memset(&counter, 0, sizeof(counter));
    if (QueryPerformanceFrequency(&frequency) == FALSE)
    {
        return 0;
    }
    if (frequency.QuadPart == 0)
    {
        return 0;
    }
    if (QueryPerformanceCounter(&counter) == FALSE)
    {
        return 0;
    }
    if (counter.QuadPart == 0)
    {
        return 0;
    }
    return (sim_time_t)((counter.QuadPart * 1000000) / frequency.QuadPart);
}


struct win32_timer_object
{
    MMRESULT id;
    TIMECAPS cap;
    UINT accuracy;
    void(*func)(sim_pointer_t);
    sim_pointer_t parameter;
    sim_time_t time;
    sim_bool_t periodic;
};

static CALLBACK timer_callback(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    struct win32_timer_object* timer = (struct win32_timer_object*)dwUser;
    /* Execute the user's timeout callback function */
    if (timer->func)
    {
        timer->func(timer->parameter);
    }
}

/*
 * @param timer            Timer object to be initialized, input parameter is not empty
 * @param time             Timeout
 * @param periodic         Cycle flag, which is triggered when it is true
 * @param func             Timeout callback function
 * @param priv             Callback function parameters
 *
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_timer_init(sim_timer_t* timer, sim_time_t time, sim_bool_t periodic,
    void(*func)(sim_pointer_t), sim_pointer_t priv)
{
    struct win32_timer_object* object;
    MMRESULT res;

    if (timer == NULL || func == NULL || time == 0)
    {
        return -SIM_INVAL;
    }
    object = sim_malloc(sizeof(*object));
    assert(object != NULL);
    memset(object, 0, sizeof(*object));
    // config timer
    res = timeGetDevCaps(&object->cap, sizeof(object->cap));
    assert(res == TIMERR_NOERROR);
    object->accuracy = min(max(object->cap.wPeriodMin, 1), object->cap.wPeriodMax);
    res = timeBeginPeriod(object->accuracy);
    assert(res == TIMERR_NOERROR);
    object->func = func;
    object->parameter = priv;
    object->time = time;
    object->periodic = periodic;
    // init timer
    timer->parent = object;
    return SIM_OK;
}

void sim_timer_deinit(sim_timer_t* timer)
{
    struct win32_timer_object* object;

    if (timer && timer->parent)
    {
        object = (struct win32_timer_object*)timer->parent;
        if (object->id)
        {
            timeKillEvent(object->id);
            object->id = 0;
        }
        // end timer
        if (object->accuracy != 0)
        {
            timeEndPeriod(object->accuracy);
            object->accuracy = 0;
        }
        // free
        memset(object, 0, sizeof(object));
        sim_free(object);
        timer->parent = NULL;
    }
}

/*
 * @param timer            Timer object, input parameter not null
 *
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_timer_start(sim_timer_t* timer)
{
    struct win32_timer_object* object;
    UINT flag = TIME_CALLBACK_FUNCTION;

    if (timer == NULL || timer->parent == NULL)
    {
        return -SIM_INVAL;
    }
    object = (struct win32_timer_object*)timer->parent;
    if (object->id != 0)
    {
        return -SIM_BUSY;
    }
    flag |= object->periodic ? TIME_PERIODIC : TIME_ONESHOT;
    /* start timer */
    object->id = timeSetEvent((UINT)object->time, object->cap.wPeriodMin,
        timer_callback, (DWORD_PTR)object, flag);
    if (object->id == 0)
    {
        return -SIM_ERROR;
    }
    return SIM_OK;
}

/*
 * @param timer            Timer object, input parameter not null
 *
 * @return                 Error code, SIM_OK indicates success
 */
sim_bool_t sim_timer_isactive(sim_timer_t* timer)
{
    struct win32_timer_object* object;

    if (timer == NULL || timer->parent == NULL)
    {
        return SIM_FALSE;
    }
    object = (struct win32_timer_object*)timer->parent;
    return object->id != 0;
}

/*
 * @param timer            Timer object, input parameter not null
 *
 * @return                 Error code, SIM_OK indicates success
 */
sim_err_t sim_timer_stop(sim_timer_t* timer)
{
    struct win32_timer_object* object;
    MMRESULT id;

    if (timer == NULL || timer->parent == NULL)
    {
        return -SIM_INVAL;
    }
    object = (struct win32_timer_object*)timer->parent;
    id = object->id;
    object->id = 0;
    if (id)
    {
        timeKillEvent(id);
    }
    return SIM_OK;
}

#ifdef _MSC_VER
#ifndef _CRT_TERMINATE_DEFINED
#define _CRT_TERMINATE_DEFINED
_CRTIMP __declspec(noreturn) void __cdecl exit(__in int _Code);
_CRTIMP __declspec(noreturn) void __cdecl _exit(__in int _Code);
_CRTIMP void __cdecl abort(void);
#endif
#endif

void sim_shutdown(void)
{
    exit(0);
}

extern int rtthread_startup(void);
extern void rt_low_level_init(int argc, wchar_t* argv[]);

int wmain(int argc, wchar_t* argv[])
{
    HANDLE Handle;
    BOOL res;

    Handle = GetCurrentThread();
    assert(Handle != NULL);
    res = SetThreadPriority(Handle, THREAD_PRIORITY_HIGHEST);
    assert(res == TRUE);
    //init low level function
    sim_console_lowlevel_init();
    // param
    rt_low_level_init(argc, argv);
    /* startup RT-Thread RTOS */
    rtthread_startup();
    //deinit low level function
    sim_console_lowlevel_deinit();
    return 0;
}
#pragma comment(linker, "/subsystem:console /entry:wmainCRTStartup")
