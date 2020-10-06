#ifndef IPCFACTORYMOCK_H
#define IPCFACTORYMOCK_H
#include "gmock-barrier.h"
#include "gmock-cv.h"
#include "gmock-mailbox.h"
#include "gmock-mux.h"
#include "gmock-thread.h"
#include "ipc/factory/ipcfactory.h"

#ifdef IPCFACTORYMOCK_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union IPCFactoryMock;
union IPCFactoryMock_Class
{
    union IPCFactory_Class IPCFactory;

    struct
    {
    struct Class Class;
    union Barrier * (* _private alloc_barrier)(union IPCFactoryMock * const ipcfactorymock);
union Conditional* (* _private alloc_conditional)(union IPCFactoryMock * const ipcfactorymock);
union MailboxCbk * (* _private alloc_mailboxcbk)(union IPCFactoryMock * const ipcfactorymock);
union Mutex * (* _private alloc_mutex)(union IPCFactoryMock * const ipcfactorymock);
union Semaphore * (* _private alloc_semaphore)(union IPCFactoryMock * const ipcfactorymock);
union ThreadCbk * (* _private alloc_threadcbk)(union IPCFactoryMock * const ipcfactorymock);
union Timer * (* _private alloc_timer)(union IPCFactoryMock * const ipcfactorymock);
union Clock * (* _private alloc_clock)(union IPCFactoryMock * const ipcfactorymock);

    };
};

union IPCFactoryMock
{
    union IPCFactoryMock_Class * vtbl;
    union IPCFactory IPCFactory;
    struct
    {
      union Object Object;
      mock::GThread * _private thread;
mock::GMailbox * _private mbx;
mock::GMux * _private mux;
mock::GCV * _private cv;

    };
};

extern union IPCFactoryMock_Class * Get_IPCFactoryMock_Class(void);

extern void IPCFactoryMock_populate(union IPCFactoryMock * const ipcfactorymock, mock::GThread * const thread, mock::GMailbox * const mbx, mock::GMux * const mux, mock::GCV * const cv);

extern union Barrier * IPCFactoryMock_alloc_barrier(union IPCFactoryMock * const ipcfactorymock);

extern union Conditional* IPCFactoryMock_alloc_conditional(union IPCFactoryMock * const ipcfactorymock);

extern union MailboxCbk * IPCFactoryMock_alloc_mailboxcbk(union IPCFactoryMock * const ipcfactorymock);

extern union Mutex * IPCFactoryMock_alloc_mutex(union IPCFactoryMock * const ipcfactorymock);

extern union Semaphore * IPCFactoryMock_alloc_semaphore(union IPCFactoryMock * const ipcfactorymock);

extern union ThreadCbk * IPCFactoryMock_alloc_threadcbk(union IPCFactoryMock * const ipcfactorymock);

extern union Timer * IPCFactoryMock_alloc_timer(union IPCFactoryMock * const ipcfactorymock);

extern union Clock * IPCFactoryMock_alloc_clock(union IPCFactoryMock * const ipcfactorymock);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*IPCFACTORYMOCK_H*/