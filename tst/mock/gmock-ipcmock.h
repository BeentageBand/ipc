#ifndef GMOCK_IPCMOCK_H_
#define GMOCK_IPCMOCK_H_
#include "gmock/gmock.h"
#include "ipc/common/ipc_types.h"
#include "ipc/mailbox/mailbox.h"
#include "ipc/thread/thread.h"

namespace mock {

class GIPC {
    public:
    virtual union IPCFactory * get_factory(void) = 0;
    virtual bool register_mbx(union Mailbox * const mbx) = 0;
    virtual bool register_thread(union Thread * const thread) = 0;
    virtual bool unregister_mbx(union Mailbox * const mbx) = 0;
    virtual bool unregister_thread(union Thread * const thread) = 0;
    virtual union Thread * find_thread(IPC_TID_T const tid) = 0;
    virtual union Mailbox * find_mailbox(IPC_MID_T const tid) = 0;
};

class IPCMock : public GIPC {
    public:
    MOCK_METHOD0(get_factory, union IPCFactory * ());
    MOCK_METHOD1(register_mbx, bool (union Mailbox * const mbx));
    MOCK_METHOD1(register_thread, bool (union Thread * const thread));
    MOCK_METHOD1(unregister_mbx, bool (union Mailbox * const mbx));
    MOCK_METHOD1(unregister_thread, bool (union Thread * const thread));
    MOCK_METHOD1(find_thread, union Thread * (IPC_TID_T const tid));
    MOCK_METHOD1(find_mailbox, union Mailbox * (IPC_MID_T const tid));
};

}

#endif // GMOCK_IPCMOCK_H_