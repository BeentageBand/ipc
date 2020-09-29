#ifndef MAIL_INT_H
#define MAIL_INT_H
#define MAIL_IMPLEMENTATION

#include "mail.h"

static void mail_override(union Mail_Class * const mail);

union Mail_Class * Get_Mail_Class(void)
{
  static union Mail_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  mail_override(&clazz);
  return &clazz;
}
void Mail_dump(union Mail * const mail)
{
  return mail->vtbl->dump(mail);
}

void Mail_set_payload(union Mail * const mail, void * const data, size_t const data_size)
{
  return mail->vtbl->set_payload(mail, data, data_size);
}

void Mail_set_mid(union Mail * const mail, IPC_MID_T const mid)
{
  return mail->vtbl->set_mid(mail, mid);
}

void Mail_set_sender(union Mail * const mail, IPC_TID_T const sender)
{
  return mail->vtbl->set_sender(mail, sender);
}

void Mail_set_receiver(union Mail * const mail, IPC_TID_T const receiver)
{
  return mail->vtbl->set_receiver(mail, receiver);
}


#endif /*MAIL_INT_H*/
