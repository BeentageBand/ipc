/*=====================================================================================*/
/**
 * mailbox.h
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#ifndef MAILBOX_H_
#define MAILBOX_H_
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "ipc_types.h"
#include "mail.h"
#include "cqueue.h"
#include "mailbox_ext.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Define Macros
 *=====================================================================================*/
#define Mailbox_INHERITS BASE_CLASS

#define Mailbox_MEMBERS(_member, _class) \
_member(IPC_Task_Id_T _private, owner) \
_member(t_Queue(Mail) _private, mailbox) \
_member(size_t _private, data_size) \
_member(union Mailbox_Ext _private * _private, cbk) \

#define Mailbox_METHODS(_method, _class) \
void _method(void, _class, push_mail, Mail_T * const) \
_method(union Mail const *, _class, pop_mail, void) \
_method(union Mail const *, _class, find_mail, IPC_Mail_Id_T const) \

#ifdef __cplusplus
extern "C" {
#endif
/*=====================================================================================* 
 * Exported Type Declarations
 *=====================================================================================*/
CLASS_DECL(Queue, Mail);

CLASS_DECL(Mailbox);
/*=====================================================================================* 
 * Exported Object Declarations
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Prototypes
 *=====================================================================================*/
extern union Mailbox Mailbox_Setup(IPC_Task_Id_T const owner, uint32_t const mail_elems, size_t const data_size);
extern union Mailbox * Mailbox_Setup_New(IPC_Task_Id_T const owner, uint32_t const mail_elems, size_t const data_size);
/*=====================================================================================* 
 * Exported Function Like Macros
 *=====================================================================================*/
#ifdef __cplusplus
}
#endif
/*=====================================================================================* 
 * mailbox.h
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
#endif /*MAILBOX_H_*/
