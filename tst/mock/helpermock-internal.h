#ifndef HELPERMOCK_INT_H
#define HELPERMOCK_INT_H
#define HELPERMOCK_IMPLEMENTATION

#include "helpermock.h"

#ifdef __cplusplus
extern "C" {
#endif

static void helpermock_override(union HelperMock_Class * const helpermock);

union HelperMock_Class * Get_HelperMock_Class(void)
{
  static union HelperMock_Class clazz = {};
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), &Get_IPCHelper_Class()->Class);
  helpermock_override(&clazz);
  return &clazz;
}
union IPCFactory * HelperMock_get_factory(union HelperMock * const helpermock)
{
  return helpermock->vtbl->get_factory(helpermock);
}

void HelperMock_append(union HelperMock * const helpermock, union IPCHelper * const helper)
{
  return helpermock->vtbl->append(helpermock, helper);
}

bool HelperMock_register_mbx(union HelperMock * const helpermock, union Mailbox * const mbx)
{
  return helpermock->vtbl->register_mbx(helpermock, mbx);
}

bool HelperMock_register_thread(union HelperMock * const helpermock, union Thread * const thread)
{
  return helpermock->vtbl->register_thread(helpermock, thread);
}

bool HelperMock_unregister_mbx(union HelperMock * const helpermock, union Mailbox * const mbx)
{
  return helpermock->vtbl->unregister_mbx(helpermock, mbx);
}

bool HelperMock_unregister_thread(union HelperMock * const helpermock, union Thread * const thread)
{
  return helpermock->vtbl->unregister_thread(helpermock, thread);
}

union Thread * HelperMock_find_thread(union HelperMock * const helpermock, IPC_TID_T const tid)
{
  return helpermock->vtbl->find_thread(helpermock, tid);
}

union Mailbox * HelperMock_find_mailbox(union HelperMock * const helpermock, IPC_MID_T const tid)
{
  return helpermock->vtbl->find_mailbox(helpermock, tid);
}


#ifdef __cplusplus
}
#endif
#endif /*HELPERMOCK_INT_H*/
