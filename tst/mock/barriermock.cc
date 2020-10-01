#include "barriermock-internal.h"

static void barriermock_wait(union BarrierMock * const barriermock, IPC_Clock_T const ms);
static void barriermock_ready(union BarrierMock * const barriermock);

void barriermock_override(union BarrierMock_Class * const clazz)
{
    clazz->ready = barriermock_ready;
    clazz->wait = barriermock_wait;
}

void barriermock_wait(union BarrierMock * const barriermock, IPC_Clock_T const ms)
{
    barriermock->mock->wait(ms);
}

void barriermock_ready(union BarrierMock * const barriermock)
{
    barriermock->mock->until_ready();
}

void BarrierMock_populate(union BarrierMock * const barriermock, mock::GBarrier * const mock)
{
    Barrier_populate(&barriermock->Barrier);
    Object_populate(&barriermock->Object, &Get_BarrierMock_Class()->Class);
    barriermock->mock = mock;
}