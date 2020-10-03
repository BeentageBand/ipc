#ifndef IPCFACTORY_H
#define IPCFACTORY_H
#include "ipc/clock/clock.h"
#include "ipc/mailbox/mailboxcbk.h"
#include "ipc/mailbox/mailbox.h"
#include "ipc/mutex/mutex.h"
#include "ipc/semaphore/semaphore.h"
#include "ipc/thread/threadcbk.h"
#include "ipc/thread/thread.h"
#include "ipc/timer/timer.h"

#ifdef IPCFACTORY_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union IPCFactory;
union IPCFactory_Class
{
    
    struct
    {
    struct Class Class;
    union Barrier * (* _private alloc_barrier)(union IPCFactory * const ipcfactory);
union Conditional* (* _private alloc_conditional)(union IPCFactory * const ipcfactory);
union MailboxCbk * (* _private alloc_mailboxcbk)(union IPCFactory * const ipcfactory);
union Mutex * (* _private alloc_mutex)(union IPCFactory * const ipcfactory);
union Semaphore * (* _private alloc_semaphore)(union IPCFactory * const ipcfactory);
union ThreadCbk * (* _private alloc_threadcbk)(union IPCFactory * const ipcfactory);
union Timer * (* _private alloc_timer)(union IPCFactory * const ipcfactory);
union Clock * (* _private alloc_clock)(union IPCFactory * const ipcfactory);

    };
};

union IPCFactory
{
    union IPCFactory_Class * vtbl;
        struct
    {
      union Object Object;
      
    };
};

extern union IPCFactory_Class * Get_IPCFactory_Class(void);

extern void IPCFactory_populate(union IPCFactory * const ipcfactory);

extern union Barrier * IPCFactory_alloc_barrier(union IPCFactory * const ipcfactory);

extern union Conditional* IPCFactory_alloc_conditional(union IPCFactory * const ipcfactory);

extern union MailboxCbk * IPCFactory_alloc_mailboxcbk(union IPCFactory * const ipcfactory);

extern union Mutex * IPCFactory_alloc_mutex(union IPCFactory * const ipcfactory);

extern union Semaphore * IPCFactory_alloc_semaphore(union IPCFactory * const ipcfactory);

extern union ThreadCbk * IPCFactory_alloc_threadcbk(union IPCFactory * const ipcfactory);

extern union Timer * IPCFactory_alloc_timer(union IPCFactory * const ipcfactory);

extern union Clock * IPCFactory_alloc_clock(union IPCFactory * const ipcfactory);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*IPCFACTORY_H*/