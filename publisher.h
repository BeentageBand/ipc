#ifndef PUBLISHER_H_
#define PUBLISHER_H_
 
#include "mailbox.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool_t Publisher_subscribe(union Mailbox * const mailbox, IPC_Mail_Id_T const mail_id);

extern bool_t Publisher_unsubscribe(union Mailbox * const mailbox, IPC_Mail_Id_T const mail_id);

extern void Publisher_publish_mail(union Mail * const mail);
 
#ifdef __cplusplus
}
#endif
 
#endif /*PUBLISHER_H_*/
