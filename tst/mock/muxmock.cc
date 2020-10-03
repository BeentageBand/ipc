#include "muxmock-internal.h"

static bool muxmock_unlock(union MuxMock * const muxmock);
static bool muxmock_lock(union MuxMock * const muxmock, IPC_Clock_T const ms);

void muxmock_override(union MuxMock_Class * const clazz)
{
    clazz->lock = muxmock_lock;
    clazz->unlock = muxmock_unlock;
}

bool muxmock_unlock(union MuxMock * const muxmock)
{
    return muxmock->mock->unlock();
}

bool muxmock_lock(union MuxMock * const muxmock, IPC_Clock_T const ms)
{
    return muxmock->mock->lock(ms);
}

void MuxMock_populate(union MuxMock * const muxmock, mock::GMux * const mock)
{
    Mutex_populate(&muxmock->Mutex);
    Object_populate(&muxmock->Object, &Get_MuxMock_Class()->Class);
    muxmock->mock = mock;
}