#include "thread-internal.h"
#include "logger/logger.h"
#include "ipc/semaphore/semaphore.h"

static void thread_cbk_delete(union Thread * const obj);

static void thread_delete(union Thread * const obj);
static void thread_run(union Thread * const thread);
static void thread_wait(union Thread * const thread, IPC_Clock_T const wait_ms);
static void thread_ready(union Thread * const thread);
static void thread_join(union Thread * const thread, IPC_Clock_T const wait_ms);
static void thread_runnable(union Thread * const thread);

void thread_override(union Thread_Class * const clazz)
{
  clazz->Class.destroy = (Class_Delete_T) thread_delete;
  clazz->join = thread_join;
  clazz->wait = thread_wait;
  clazz->run = thread_run;
  clazz->ready = thread_ready;
  clazz->runnable = thread_runnable;
}

void thread_delete(union Thread * const thread)
{
  if(ThreadCbk_join_thread(thread->cbk, thread))
    {
      ThreadCbk_unregister_thread(thread->cbk, thread);
      thread->cbk = NULL;
    }
}

void thread_join(union Thread * const thread, IPC_Clock_T const wait_ms)
{
    //TODO
}


void thread_run(union Thread * const thread)
{
  if(ThreadCbk_run_thread(thread->cbk, thread))
    {
      if(!Mutex_lock(thread->mux, 200)) return;
      thread->ready = true;
      Conditional_signal(thread->cv);
      Mutex_unlock(thread->mux);
    }
}

void thread_runnable(union Thread * const thread){}//Implements!

void thread_wait(union Thread * const thread, uint32_t const wait_ms)
{
  if(!Mutex_lock(thread->mux, wait_ms)) return;
  while(!thread->ready)
  {
    Conditional_wait(thread->cv, wait_ms);
  }
  Mutex_unlock(thread->mux);
}

void thread_ready(union Thread * const thread)
{
  if(!Mutex_lock(thread->mux, 200)) return;
  Conditional_signal(thread->cv);
  Mutex_unlock(thread->mux);
}

void Thread_populate(union Thread * const thread, union ThreadCbk *  const cbk, IPC_TID_T const id, union Mutex * const mux, 
                     union Conditional * const cv)
{
  Object_populate(&thread->Object, &Get_Thread_Class()->Class);
  thread->id = id;
  thread->mux = mux;
  thread->cv = cv;
  thread->ready = false;
  ThreadCbk_register_thread(thread->cbk, thread);
}
