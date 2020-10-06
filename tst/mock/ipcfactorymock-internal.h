#ifndef IPCFACTORYMOCK_INT_H
#define IPCFACTORYMOCK_INT_H
#define IPCFACTORYMOCK_IMPLEMENTATION

#include "ipcfactorymock.h"

#ifdef __cplusplus
extern "C" {
#endif

static void ipcfactorymock_override(union IPCFactoryMock_Class * const ipcfactorymock);

union IPCFactoryMock_Class * Get_IPCFactoryMock_Class(void)
{
  static union IPCFactoryMock_Class clazz = {};
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), &Get_IPCFactory_Class()->Class);
  ipcfactorymock_override(&clazz);
  return &clazz;
}
union Barrier * IPCFactoryMock_alloc_barrier(union IPCFactoryMock * const ipcfactorymock)
{
  return ipcfactorymock->vtbl->alloc_barrier(ipcfactorymock);
}

union Conditional* IPCFactoryMock_alloc_conditional(union IPCFactoryMock * const ipcfactorymock)
{
  return ipcfactorymock->vtbl->alloc_conditional(ipcfactorymock);
}

union MailboxCbk * IPCFactoryMock_alloc_mailboxcbk(union IPCFactoryMock * const ipcfactorymock)
{
  return ipcfactorymock->vtbl->alloc_mailboxcbk(ipcfactorymock);
}

union Mutex * IPCFactoryMock_alloc_mutex(union IPCFactoryMock * const ipcfactorymock)
{
  return ipcfactorymock->vtbl->alloc_mutex(ipcfactorymock);
}

union Semaphore * IPCFactoryMock_alloc_semaphore(union IPCFactoryMock * const ipcfactorymock)
{
  return ipcfactorymock->vtbl->alloc_semaphore(ipcfactorymock);
}

union ThreadCbk * IPCFactoryMock_alloc_threadcbk(union IPCFactoryMock * const ipcfactorymock)
{
  return ipcfactorymock->vtbl->alloc_threadcbk(ipcfactorymock);
}

union Timer * IPCFactoryMock_alloc_timer(union IPCFactoryMock * const ipcfactorymock)
{
  return ipcfactorymock->vtbl->alloc_timer(ipcfactorymock);
}

union Clock * IPCFactoryMock_alloc_clock(union IPCFactoryMock * const ipcfactorymock)
{
  return ipcfactorymock->vtbl->alloc_clock(ipcfactorymock);
}


#ifdef __cplusplus
}
#endif
#endif /*IPCFACTORYMOCK_INT_H*/
