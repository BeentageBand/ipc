#define COBJECT_IMPLEMENTATION
#define Dbg_FID DBG_FID_DEF(IPC_FID, 5)

#include "dbg_log.h"
#include "ipc_helper.h"
#include "thread.h"
#include "sem.h"


static void thread_delete(struct Object * const obj);
static void thread_run(union Thread * const this);
static void thread_wait(union Thread * const this, IPC_Clock_T const wait_ms);
static void thread_runnable(union Thread * const this);

struct Thread_Class Thread_Class =
    {
   {thread_delete, NULL},
   thread_run,
   thread_wait,
   thread_runnable
    };

static union Thread Thread = {NULL};

void thread_delete(struct Object * const obj)
{
  Thread_T * const this = (Thread_T *)Object_Cast(&Thread_Class.Class, obj);
  Isnt_Nullptr(this,);

  IPC_Helper_T * ipc_helper = IPC_get_instance();
  if(ipc_helper->vtbl->join_thread(ipc_helper, this))
    {
      ipc_helper->vtbl->free_thread(ipc_helper, this);
    }
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
  t_sem->vtbl->wait(t_sem, wait_ms);
}

void Populate_Thread(union Thread * const this, IPC_TID_T const tid)
{
  if(NULL == Thread.vtbl)
    {
      Thread.vtbl = &Thread_Class;
      Thread.tid = IPC_MAX_TID;
      Thread.attr = NULL;
    }

  memcpy(this, &Thread, sizeof(Thread));
  this->tid = tid;

  IPC_Helper_T * ipc_helper = IPC_get_instance();

  if(tid <IPC_MAX_TID &&
      ipc_helper->vtbl->alloc_thread(ipc_helper, this))
    {
      Populate_Semaphore(&this->sem_ready, 1);
      this->sem_ready.vtbl->wait(&this->sem_ready, 0); //Thread is not ready
    }
}
