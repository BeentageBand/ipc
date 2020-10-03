#ifndef MAILBOXMOCK_H
#define MAILBOXMOCK_H
#include "ipc/mailbox/mailboxcbk.h"
#include "gmock-mailbox.h"

#ifdef MAILBOXMOCK_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union MailboxMock;
union MailboxMock_Class
{
    union MailboxCbk_Class MailboxCbk;

    struct
    {
    struct Class Class;
    bool (* _private register_mbx)(union MailboxMock * const mailboxmock, union Mailbox * const mailbox);
bool (* _private unregister_mbx)(union MailboxMock * const mailboxmock, union Mailbox * const mailbox);

    };
};

union MailboxMock
{
    union MailboxMock_Class * vtbl;
    union MailboxCbk MailboxCbk;
    struct
    {
      union Object Object;
      mock::GMailbox * _private mock;

    };
};

extern union MailboxMock_Class * Get_MailboxMock_Class(void);

extern void MailboxMock_populate(union MailboxMock * const mailboxmock, mock::GMailbox * const mock);

extern bool MailboxMock_register_mbx(union MailboxMock * const mailboxmock, union Mailbox * const mailbox);

extern bool MailboxMock_unregister_mbx(union MailboxMock * const mailboxmock, union Mailbox * const mailbox);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*MAILBOXMOCK_H*/