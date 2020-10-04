#include "gmock/gmock.h"
#include "tst/mailist.h"
#include "ipc/publisher/publishersingleton.h"

using namespace ::testing;
using namespace std;


class Publisher_Gtest : public Test
{
    public:
    union PublisherSingleton * publisher;
    
    void SetUp(void) {
        this->publisher = _new(union PublisherSingleton);
        PublisherSingleton_populate(this->publisher);
        PublisherSingleton_subscribe(this->publisher, IPC_GTEST_1_WORKER_TID, IPC_GTEST_INT_MID);
    }

    void TearDown(void) {
        _delete(this->publisher);
    }
};


TEST_F(Publisher_Gtest, subscribe)
{
    //EXPECT_CALL(*Mock_IPC::get().mock_rw, wlock(200)).WillOnce(Return(true));
    //EXPECT_CALL(*Mock_IPC::get().mock_rw, unlock());
    bool rc = PublisherSingleton_subscribe(this->publisher, IPC_MAIN_TID, WORKER_INT_SHUTDOWN_MID);
    ASSERT_TRUE(rc);


    //EXPECT_CALL(*Mock_IPC::get().mock_rw, rlock(200)).WillRepeatedly(Return(true));
    //EXPECT_CALL(*Mock_IPC::get().mock_rw, unlock());
    //auto found = subscription.find(IPC_MAIN_TID);
    //ASSERT_FALSE(found == subscription.end());
    //ASSERT_EQ(*found, IPC_MAIN_TID);
}

TEST_F(Publisher_Gtest, publish)
{
    using ::testing::_;
    //shared_ptr<Mock_Sender> mock_sender = make_shared<NiceMock<Mock_Sender>>(IPC_MAIN_TID);
    //Publisher & pub = Publisher::get();

    //EXPECT_CALL(*mock_sender, send(_));
    PublisherSingleton_publish(this->publisher, WORKER_INT_SHUTDOWN_MID, NULL, 0);
}

TEST_F(Publisher_Gtest, unsubscribe)
{

    //EXPECT_CALL(*Mock_IPC::get().mock_rw, wlock(200)).WillRepeatedly(Return(true));
    //EXPECT_CALL(*Mock_IPC::get().mock_rw, unlock());

    bool rc = PublisherSingleton_unsubscribe(this->publisher, IPC_GTEST_1_WORKER_TID ,IPC_GTEST_INT_MID);
    ASSERT_TRUE(rc);

    //EXPECT_CALL(*Mock_IPC::get().mock_rw, wlock(200)).WillRepeatedly(Return(true));
    //EXPECT_CALL(*Mock_IPC::get().mock_rw, unlock());
    //auto subscription = pub.find_subscription(WORKER_INT_SHUTDOWN_MID);
    //ASSERT_TRUE(subscription.empty());

    //EXPECT_CALL(*Mock_IPC::get().mock_rw, rlock(200)).WillRepeatedly(Return(true));
    //EXPECT_CALL(*Mock_IPC::get().mock_rw, unlock()).Times(2);
    //auto found = subscription.find(IPC_MAIN_TID);
    //ASSERT_TRUE(found == subscription.end());
}
