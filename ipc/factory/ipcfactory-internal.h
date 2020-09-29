#ifndef IPCFACTORY_INT_H
#define IPCFACTORY_INT_H
#define IPCFACTORY_IMPLEMENTATION

#include "ipcfactory.h"

static void ipcfactory_override(union IPCFactory_Class * const ipcfactory);

union IPCFactory_Class * Get_IPCFactory_Class(void)
{
  static union IPCFactory_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  ipcfactory_override(&clazz);
  return &clazz;
}
union Conditional* IPCFactory_alloc_conditional(union IPCFactory * const ipcfactory)
{
  return ipcfactory->vtbl->alloc_conditional(ipcfactory);
}

union MailboxCbk * IPCFactory_alloc_mailboxcbk(union IPCFactory * const ipcfactory)
{
  return ipcfactory->vtbl->alloc_mailboxcbk(ipcfactory);
}

union Mutex * IPCFactory_alloc_mutex(union IPCFactory * const ipcfactory)
{
  return ipcfactory->vtbl->alloc_mutex(ipcfactory);
}

union Semaphore * IPCFactory_alloc_semaphore(union IPCFactory * const ipcfactory)
{
  return ipcfactory->vtbl->alloc_semaphore(ipcfactory);
}

union ThreadCbk * IPCFactory_alloc_threadcbk(union IPCFactory * const ipcfactory)
{
  return ipcfactory->vtbl->alloc_threadcbk(ipcfactory);
}

union Timer * IPCFactory_alloc_timer(union IPCFactory * const ipcfactory)
{
  return ipcfactory->vtbl->alloc_timer(ipcfactory);
}

union Clock * IPCFactory_alloc_clock(union IPCFactory * const ipcfactory)
{
  return ipcfactory->vtbl->alloc_clock(ipcfactory);
}


#endif /*IPCFACTORY_INT_H*/
