#ifndef IPCFACTORY_H
#define IPCFACTORY_H
#include "ipc/conditional/conditional.h"
#include "ipc/mailbox/mailboxcbk.h"
#include "ipc/mutex/mutex.h"
#include "ipc/semaphore/semaphore.h"
#include "ipc/thread/threadcbk.h"
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
    void (* _private alloc_conditional)(union IPCFactory * const ipcfactory);
void (* _private alloc_mailboxcbk)(union IPCFactory * const ipcfactory);
void (* _private alloc_mutex)(union IPCFactory * const ipcfactory);
void (* _private alloc_semaphore)(union IPCFactory * const ipcfactory);
void (* _private alloc_threadcbk)(union IPCFactory * const ipcfactory);
void (* _private alloc_timer)(union IPCFactory * const ipcfactory);

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

extern void IPCFactory_alloc_conditional(union IPCFactory * const ipcfactory);

extern void IPCFactory_alloc_mailboxcbk(union IPCFactory * const ipcfactory);

extern void IPCFactory_alloc_mutex(union IPCFactory * const ipcfactory);

extern void IPCFactory_alloc_semaphore(union IPCFactory * const ipcfactory);

extern void IPCFactory_alloc_threadcbk(union IPCFactory * const ipcfactory);

extern void IPCFactory_alloc_timer(union IPCFactory * const ipcfactory);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*IPCFACTORY_H*/