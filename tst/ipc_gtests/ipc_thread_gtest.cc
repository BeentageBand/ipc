#include "gtest/gtest.h"
#include "tst/mailist.h"
#include "ipc/barrier/barrier.h"
#include "tst/mock/gmock-thread.h"
#include "tst/mock/gmock-barrier.h"
#include "tst/mock/threadmock.h"
#include "tst/mock/barriermock.h"
#include "tst/mailist.h"

using namespace ::testing;
using namespace std;

class Gtest_Thread : public Test
{
    public:
        IPC_TID_T tid;
        union Thread * thread;
        union ThreadMock * cbk;
        union BarrierMock * barrier;
        NiceMock<mock::ThreadMock> mock_cbk;
        NiceMock<mock::BarrierMock> mock_barrier;

    void SetUp(void)
    {
        using ::testing::_;
        this->tid = IPC_GTEST_1_WORKER_TID;
        EXPECT_CALL(this->mock_cbk, register_thread(_)).WillOnce(Return(0));

        this->cbk = _new(union ThreadMock);
        ThreadMock_populate(this->cbk, &this->mock_cbk);

        this->barrier = _new(union BarrierMock);

        BarrierMock_populate(this->barrier, &this->mock_barrier);

        this->thread = _new(union Thread);
        Thread_populate(this->thread, &this->cbk->ThreadCbk, this->tid, &this->barrier->Barrier);

        ASSERT_TRUE(thread);
        ASSERT_EQ(thread->id, IPC_GTEST_1_WORKER_TID);
    }

    void TearDown(void)
    {
        EXPECT_CALL(this->mock_cbk, join_thread()).WillOnce(Return(0));
        EXPECT_CALL(this->mock_cbk, cancel_thread(_));
        _delete(this->thread);
        free(this->thread);
        _delete(this->barrier);
        free(this->barrier);
        _delete(this->cbk);
        free(this->cbk);
    }

};

TEST_F(Gtest_Thread, run)
{
    using ::testing::_;
    EXPECT_CALL(this->mock_cbk, create_thread(_));
    Thread_run(this->thread);
}

TEST_F(Gtest_Thread, wait)
{
    using ::testing::_;
    EXPECT_CALL(this->mock_barrier, wait(1000)).WillOnce(Return(true));
    Thread_wait(this->thread, 1000);
}

TEST_F(Gtest_Thread, join)
{
    EXPECT_CALL(this->mock_cbk, join_thread()).WillOnce(Return(0));
    Thread_join(this->thread, 1000);
}


TEST_F(Gtest_Thread, ready)
{
    EXPECT_CALL(this->mock_barrier, until_ready());
    Thread_ready(this->thread);
}

TEST_F(Gtest_Thread, delete)
{
    using ::testing::_;
    EXPECT_CALL(this->mock_cbk, join_thread()).WillOnce(Return(0));
    EXPECT_CALL(this->mock_cbk, cancel_thread(_));
    _delete(this->thread);

    EXPECT_CALL(this->mock_cbk, register_thread(_)).WillOnce(Return(0));
    this->thread = _new(union Thread);
    Thread_populate(this->thread, &this->cbk->ThreadCbk, this->tid, &this->barrier->Barrier);
}
