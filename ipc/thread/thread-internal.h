#ifndef THREAD_INT_H
#define THREAD_INT_H
#define THREAD_IMPLEMENTATION

#include "thread.h"

#ifdef __cplusplus
extern "C" {
#endif

static void thread_override(union Thread_Class * const thread);

union Thread_Class * Get_Thread_Class(void)
{
  static union Thread_Class clazz = {};
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  thread_override(&clazz);
  return &clazz;
}
void Thread_run(union Thread * const thread)
{
  return thread->vtbl->run(thread);
}

void Thread_ready(union Thread * const thread)
{
  return thread->vtbl->ready(thread);
}

void Thread_wait(union Thread * const thread, IPC_Clock_T const wait_ms)
{
  return thread->vtbl->wait(thread, wait_ms);
}

bool Thread_joinable(union Thread * const thread)
{
  return thread->vtbl->joinable(thread);
}

void Thread_join(union Thread * const thread, IPC_Clock_T const wait_ms)
{
  return thread->vtbl->join(thread, wait_ms);
}

void Thread_runnable(union Thread * const thread)
{
  return thread->vtbl->runnable(thread);
}


#ifdef __cplusplus
}
#endif
#endif /*THREAD_INT_H*/
