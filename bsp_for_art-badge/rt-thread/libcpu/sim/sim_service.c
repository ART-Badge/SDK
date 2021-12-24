/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-17     tyx          first implementation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sim.h>

#define SIM_INTERRUPT_SERVER_STOP    0
#define SIM_INTERRUPT_SERVER_RUNING  1
#define SIM_INTERRUPT_SERVER_CLOSE   2

typedef union __sim_interrupt_owner_t
{
    /* flags bit description
     * 31   7       1         0
     * | id | count | disable |
     */
    sim_atomic_t raw;
    struct sim_interrupt_state_bit
    {
        volatile unsigned long disable : 1;   /* global interrupt disable flags. 0:en 1:dis */
        volatile unsigned long count : 6;     /* disable interrupt runner id */
        volatile unsigned long id : 25;       /* runner id of the lock interrupt */
    }bit;
}sim_interrupt_owner_t;

struct sim_interrupt_lock
{
    sim_interrupt_owner_t owner;
    sim_event_t wait;
};

struct sim_interrupt_object
{
    sim_sem_t trigger;
    sim_uint8_t pendtab[SIM_INTERRUPT_MAX];
    sim_event_t schedule;
    sim_sem_t systick;
    sim_atomic_t state;
    unsigned long id;
    struct sim_interrupt_lock lock;
};

static struct sim_interrupt_object *sim_interrupt_object_get(void)
{
    static struct sim_interrupt_object interrupt_object;

    if (interrupt_object.state == SIM_INTERRUPT_SERVER_STOP)
    {
        struct sim_interrupt_object* object = &interrupt_object;
        sim_err_t err;
        // init sem
        err = sim_sem_init(&object->trigger, 0);
        assert(err == SIM_OK);
        // init lock wait event
        err = sim_event_init(&object->lock.wait, SIM_FALSE);
        assert(err == SIM_OK);
        //init schedule event
        err = sim_event_init(&object->schedule, SIM_FALSE);
        assert(err == SIM_OK);
        //init systick event
        err = sim_sem_init(&object->systick, 0);
        assert(err == SIM_OK);
        object->state = SIM_INTERRUPT_SERVER_RUNING;
    }
    return &interrupt_object;
}

static void sim_interrupt_doing(void)
{
    struct sim_interrupt_object* object = sim_interrupt_object_get();
    sim_uint8_t pendtab[SIM_INTERRUPT_MAX];
    long index;

    //copy interrupt pending tab
    memcpy(pendtab, object->pendtab, sizeof(pendtab));
    //reset interrupt pending tab
    for (index = 0; index < SIM_INTERRUPT_MAX; index++)
    {
        if (pendtab[index] == SIM_TRUE)
        {
            // clean pending
            object->pendtab[index] = SIM_FALSE;
        }
    }
    // get interrupt number
    for (index = 0; index < SIM_INTERRUPT_MAX; index++)
    {
        if (pendtab[index] == SIM_TRUE)
        {
            // call interrupt handle function
            sim_interrupt_dispatch(index);
        }
    }
}

void sim_interrupt_server_run(void)
{
    struct sim_interrupt_object *object = sim_interrupt_object_get();
    sim_handle_t interrupt_context[64];

    //reset interrupt
    object->id = sim_thread_identifier(NULL);
    object->lock.owner.raw = 0;
    sim_event_set(&object->lock.wait);
    /* interrupt server start */
    while (object->state == SIM_INTERRUPT_SERVER_RUNING)
    {
        // wait trigger
        sim_sem_take(&object->trigger, SIM_WAITING_FOREVER);
        // disable interrupt
        sim_interrupt_context_load(interrupt_context);
        // do irq
        sim_interrupt_doing();
        // check systick interrupt
        if (sim_sem_take(&object->systick, SIM_WAITING_NO) == SIM_OK)
        {
            sim_interrupt_systick();
        }
        // check schedule
        if (sim_event_wait(&object->schedule, SIM_WAITING_NO) == -SIM_TIMEOUT)
        {
            sim_interrupt_schedule_handler();
            //unlock schedule event
            sim_event_set(&object->schedule);
        }
        // enable interrupt
        sim_interrupt_context_store(interrupt_context);
    }
    //resource release
    sim_sem_deinit(&object->trigger);
    sim_event_deinit(&object->lock.wait);
    sim_event_deinit(&object->schedule);
    memset(object, 0, sizeof(object));
}

