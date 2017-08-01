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
#include "conditional.h"
#include "mail.h"
#include "mutex.h"
#include "mail_ringbuffer.h"
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
#undef CLASS_CONSTRUCTORS

#define CLASS_NAME Mailbox
#define CLASS_INHERITS Object

#define CLASS_MEMBERS(_member) \
_member(IPC_Task_Id_T _private, owner) \
_member(Queue_Mail_T _private, mailbox) \
_member(size_t _private, data_size) \
_member(Mutex_T _private * _private, mutex) \
_member(Conditional_T _private * _private, cond) \

#define CLASS_METHODS(_method, _void_method) \
      void _method(ctor, IPC_Task_Id_T const owner, uint32_t const mail_elems, size_t const data_size) \
      bool_t _method(subscribe, IPC_Mail_Id_T const) \
      bool_t _method(unsubscribe, IPC_Mail_Id_T const) \
      void _method(push_mail, Mail_T * const) \
      Mail_T const * _method(pop_mail, uint32_t const) \
      Mail_T const * _method(get_mail_by_mail_id, IPC_Mail_Id_T const * const, uint32_t const, uint32_t const) \
      void _void_method(dump) \

#define CLASS_CONSTRUCTORS(_ctor)

#ifdef __cplusplus
extern "C" {
#endif
/*=====================================================================================* 
 * Exported Type Declarations
 *=====================================================================================*/
CLASS_DECLARATION
/*=====================================================================================* 
 * Exported Object Declarations
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Prototypes
 *=====================================================================================*/

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
