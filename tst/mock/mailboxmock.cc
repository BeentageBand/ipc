#include "mailboxmock-internal.h"


static bool mailboxmock_register_mbx(union MailboxMock * const mailboxmock, union Mailbox * const mailbox);
static bool mailboxmock_unregister_mbx(union MailboxMock * const mailboxmock, union Mailbox * const mailbox);

void mailboxmock_override(union MailboxMock_Class * const clazz)
{
    clazz->register_mbx = mailboxmock_register_mbx;
    clazz->unregister_mbx = mailboxmock_unregister_mbx;
}

bool mailboxmock_register_mbx(union MailboxMock * const mailboxmock, union Mailbox * const mailbox)
{
    return mailboxmock->mock->register_mbx(mailbox);
}

bool mailboxmock_unregister_mbx(union MailboxMock * const mailboxmock, union Mailbox * const mailbox)
{
    return mailboxmock->mock->unregister_mbx(mailbox);
}

void MailboxMock_populate(union MailboxMock * const mailboxmock, mock::GMailbox * const mock)
{
    MailboxCbk_populate(&mailboxmock->MailboxCbk);
    Object_populate(&mailboxmock->Object, &Get_MailboxCbk_Class()->Class);
    mailboxmock->mock = mock;
}