void sim_interrupt_server_stop(void)
{
    struct sim_interrupt_object* object = sim_interrupt_object_get();
    sim_long_t count = 0, max = 100;

    if (object->state == SIM_INTERRUPT_SERVER_STOP)
    {
        return;
    }
    if (object->state == SIM_INTERRUPT_SERVER_RUNING)
    {
        object->state = SIM_INTERRUPT_SERVER_CLOSE;
        sim_sem_release(&object->trigger);
    }
    while (count < max && object->state == SIM_INTERRUPT_SERVER_CLOSE)
    {
        sim_msleep(10);
        count++;
    }
}

void sim_interrupt_trigger(long index)
{
    struct sim_interrupt_object *object = sim_interrupt_object_get();

    if (index < 0 || index > SIM_INTERRUPT_MAX)
        return;
    //trigger
    object->pendtab[index] = SIM_TRUE;
    //wakeup server thread
    sim_sem_release(&object->trigger);
}

void sim_interrupt_schedule_trigger(void)
{
    struct sim_interrupt_object* object = sim_interrupt_object_get();
    // lock schedule event
    sim_event_reset(&object->schedule);
    //wakeup server thread
    sim_sem_release(&object->trigger);
}

void sim_interrupt_systick_trigger(void)
{
    struct sim_interrupt_object* object = sim_interrupt_object_get();
    // set systick interrupt
    sim_sem_release(&object->systick);
    //wakeup server thread
    sim_sem_release(&object->trigger);
}

long sim_interrupt_disable(void)
{
    struct sim_interrupt_object *object = sim_interrupt_object_get();
    sim_interrupt_owner_t owner;
    sim_atomic_t backup;
    sim_long_t identifier = sim_thread_identifier(NULL);
    long res;

    do
    {
        // wait interrupt enable
        do
        {
            // get current interrupt owner
            owner.raw = backup = object->lock.owner.raw;
            // wait interrupt enable
            res = owner.bit.disable && owner.bit.id != identifier;
            if (res)
            {
                assert(owner.bit.id != 0);
                sim_cpu_yield();
                sim_event_wait(&object->lock.wait, SIM_WAITING_FOREVER);
            }
        } while (res);
        // disable interrupt
        owner.bit.disable = 1;
        owner.bit.id = identifier;
        owner.bit.count += 1;
        res = sim_atomic_compare_and_swap(&object->lock.owner.raw, backup, owner.raw);
    } while (res != backup);
    // clear event
    sim_event_reset(&object->lock.wait);
    // return level
    return backup;
}

void sim_interrupt_enable(long level)
{
    struct sim_interrupt_object* object = sim_interrupt_object_get();
    sim_interrupt_owner_t owner;
    sim_atomic_t backup;
    sim_long_t identifier = sim_thread_identifier(NULL);
    long res;

    owner.raw = level;
    if (owner.bit.disable)
        return;
    //check interrupt state
    owner.raw = backup = object->lock.owner.raw;
    assert(owner.bit.disable);
    assert(owner.bit.id == identifier);
    //enable interrupt
    owner.bit.disable = 0;
    owner.bit.id = 0;
    owner.bit.count += 1;
    //write back
    res = sim_atomic_compare_and_swap(&object->lock.owner.raw, backup, owner.raw);
    assert(res == backup);
    //set event. wakeup wait thread
    sim_event_set(&object->lock.wait);
    if (identifier != object->id)
    {
        //!!!!!! wait for interrupt schedule to complete !!!!!!
        sim_event_wait(&object->schedule, SIM_WAITING_FOREVER);
    }
}

 /*
  * @param size          Memory block size
  *
  * @return              Memory block pointer
  */
sim_pointer_t sim_malloc(sim_size_t size)
{
    sim_pointer_t ptr;

    ptr = malloc(size);
    if (ptr)
    {
        memset(ptr, 0, size);
    }
    return ptr;
}

/*
 * @param ptr           Memory block pointer
 */
void sim_free(sim_pointer_t ptr)
{
    free(ptr);
}


#define UNLOCK    (-1)

 /*
  * @param lock          The atomic lock object to be initialized cannot be empty
  */
void sim_spinlock_init(sim_spinlock_t* lock)
{
    lock->lock = UNLOCK;
}

