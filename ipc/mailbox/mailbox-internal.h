#ifndef MAILBOX_INT_H
#define MAILBOX_INT_H
#define MAILBOX_IMPLEMENTATION

#include "mailbox.h"

static void mailbox_override(union Mailbox_Class * const mailbox);

union Mailbox_Class * Get_Mailbox_Class(void)
{
  static union Mailbox_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  mailbox_override(&clazz);
  return &clazz;
}
void Mailbox_push_mail(union Mailbox * const mailbox, union Mail * const mail)
{
  mailbox->vtbl->push_mail(mailbox, mail);
}

bool Mailbox_retrieve(union Mailbox * const mailbox, union Mail * const mail)
{
  return mailbox->vtbl->retrieve(mailbox, mail);
}

bool Mailbox_retrieve_only(union Mailbox * const mailbox, union Mail * const mail, IPC_MID_T const mid)
{
  return mailbox->vtbl->retrieve_only(mailbox, mail, mid);
}


#endif /*MAILBOX_INT_H*/
