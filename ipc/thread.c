#define COBJECT_IMPLEMENTATION
#define Dbg_FID DBG_FID_DEF(IPC_FID, 5)

#include "logger/logger.h.h"
#include "ipc_helper.h"
#include "thread.h"
#include "sem.h"

static void thread_cbk_delete(struct Object * const obj);

static void thread_delete(struct Object * const obj);
static void thread_run(union Thread * const this);
static void thread_wait(union Thread * const this, IPC_Clock_T const wait_ms);
static void thread_ready(union Thread * const this);
static void thread_join(union Thread * const this, IPC_Clock_T const wait_ms);
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
   thread_ready,
   thread_join,
   thread_runnable
};

static union Thread Thread = {NULL};

void thread_cbk_delete(struct Object * const obj){}

void thread_delete(struct Object * const obj)
{
  Thread_T * const this = (Thread_T *)Object_Cast(&Thread_Class.Class, obj);
  Isnt_Nullptr(this,);

  if(this->cbk->vtbl->join_thread(this->cbk, this))
    {
      this->cbk->vtbl->unregister_thread(this->cbk, this);
      _delete(this->cbk);
      free(this->cbk);
      this->cbk = NULL;
    }
}

void thread_join(union Thread * const this, IPC_Clock_T const wait_ms)
{
    //TODO
}


void thread_run(union Thread * const this)
{
  if(this->cbk->vtbl->run_thread(this->cbk, this))
    {
      if(!this->mux.vtbl->lock(&this->mux, 200)) return;
      this->ready = true;
      this->cv.vtbl->signal(&this->cv);
      this->mux.vtbl->unlock(&this->mux);
    }
}

void thread_runnable(union Thread * const this){}//Implements!

void thread_wait(union Thread * const this, uint32_t const wait_ms)
{
  if(!this->mux.vtbl->lock(&this->mux, wait_ms)) return;
  while(!this->ready)
  {
    this->cv.vtbl->wait(&this->cv, wait_ms);
  }
  this->mux.vtbl->unlock(&this->mux);
}

void thread_ready(union Thread * const this)
{
  if(!this->mux.vtbl->lock(&this->mux, 200)) return;
  this->cv.vtbl->signal(&this->cv);
  this->ready = true;
  this->mux.vtbl->unlock(&this->mux);


}

void Populate_Thread(union Thread * const this, IPC_TID_T const tid)
{
  if(NULL == Thread.vtbl)
  {
    Thread.vtbl = &Thread_Class;
    Thread.tid = IPC_MAX_TID;
  }
  memcpy(this, &Thread, sizeof(Thread));
  this->tid = tid;

  IPC_Helper_T * ipc_helper = IPC_get_instance();

  if(tid <IPC_MAX_TID &&
      ipc_helper->vtbl->alloc_thread(ipc_helper, this))
    {
      Populate_Mutex(&this->mux);
      Populate_Conditional(&this->cv, &this->mux);
      this->cbk->vtbl->register_thread(this->cbk, this);
    }
}
