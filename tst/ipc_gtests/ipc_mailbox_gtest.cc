#include "tst/mock/gmock-mailbox.h"
#include "tst/mock/gmock-mux.h"
#include "tst/mock/gmock-cv.h"
#include "ipc/mailbox/mailbox.h"
#include "tst/mock/muxmock.h"
#include "tst/mock/mailboxmock.h"
#include "tst/mock/cvmock.h"
#include "tst/mailist.h"

using namespace std;
using namespace ::testing;

class Gtest_Mailbox : public Test
{
    public:
    IPC_TID_T tid;
    NiceMock<mock::MuxMock> mock_mux;
    NiceMock<mock::CVMock> mock_cv;
    NiceMock<mock::MailboxMock> mock_cbk;
    union Mailbox * mailbox;
    union MailboxMock * cbk;
    union MuxMock * mux;
    union CVMock * cv;

    void SetUp(void)
    {
        using ::testing::_;
        this->tid = IPC_GTEST_1_WORKER_TID;

        this->cbk = _new(union MailboxMock);
        MailboxMock_populate(this->cbk, &mock_cbk);

        this->cv = _new(union CVMock);
        CVMock_populate(this->cv, &this->mux->Mutex, &this->mock_cv);

        this->mux = _new(union MuxMock);
        MuxMock_populate(this->mux, &this->mock_mux);

        this->mailbox = _new(union Mailbox);
        Mailbox_populate(this->mailbox, this->tid, &this->mux->Mutex, 
            &this->cv->Conditional, &this->cbk->MailboxCbk);
    }

    void TearDown(void)
    {
        using::testing::_;
        _delete(this->cbk);
        _delete(this->mux);
        _delete(this->cv);
    }
};

TEST_F(Gtest_Mailbox, push_and_tail)
{
    Mail mail = {NULL};
    int payload = 25;
    Mail_populate(&mail, WORKER_INT_SHUTDOWN_MID, this->tid, IPC_GTEST_1_WORKER_TID, 
        &payload, 
        sizeof(payload));
    EXPECT_CALL(this->mock_mux, lock(200)).WillOnce(Return(true));
    EXPECT_CALL(this->mock_cv, signal());
    EXPECT_CALL(this->mock_mux, unlock());
    Mailbox_push_mail(this->mailbox, &mail);


    EXPECT_CALL(this->mock_mux, lock(200)).WillOnce(Return(true));
    EXPECT_CALL(this->mock_mux, unlock());

    union Mail rcv_mail = {NULL}; 
    bool rcv = Mailbox_retrieve(this->mailbox, &rcv_mail);
    ASSERT_TRUE(rcv);
    ASSERT_EQ(WORKER_INT_SHUTDOWN_MID, rcv_mail.mid);
    ASSERT_EQ(this->tid, rcv_mail.sender);
}

TEST_F(Gtest_Mailbox, tail_timeout)
{
    EXPECT_CALL(this->mock_mux, lock(200)).WillOnce(Return(false));

    union Mail rcv_mail = {NULL}; 
    bool rcv = Mailbox_retrieve(this->mailbox, &rcv_mail);
    ASSERT_FALSE(rcv);
}

TEST_F(Gtest_Mailbox, tail_with_mid)
{
    Mail mail = {NULL};
    int payload = 25;
    Mail_populate(&mail, WORKER_INT_SHUTDOWN_MID, this->tid, IPC_GTEST_1_WORKER_TID, 
        &payload, 
        sizeof(payload));
    EXPECT_CALL(this->mock_mux, lock(200)).WillOnce(Return(true));
    EXPECT_CALL(this->mock_cv, signal());
    EXPECT_CALL(this->mock_mux, unlock());
    Mailbox_push_mail(this->mailbox, &mail);

    EXPECT_CALL(this->mock_mux, lock(200)).WillOnce(Return(true));
    EXPECT_CALL(this->mock_mux, unlock());
    union Mail rcv_mail = {NULL};
    bool rcv = Mailbox_retrieve_only(this->mailbox, &rcv_mail, 
        this->tid = IPC_GTEST_1_WORKER_TID);
    ASSERT_TRUE(rcv);
}
