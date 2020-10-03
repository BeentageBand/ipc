#include <string.h>
#include "logger/logger.h"
#include "mail-internal.h"

static void mail_delete(union Mail * const mail);
static void mail_set_payload(union Mail * const mail, void const * const payload, size_t const pay_size);
static void mail_dump(union Mail * const mail);
static void mail_set_mid(union Mail * const mail,IPC_MID_T const);
static void mail_set_sender(union Mail * const mail,IPC_TID_T const);
static void mail_set_receiver(union Mail * const mail,IPC_TID_T const);

void mail_override(union Mail_Class * const clazz)
{
  clazz->Class.destroy = (Class_Delete_T) mail_delete;
  clazz->dump = mail_dump;
  clazz->set_mid = mail_set_mid;
  clazz->set_sender = mail_set_sender;
  clazz->set_receiver = mail_set_receiver;
  clazz->set_payload = mail_set_payload;
}

void mail_delete(union Mail * const mail)
{
  if (NULL != mail->payload) free(mail->payload);
  mail->payload = NULL;
  mail->pay_size = 0;
}

void Mail_populate(union Mail * const mail, IPC_MID_T const mid, IPC_TID_T const sender, IPC_TID_T const receiver, 
                          void * const payload, 
                          size_t const pay_size)
{
  Object_populate(&mail->Object, &Get_Mail_Class()->Class);
  mail->sender = sender;
  mail->receiver = receiver;
  mail->mid = mid;
  mail->vtbl->set_payload(mail, payload, pay_size);
}

void mail_set_payload(union Mail * const mail, void const * const payload, size_t const pay_size)
{
  if(NULL != mail->payload && mail->pay_size != pay_size)
  {
     free(mail->payload);
     mail->payload = NULL;
     mail->pay_size = 0;
  }

  mail->pay_size = pay_size;

  if(mail->pay_size)
  {
    mail->payload = malloc(mail->pay_size);
    memcpy(mail->payload, payload, pay_size);
  }
}

void mail_dump(union Mail * const mail)
{
  //TODO remove implementation
}

void mail_set_mid(union Mail * const mail, IPC_MID_T const mid)
{
  mail->mid = mid;
}

void mail_set_sender(union Mail * const mail, IPC_TID_T const sender_task)
{
  mail->sender = sender_task;
}

void mail_set_receiver(union Mail * const mail, IPC_TID_T const receiver_task)
{
  mail->receiver = receiver_task;
}
