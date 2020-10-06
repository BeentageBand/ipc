#include "ipcfactorymock-internal.h"
#include "cvmock.h"
#include "mailboxmock.h"
#include "muxmock.h"
#include "threadmock.h"

static union Clock * ipcfactorymock_alloc_clock(union IPCFactoryMock * const ipcfactorymock);
static union Conditional* ipcfactorymock_alloc_conditional(union IPCFactoryMock * const ipcfactorymock);
static union MailboxCbk * ipcfactorymock_alloc_mailboxcbk(union IPCFactoryMock * const ipcfactorymock);
static union Mutex * ipcfactorymock_alloc_mutex(union IPCFactoryMock * const ipcfactorymock);
static union Semaphore * ipcfactorymock_alloc_semaphore(union IPCFactoryMock * const ipcfactorymock);
static union ThreadCbk * ipcfactorymock_alloc_threadcbk(union IPCFactoryMock * const ipcfactorymock);
static union Timer * ipcfactorymock_alloc_timer(union IPCFactoryMock * const ipcfactorymock);

void ipcfactorymock_override(union IPCFactoryMock_Class * clazz)
{
    clazz->alloc_clock = ipcfactorymock_alloc_clock;
    clazz->alloc_conditional = ipcfactorymock_alloc_conditional;
    clazz->alloc_mailboxcbk = ipcfactorymock_alloc_mailboxcbk;
    clazz->alloc_mutex = ipcfactorymock_alloc_mutex;
    clazz->alloc_semaphore = ipcfactorymock_alloc_semaphore;
    clazz->alloc_threadcbk = ipcfactorymock_alloc_threadcbk;
    clazz->alloc_timer = ipcfactorymock_alloc_timer;
}

union Conditional* ipcfactorymock_alloc_conditional(union IPCFactoryMock * const ipcfactorymock)
{
    union CVMock * mock = _new(union CVMock);
    CVMock_populate(mock, IPCFactoryMock_alloc_mutex(ipcfactorymock), ipcfactorymock->cv);
    return &mock->Conditional;
}

union MailboxCbk * ipcfactorymock_alloc_mailboxcbk(union IPCFactoryMock * const ipcfactorymock)
{
    union MailboxMock * mock = _new(union MailboxMock);
    MailboxMock_populate(mock, ipcfactorymock->mbx);
    return &mock->MailboxCbk;
}

union Mutex * ipcfactorymock_alloc_mutex(union IPCFactoryMock * const ipcfactorymock)
{
    union MuxMock * mock = _new(union MuxMock);
    MuxMock_populate(mock, ipcfactorymock->mux);
    return &mock->Mutex;
}

union Semaphore * ipcfactorymock_alloc_semaphore(union IPCFactoryMock * const ipcfactorymock)
{
    return NULL;
}

union ThreadCbk * ipcfactorymock_alloc_threadcbk(union IPCFactoryMock * const ipcfactorymock)
{
    union ThreadMock * mock = _new(union ThreadMock);
    ThreadMock_populate(mock, ipcfactorymock->thread);
    return &mock->ThreadCbk;
}

union Timer * ipcfactorymock_alloc_timer(union IPCFactoryMock * const ipcfactorymock)
{
    return NULL;
}

union Clock * ipcfactorymock_alloc_clock(union IPCFactoryMock * const ipcfactorymock)
{
    return NULL;
}

void IPCFactoryMock_populate(union IPCFactoryMock * const ipcfactorymock)
{
    IPCFactory_populate(&ipcfactorymock->IPCFactory);
}