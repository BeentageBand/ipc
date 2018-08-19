#define COBJECT_IMPLEMENTATION
#define Dbg_FID DBG_FID_DEF(IPC_FID, 5)

#include "dbg_log.h"
#include "ipc_helper.h"
#include "thread.h"
#include "sem.h"

static void thread_cbk_delete(struct Object * const obj);

static void thread_delete(struct Object * const obj);
static void thread_run(union Thread * const this);
static void thread_wait(union Thread * const this, IPC_Clock_T const wait_ms);
static void thread_runnable(union Thread * const this);

struct Thread_Cbk_Class Thread_Cbk_Class =
{
  {thread_cbk_delete, NULL},
  NULL,
  NULL,
  NULL,
  NULL
};

struct Thread_Class Thread_Class =
{
   {thread_delete, NULL},
   thread_run,
   thread_wait,
   thread_runnable
};

static union Thread_Cbk Thread_Cbk = {NULL};
static union Thread Thread = {NULL};

void thread_cbk_delete(struct Object * const obj){}

void thread_delete(struct Object * const obj)
{
  Thread_T * const this = (Thread_T *)Object_Cast(&Thread_Class.Class, obj);
  Isnt_Nullptr(this,);

  if(this->cbk->vtbl->join_thread(this->cbk, this))
    {
      _delete(this->cbk)
      free(this->cbk);
      this->cbk = NULL;
    }
}

void thread_run(union Thread * const this)
{
  if(this->cbk->vtbl->run_thread(this->cbk, this))
    {
      if(!this->mux.vtbl->lock(&this->mux, 200)) return;
      this->ready = true;
      this->cond.vtbl->signal(&this->cond);
      this->mux.vtbl->unlock(&this->mux);
    }
}

void thread_runnable(union Thread * const this){}//Implements!

void thread_wait(union Thread * const this, uint32_t const wait_ms)
{
  if(!this->mux.vtbl->lock(&this->mux, wait_ms)) return;
  while(!this->ready)
  {
    this->cond.vtbl->wait(&this->cond, wait_ms);
  }
  this->mux.vtbl->unlock(&this->mux);
}

void Populate_Thread(union Thread * const this, IPC_TID_T const tid)
{
  if(NULL == Thread.vtbl)
    {
      Thread.vtbl = &Thread_Class;
      Thread.tid = IPC_MAX_TID;
      Thread.cbk = NULL;
      Thread.mux = {NULL};
      Thread.cond = {NULL};
      Thread.ready = false;
    }

  memcpy(this, &Thread, sizeof(Thread));
  this->tid = tid;

  IPC_Helper_T * ipc_helper = IPC_get_instance();

  if(tid <IPC_MAX_TID &&
      ipc_helper->vtbl->alloc_thread(ipc_helper, this))
    {
      Populate_Mutex(&this->mux);
      Populate_Conditional(&this->cond, &this->mux);
    }
}
