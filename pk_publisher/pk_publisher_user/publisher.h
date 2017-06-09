/*=====================================================================================*/
/**
 * publisher.h
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#ifndef PUBLISHER_H_
#define PUBLISHER_H_
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "mail.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Define Macros
 *=====================================================================================*/
#undef CLASS_NAME
#undef CLASS_INHERITS
#undef CLASS_MEMBERS
#undef CLASS_METHODS

#define CLASS_NAME Publisher
#define CLASS_INHERITS Object

#define CLASS_MEMBERS(_member) \
_member(union Vector_Mailbox * _private, mailboxes) \

#define CLASS_METHODS(_method, _void_method) \
void _method(ctor, uint32_t const, size_t const) \
bool_t _method(subscribe, union Mailbox * const, IPC_Mail_Id_T const) \
bool_t _method(unsubscribe, union Mailbox * const, IPC_Mail_Id_T const) \
void _method(publish_mail, Mail_T * const) \

#ifdef __cplusplus
extern "C" {
#endif
/*=====================================================================================* 
 * Exported Type Declarations
 *=====================================================================================*/
union Mailbox;

CLASS_DECLARATION
/*=====================================================================================* 
 * Exported Object Declarations
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Prototypes
 *=====================================================================================*/
extern bool_t Publisher_subscribe(union Mailbox * const mailbox, IPC_Mail_Id_T const mail_id);
extern bool_t Publisher_unsubscribe(union Mailbox * const mailbox, IPC_Mail_Id_T const mail_id);
extern void Publisher_publish_mail(Mail_T * const mail);
/*=====================================================================================* 
 * Exported Function Like Macros
 *=====================================================================================*/
#ifdef __cplusplus
}
#endif
/*=====================================================================================* 
 * publisher.h
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
#endif /*PUBLISHER_H_*/
