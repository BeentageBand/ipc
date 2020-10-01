#ifndef MOCK_BARRIER_H_
#define MOCK_BARRIER_H_

#include "gmock/gmock.h"
#include "ipc/common/ipc_types.h"

namespace mock {

class GBarrier {
    public:
    virtual void wait(void) = 0;
    virtual bool wait(IPC_Clock_T const wait_ms) = 0;
    virtual void until_ready(void) = 0;
};

class BarrierMock : public GBarrier
{
    public:
    MOCK_METHOD0(wait, void ());
    MOCK_METHOD1(wait, bool (IPC_Clock_T const wait_ms));
    MOCK_METHOD0(until_ready, void ());
};

}

#endif // !MOCK_BARRIER_H_