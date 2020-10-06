#include "helpermock-internal.h"


static union IPCFactory * helpermock_get_factory(union HelperMock * const helpermock);
static void helpermock_append(union HelperMock * const helpermock, union IPCHelper * const helper);
static bool helpermock_register_mbx(union HelperMock * const helpermock, union Mailbox * const mbx);
static bool helpermock_register_thread(union HelperMock * const helpermock, union Thread * const thread);
static bool helpermock_unregister_mbx(union HelperMock * const helpermock, union Mailbox * const mbx);
static bool helpermock_unregister_thread(union HelperMock * const helpermock, union Thread * const thread);
static union Thread * helpermock_find_thread(union HelperMock * const helpermock, IPC_TID_T const tid);
static union Mailbox * helpermock_find_mailbox(union HelperMock * const helpermock, IPC_MID_T const tid);

void helpermock_override(union HelperMock_Class * const clazz)
{
    clazz->append = helpermock_append;
    clazz->register_mbx = helpermock_register_mbx;
    clazz->unregister_mbx = helpermock_unregister_mbx;
    clazz->find_mailbox = helpermock_find_mailbox;
    clazz->register_thread = helpermock_register_thread;
    clazz->unregister_thread = helpermock_unregister_thread;
    clazz->find_thread = helpermock_find_thread;
}

union IPCFactory * helpermock_get_factory(union HelperMock * const helpermock)
{
    return helpermock->factory;
}

void helpermock_append(union HelperMock * const helpermock, union IPCHelper * const helper)
{
    helpermock->next = helper;
}

bool helpermock_register_mbx(union HelperMock * const helpermock, union Mailbox * const mbx)
{
    return helpermock->mock->register_mbx(mbx);
}

bool helpermock_register_thread(union HelperMock * const helpermock, union Thread * const thread)
{
    return helpermock->mock->register_thread(thread);
}

bool helpermock_unregister_mbx(union HelperMock * const helpermock, union Mailbox * const mbx)
{
    return helpermock->mock->unregister_mbx(mbx);
}

bool helpermock_unregister_thread(union HelperMock * const helpermock, union Thread * const thread)
{
    return helpermock->mock->unregister_thread(thread);
}

union Thread * helpermock_find_thread(union HelperMock * const helpermock, IPC_TID_T const tid)
{
    return helpermock->mock->find_thread(tid);
}

union Mailbox * helpermock_find_mailbox(union HelperMock * const helpermock, IPC_MID_T const tid)
{
    return helpermock->mock->find_mailbox(tid);
}

void HelperMock_populate(union HelperMock * const helpermock, union IPCFactory * const factory, 
    mock::IPCMock * const mock)
{
    IPCHelper_populate(&helpermock->IPCHelper, factory);
    Object_populate(&helpermock->Object, &Get_HelperMock_Class()->Class);
    helpermock->mock = mock;
}