#ifndef GMOCK_MAILBOX_H_
#define GMOCK_MAILBOX_H_

#include "gmock/gmock.h"
#include "ipc/mailbox/mailbox.h"

namespace mock {
class GMailbox {

    public:
    virtual bool register_mbx(union Mailbox * const mbx) = 0;
    virtual bool unregister_mbx(union Mailbox * const mbx) = 0;

};
class MailboxMock : public GMailbox {

    public:
    MOCK_METHOD1(register_mbx, bool (union Mailbox * const mbx));
    MOCK_METHOD1(unregister_mbx, bool (union Mailbox * const mbx));
};
}

#endif // !GMOCK_MAILBOX_H_