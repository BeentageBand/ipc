#include "gmock/gmock.h"
#include "tst/mailist.h"
#include "ipc/mail/mail.h"

using namespace std;

static union Mail mail = {NULL};

TEST(Mail, constructor)
{
    int payload = 25;
    Mail_populate(&mail, WORKER_INT_SHUTDOWN_MID, IPC_GTEST_1_WORKER_TID, 
        IPC_GTEST_1_WORKER_TID, 
        &payload, 
        sizeof(payload));
    ASSERT_EQ(WORKER_INT_SHUTDOWN_MID, mail.mid);
    ASSERT_EQ(IPC_GTEST_1_WORKER_TID , mail.receiver);
    ASSERT_NE(nullptr, mail.payload);
    ASSERT_EQ(sizeof(payload), mail.pay_size);
    ASSERT_EQ(payload, *static_cast<int *>(mail.payload));
}

// Valgrind, mail copies payload
TEST(Mail, destructor)
{
    _delete(&mail);
}