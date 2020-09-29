#include "mailbox.h"
#include "mailboxcbk-internal.h"

void mailboxcbk_override(union MailboxCbk_Class * const clazz)
{}

void MailboxCbk_populate(union MailboxCbk * const mailboxcbk)
{
    Object_populate(&mailboxcbk->Object, &Get_MailboxCbk_Class()->Class);
}