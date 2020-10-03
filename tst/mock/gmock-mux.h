#ifndef GMOCK_MUX_H_
#define GMOCK_MUX_H_
#include "gmock/gmock.h"
#include "ipc/common/ipc_types.h"

namespace mock {
class GMux {
    public:
    virtual bool lock(IPC_Clock_T const tout_ms) = 0;
    virtual bool unlock (void) = 0;
};

class MuxMock : public GMux
{
    public:
    MOCK_METHOD1(lock, bool (IPC_Clock_T const tout_ms));
    MOCK_METHOD0(unlock, bool ());
};

}

#endif // GMOCK_MUX_H_