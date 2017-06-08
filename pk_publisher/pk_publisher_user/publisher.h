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
#include "mailbox.h"
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

#define CLASS_NAME Mailbox
#define CLASS_INHERITS Object

#define CLASS_MEMBERS(_member) \
_member(Ring_Buffer_T * _private, mailboxes) \

#define CLASS_METHODS(_method, _void_method) \
bool_t _method(subscribe, Mailbox_T * const mailbox) \
bool_t _method(unsubscribe, Mailbox_T * const mailbox) \
void _method(publish_mail, Mail_T * const mail) \

#ifdef __cplusplus
extern "C" {
#endif
/*=====================================================================================* 
 * Exported Type Declarations
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Object Declarations
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Prototypes
 *=====================================================================================*/
extern bool_t Publisher_subscribe(Mailbox_T * const mailbox);
extern bool_t Publisher_unsubscribe(Mailbox_T * const mailbox);
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
