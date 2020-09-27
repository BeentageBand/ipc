#ifndef MAILBOXCBK_INT_H
#define MAILBOXCBK_INT_H
#define MAILBOXCBK_IMPLEMENTATION

#include "mailboxcbk.h"

static void mailboxcbk_override(union MailboxCbk_Class * const mailboxcbk);

union MailboxCbk_Class * Get_MailboxCbk_Class(void)
{
  static union MailboxCbk_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  mailboxcbk_override(&clazz);
  return &clazz;
}
bool MailboxCbk_register_mbx(union MailboxCbk * const mailboxcbk, union Mailbox * const mailbox)
{
  return mailboxcbk->vtbl->register_mbx(mailboxcbk, mailbox);
}

bool MailboxCbk_unregister_mbx(union MailboxCbk * const mailboxcbk, union Mailbox * const mailbox)
{
  return mailboxcbk->vtbl->unregister_mbx(mailboxcbk, mailbox);
}


#endif /*MAILBOXCBK_INT_H*/
