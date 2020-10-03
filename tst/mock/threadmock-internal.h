#ifndef THREADMOCK_INT_H
#define THREADMOCK_INT_H
#define THREADMOCK_IMPLEMENTATION

#include "threadmock.h"

#ifdef __cplusplus
extern "C" {
#endif

static void threadmock_override(union ThreadMock_Class * const threadmock);

union ThreadMock_Class * Get_ThreadMock_Class(void)
{
  static union ThreadMock_Class clazz = {};
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), &Get_ThreadCbk_Class()->Class);
  threadmock_override(&clazz);
  return &clazz;
}
bool ThreadMock_register_thread(union ThreadMock * const threadmock, union Thread * const thread)
{
  return threadmock->vtbl->register_thread(threadmock, thread);
}

bool ThreadMock_run_thread(union ThreadMock * const threadmock, union Thread * const thread)
{
  return threadmock->vtbl->run_thread(threadmock, thread);
}

bool ThreadMock_join_thread(union ThreadMock * const threadmock, union Thread * const thread)
{
  return threadmock->vtbl->join_thread(threadmock, thread);
}

bool ThreadMock_unregister_thread(union ThreadMock * const threadmock, union Thread * const thread)
{
  return threadmock->vtbl->unregister_thread(threadmock, thread);
}


#ifdef __cplusplus
}
#endif
#endif /*THREADMOCK_INT_H*/
