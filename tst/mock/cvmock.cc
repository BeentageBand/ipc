#include "cvmock-internal.h"


static bool cvmock_signal(union CVMock * const cvmock);
static bool cvmock_wait(union CVMock * const cvmock, IPC_Clock_T const ms);


void cvmock_override(union CVMock_Class * const clazz)
{
    clazz->signal = cvmock_signal;
    clazz->wait = cvmock_wait;
}

bool cvmock_signal(union CVMock * const cvmock)
{
    return cvmock->mock->signal();
}

bool cvmock_wait(union CVMock * const cvmock, IPC_Clock_T const ms)
{
    return cvmock->mock->wait(ms);
}

void CVMock_populate(union CVMock * const cvmock, union Mutex * const mutex, mock::GCV * const mock)
{
    Conditional_populate(&cvmock->Conditional, mutex);
    Object_populate(&cvmock->Object, &Get_CVMock_Class()->Class);
    cvmock->mock = mock;
}