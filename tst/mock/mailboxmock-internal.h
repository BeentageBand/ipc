#ifndef MAILBOXMOCK_INT_H
#define MAILBOXMOCK_INT_H
#define MAILBOXMOCK_IMPLEMENTATION

#include "mailboxmock.h"

#ifdef __cplusplus
extern "C" {
#endif

static void mailboxmock_override(union MailboxMock_Class * const mailboxmock);

union MailboxMock_Class * Get_MailboxMock_Class(void)
{
  static union MailboxMock_Class clazz = {};
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), &Get_MailboxCbk_Class()->Class);
  mailboxmock_override(&clazz);
  return &clazz;
}
bool MailboxMock_register_mbx(union MailboxMock * const mailboxmock, union Mailbox * const mailbox)
{
  return mailboxmock->vtbl->register_mbx(mailboxmock, mailbox);
}

bool MailboxMock_unregister_mbx(union MailboxMock * const mailboxmock, union Mailbox * const mailbox)
{
  return mailboxmock->vtbl->unregister_mbx(mailboxmock, mailbox);
}


#ifdef __cplusplus
}
#endif
#endif /*MAILBOXMOCK_INT_H*/
