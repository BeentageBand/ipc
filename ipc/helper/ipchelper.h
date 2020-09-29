#ifndef IPCHELPER_H
#define IPCHELPER_H
#include "ipc/clock/clock.h"
#include "ipc/mailbox/mailbox.h"
#include "ipc/factory/ipcfactory.h"
#include "ipc/thread/thread.h"

typedef union Mailbox * Mailbox_Ptr;

#define CHashMap_Params int, Mailbox_Ptr
#include "ctemplate-lib/hashmap/chashmap-template.h"
#undef CHashMap_Params

typedef union Thread * Thread_Ptr;

#define CHashMap_Params int, Thread_Ptr
#include "ctemplate-lib/hashmap/chashmap-template.h"
#undef CHashMap_Params

typedef union CHashMap_int_Mailbox_Ptr MailboxPool_T;
typedef union CHashMap_int_Thread_Ptr ThreadPool_T;

#ifdef IPCHELPER_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif


union IPCHelper;
union IPCHelper_Class
{
    
    struct
    {
    struct Class Class;
    union IPCFactory * (* _private get_factory)(union IPCHelper * const ipchelper);
    void (* _private append)(union IPCHelper * const ipchelper, union IPCHelper * const helper);
    bool (* _private register_mbx)(union IPCHelper * const ipchelper, union Mailbox * const mbx);
    bool (* _private register_thread)(union IPCHelper * const ipchelper, union Thread * const thread);
    bool (* _private unregister_mbx)(union IPCHelper * const ipchelper, union Mailbox * const mbx);
    bool (* _private unregister_thread)(union IPCHelper * const ipchelper, union Thread * const thread);
    union Thread * (* _private find_thread)(union IPCHelper * const ipchelper, IPC_TID_T const tid);
    union Mailbox * (* _private find_mailbox)(union IPCHelper * const ipchelper, IPC_MID_T const tid);
    };
};

union IPCHelper
{
    union IPCHelper_Class * vtbl;
        struct
    {
      union Object Object;
      union IPCHelper * _private next;
union IPCFactory * _private factory;
ThreadPool_T _private threadpool;
MailboxPool_T _private mailboxpool;
union Mutex * _private mux;
union Clock * _private clock;

    };
};

extern union IPCHelper_Class * Get_IPCHelper_Class(void);

extern void IPCHelper_populate(union IPCHelper * const ipchelper, union IPCFactory * const factory);

extern union IPCFactory * IPCHelper_get_factory(union IPCHelper * const ipchelper);

extern void IPCHelper_append(union IPCHelper * const ipchelper, union IPCHelper * const helper);

extern bool IPCHelper_register_mbx(union IPCHelper * const ipchelper, union Mailbox * const mbx);

extern bool IPCHelper_register_thread(union IPCHelper * const ipchelper, union Thread * const thread);

extern bool IPCHelper_unregister_mbx(union IPCHelper * const ipchelper, union Mailbox * const mbx);

extern bool IPCHelper_unregister_thread(union IPCHelper * const ipchelper, union Thread * const thread);

extern union Thread * IPCHelper_find_thread(union IPCHelper * const ipchelper, IPC_TID_T const tid);

extern union Mailbox * IPCHelper_find_mailbox(union IPCHelper * const ipchelper, IPC_MID_T const tid);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*IPCHELPER_H*/