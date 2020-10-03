#ifndef GMOCK_CV_H
#define GMOCK_CV_H

#include "gmock/gmock.h"
#include "ipc/common/ipc_types.h"

namespace mock {
class GCV {
    public:
    virtual bool signal(void) = 0;
    virtual bool wait(IPC_Clock_T const ms) = 0;

};

class CVMock : public GCV {
    public:
    MOCK_METHOD0(signal, bool());
    MOCK_METHOD1(wait, bool (IPC_Clock_T const ms));
};

}


#endif // !GMOCK_CV_H