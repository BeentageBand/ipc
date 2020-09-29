#ifndef THREADCBK_INT_H
#define THREADCBK_INT_H
#define THREADCBK_IMPLEMENTATION

#include "threadcbk.h"

static void threadcbk_override(union ThreadCbk_Class * const threadcbk);

union ThreadCbk_Class * Get_ThreadCbk_Class(void)
{
  static union ThreadCbk_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  threadcbk_override(&clazz);
  return &clazz;
}
bool ThreadCbk_register_thread(union ThreadCbk * const threadcbk, union Thread * const thread)
{
  return threadcbk->vtbl->register_thread(threadcbk, thread);
}

bool ThreadCbk_run_thread(union ThreadCbk * const threadcbk, union Thread * const thread)
{
  return threadcbk->vtbl->run_thread(threadcbk, thread);
}

bool ThreadCbk_join_thread(union ThreadCbk * const threadcbk, union Thread * const thread)
{
  return threadcbk->vtbl->join_thread(threadcbk, thread);
}

bool ThreadCbk_unregister_thread(union ThreadCbk * const threadcbk, union Thread * const thread)
{
  return threadcbk->vtbl->unregister_thread(threadcbk, thread);
}


#endif /*THREADCBK_INT_H*/
