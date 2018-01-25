#define COBJECT_IMPLEMENTATION

#include <pthread.h>
#include "ipc_helper.h"
#include "thread.h"
#include "semaphore.h"

#define THREAD_INIT(tid, desc) -1,

static void thread_delete(struct Object * const obj);
static void thread_run(union Thread * const this);
static void thread_wait(union Thread * const this, IPC_Clock_T const wait_ms);
static void thread_runnable(union Thread * const this);
static void * thread_routine(void * arg);

union Thread_Class Thread_Class =
{
        {thread_delete, NULL},
        thread_run,
        thread_wait,
        thread_runnable
};

static union Thread Thread = {NULL};

static pthread_t Thread_Pthread_Pool[] =
{
        IPC_THREAD_LIST(THREAD_INIT)
};

static pthread_attr_t Thread_Pthread_Attr;

void thread_delete(struct Object * const obj)
{
    Thread_T * const this = Object_Cast(&Thread_Class, obj);
    Isnt_Nullptr(this,);

    IPC_Helper_T * ipc_helper = IPC_get_instance();
    ipc_helper->vtbl->join_thread(ipc_helper, this);
    IPC_Unregister_Thread(this);
    ipc_helper->vtbl->free_thread(ipc_helper, this);
}

void thread_run(union Thread * const this)
{
    IPC_Helper_T * ipc_helper = IPC_get_instance();

    if(ipc_helper->vtbl->run_thread(ipc_helper, this))
    {
        union Semaphore * t_sem = &this->sem_ready;
        t_sem->vtbl->post(t_sem);
    }
}

void thread_runnable(union Thread * const this){}//Implements!

void thread_wait(union Thread * const this, uint32_t const wait_ms)
{
    union Semaphore * t_sem = &this->sem_ready;

    if(t_sem->vtbl->wait(t_sem, wait_ms))
    {

    }
}

void Populate_Thread(union Thread * const this, IPC_TID_T const tid)
{
    if(IPC_MAX_TID <= tid) return ;

    if(NULL == Thread.vtbl)
    {
        pthread_attr_init(&Thread_Pthread_Attr);
        Thread.vtbl = &Thread_Class;
        Thread.tid = 0;
    }


    if(-1 == Thread_Pthread_Pool[this->tid])
    {
        if(IPC_Helper_find_thread(tid))
        {
            IPC_Register_Thread(this);

            memcpy(this, &Thread, sizeof(Thread));
            Populate_Semaphore(&this->sem_ready, 1);
        }
    }
}
