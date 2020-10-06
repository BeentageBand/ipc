#ifndef HELPERMOCK_H
#define HELPERMOCK_H
#include "gmock-ipcmock.h"
#include "ipc/helper/ipchelper.h"

#ifdef HELPERMOCK_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union HelperMock;
union HelperMock_Class
{
    union IPCHelper_Class IPCHelper;

    struct
    {
    struct Class Class;
    union IPCFactory * (* _private get_factory)(union HelperMock * const helpermock);
void (* _private append)(union HelperMock * const helpermock, union IPCHelper * const helper);
bool (* _private register_mbx)(union HelperMock * const helpermock, union Mailbox * const mbx);
bool (* _private register_thread)(union HelperMock * const helpermock, union Thread * const thread);
bool (* _private unregister_mbx)(union HelperMock * const helpermock, union Mailbox * const mbx);
bool (* _private unregister_thread)(union HelperMock * const helpermock, union Thread * const thread);
union Thread * (* _private find_thread)(union HelperMock * const helpermock, IPC_TID_T const tid);
union Mailbox * (* _private find_mailbox)(union HelperMock * const helpermock, IPC_MID_T const tid);

    };
};

union HelperMock
{
    union HelperMock_Class * vtbl;
    union IPCHelper IPCHelper;
    struct
    {
      union Object Object;
      union IPCHelper * _private next;
union IPCFactory * _private factory;
ThreadPool_T _private threadpool;
MailboxPool_T _private mailboxpool;
union Mutex * _private mux;
union Clock * _private clock;
mock::IPCMock * _private mock;

    };
};

extern union HelperMock_Class * Get_HelperMock_Class(void);

extern void HelperMock_populate(union HelperMock * const helpermock, union IPCHelper * const next, union IPCFactory * const factory, ThreadPool_T const threadpool, MailboxPool_T const mailboxpool, union Mutex * const mux, union Clock * const clock, mock::IPCMock * const mock);

extern union IPCFactory * HelperMock_get_factory(union HelperMock * const helpermock);

extern void HelperMock_append(union HelperMock * const helpermock, union IPCHelper * const helper);

extern bool HelperMock_register_mbx(union HelperMock * const helpermock, union Mailbox * const mbx);

extern bool HelperMock_register_thread(union HelperMock * const helpermock, union Thread * const thread);

extern bool HelperMock_unregister_mbx(union HelperMock * const helpermock, union Mailbox * const mbx);

extern bool HelperMock_unregister_thread(union HelperMock * const helpermock, union Thread * const thread);

extern union Thread * HelperMock_find_thread(union HelperMock * const helpermock, IPC_TID_T const tid);

extern union Mailbox * HelperMock_find_mailbox(union HelperMock * const helpermock, IPC_MID_T const tid);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*HELPERMOCK_H*/