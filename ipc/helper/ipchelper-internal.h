#ifndef IPCHELPER_INT_H
#define IPCHELPER_INT_H
#define IPCHELPER_IMPLEMENTATION

#include "ipchelper.h"

static void ipchelper_override(union IPCHelper_Class * const ipchelper);

union IPCHelper_Class * Get_IPCHelper_Class(void)
{
  static union IPCHelper_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  ipchelper_override(&clazz);
  return &clazz;
}
union IPCFactory * IPCHelper_get_factory(union IPCHelper * const ipchelper)
{
  return ipchelper->vtbl->get_factory(ipchelper);
}

void IPCHelper_append(union IPCHelper * const ipchelper, union IPCHelper * const helper)
{
  return ipchelper->vtbl->append(ipchelper, helper);
}

bool IPCHelper_register_mbx(union IPCHelper * const ipchelper, union Mailbox * const mbx)
{
  return ipchelper->vtbl->register_mbx(ipchelper, mbx);
}

bool IPCHelper_register_thread(union IPCHelper * const ipchelper, union Thread * const thread)
{
  return ipchelper->vtbl->register_thread(ipchelper, thread);
}

bool IPCHelper_unregister_mbx(union IPCHelper * const ipchelper, union Mailbox * const mbx)
{
  return ipchelper->vtbl->unregister_mbx(ipchelper, mbx);
}

bool IPCHelper_unregister_thread(union IPCHelper * const ipchelper, union Thread * const thread)
{
  return ipchelper->vtbl->unregister_thread(ipchelper, thread);
}

union Thread * IPCHelper_find_thread(union IPCHelper * const ipchelper, IPC_TID_T const tid)
{
  return ipchelper->vtbl->find_thread(ipchelper, tid);
}

union Mailbox * IPCHelper_find_mailbox(union IPCHelper * const ipchelper, IPC_MID_T const tid)
{
  return ipchelper->vtbl->find_mailbox(ipchelper, tid);
}


#endif /*IPCHELPER_INT_H*/