/*
 * @param lock          Atomic lock object
 */
void sim_spinlock_lock(sim_spinlock_t* lock)
{
    sim_long_t owner = sim_thread_identifier(SIM_NULL);
    sim_bool_t res;

    do
    {
        res = lock->lock == owner || sim_atomic_compare_and_swap(&lock->lock, UNLOCK, owner) == UNLOCK;
        if (!res)
        {
            sim_cpu_yield();
        }
    } while (!res);
}

/*
 * @param lock          Atomic lock object
 */
void sim_spinlock_unlock(sim_spinlock_t* lock)
{
    sim_long_t owner = sim_thread_identifier(SIM_NULL);
    sim_bool_t res;

    do
    {
        res = lock->lock == owner && sim_atomic_compare_and_swap(&lock->lock, owner, UNLOCK) == owner;
        if (!res)
        {
            sim_cpu_yield();
        }
    } while (!res);
}

/*
 * @param lock          Atomic lock object
 */
sim_bool_t sim_spinlock_islock(sim_spinlock_t* lock)
{
    return !(lock->lock == UNLOCK);
}

/*
 * @param lock          Atomic lock object
 */
sim_bool_t sim_spinlock_trylock(sim_spinlock_t* lock)
{
    sim_long_t owner = sim_thread_identifier(SIM_NULL);
    sim_bool_t res;

    res = lock->lock == owner || sim_atomic_compare_and_swap(&lock->lock, UNLOCK, owner) == UNLOCK;
    return res;
}


struct systick_des
{
    sim_thread_t thread;
    sim_time_t tick_per;
    volatile sim_flag_t state;
};

#define SIM_SYSTICK_STOP    0
#define SIM_SYSTICK_INIT    1
#define SIM_SYSTICK_RUNING  2
#define SIM_SYSTICK_CLOSE   3

static void timeout_callback(sim_pointer_t parameter)
{
    sim_sem_release((sim_sem_t*)parameter);
}

static int systick_thread(sim_pointer_t parameter)
{
    struct systick_des *systick = (struct systick_des *)parameter;
    sim_sem_t sem;
    sim_timer_t timer;
    sim_err_t err;

    // init
    err = sim_sem_init(&sem, 0);
    assert(err == SIM_OK);
    assert(systick->tick_per <= 1000);
    err = sim_timer_init(&timer, 1000 / systick->tick_per, SIM_TRUE, timeout_callback, &sem);
    assert(err == SIM_OK);
    // start
    err = sim_timer_start(&timer);
    assert(err == SIM_OK);
    systick->state = SIM_SYSTICK_RUNING;
    while (systick->state == SIM_SYSTICK_RUNING)
    {
        err = sim_sem_take(&sem, SIM_WAITING_FOREVER);
        if (systick->state != SIM_SYSTICK_RUNING)
        {
            continue;
        }
        assert(err == SIM_OK);
        sim_interrupt_systick_trigger();
    }
    // stop
    sim_timer_stop(&timer);
    sim_timer_deinit(&timer);
    sim_sem_deinit(&sem);
    systick->state = SIM_SYSTICK_STOP;
    //thread exit
    return 0;
}

static struct systick_des* systick_object_get(void)
{
    static struct systick_des _systick;

    return &_systick;
}

void sim_systick_init(sim_time_t tick_per)
{
    sim_err_t err;
    struct systick_des* systick = systick_object_get();

    if (systick->state != SIM_SYSTICK_STOP)
    {
        return;
    }
    systick->state = SIM_SYSTICK_INIT;
    systick->tick_per = tick_per;
    err = sim_thread_init(&systick->thread, systick_thread, systick, 0);
    assert(err == SIM_OK);
}

void sim_systick_deinit(void)
{
    struct systick_des* systick = systick_object_get();
    sim_long_t count = 0, max = 100;

    while (count < max && systick->state == SIM_SYSTICK_INIT)
    {
        sim_msleep(10);
        count++;
    }
    if (count < max && systick->state == SIM_SYSTICK_RUNING)
    {
        systick->state = SIM_SYSTICK_CLOSE;
    }
    //while (count < max && systick->state == SIM_SYSTICK_CLOSE)
    //{
    //    sim_msleep(systick->tick_per / 10);
    //    count++;
    //}
}
