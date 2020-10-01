#ifndef GMOCK_THREAD_H_
#define GMOCK_THREAD_H_

#include "gmock/gmock.h"
#include "ipc/thread/thread.h"

namespace mock {

class GThread {
    public:
    virtual int register_thread(union Thread & thread) = 0;
    virtual int create_thread(union Thread & thread) = 0;
    virtual int cancel_thread(void *& exit ) = 0;
    virtual int join_thread(void) = 0;
};

class ThreadMock: public GThread
{
    public:
    MOCK_METHOD1(register_thread, int (union Thread & thread));
    MOCK_METHOD1(create_thread, int (union Thread & thread));
    MOCK_METHOD1(cancel_thread, int (void *& exit ));
    MOCK_METHOD0(join_thread, int());
};

}
#endif // GMOCK_THREAD_H_