/*=====================================================================================*/
/**
 * mail.h
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#ifndef MAIL_H_
#define MAIL_H_
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "ipc_types.h"
#include "object.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Define Macros
 *=====================================================================================*/
#define Mail_INHERITS BASE_CLASS

#define Mail_MEMBERS(_member, _class) \
_member(IPC_Mail_Id_T _private, mail_id) \
_member(IPC_Task_Id_T _private, sender_task) \
_member(IPC_Task_Id_T _private, receiver_task) \
_member(void * _private, data) \
_member(size_t _private, data_size) \
_member(bool_t _private, is_dumpable) \

#define Mail_METHODS(_method, _class) \
_method(void , _class, set_data, void const *, size_t const) \
_method(void const *, _class,  get_data, void) \
_method(size_t, _class, get_data_size, void) \
_method(void, _class, dump, void) \
_method(void , _class, set_mail_id, IPC_Mail_Id_T const) \
_method(void , _class, set_sender_task, IPC_Task_Id_T const) \
_method(void , _class, set_receiver_task, IPC_Task_Id_T const) \
_method(IPC_Mail_Id_T, _class, get_mail_id, void) \
_method(IPC_Task_Id_T, _class, get_sender_task, void) \
_method(IPC_Task_Id_T, _class, get_receiver_task, void) \

#define Mail_CONSTRUCTORS(_ctor, _class)

#ifdef __cplusplus
extern "C" {
#endif
/*=====================================================================================* 
 * Exported Type Declarations
 *=====================================================================================*/
Mail_DECL(Mail)
/*=====================================================================================* 
 * Exported Object Declarations
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Prototypes
 *=====================================================================================*/
extern union Mail Mail_Fillup(IPC_Mail_Id_T const mail_id, IPC_Task_Id_T const sender_task, \
      IPC_Task_Id_T const receiver_task, void const * data, size_t const data_size);

extern union Mail * Mail_Fillup_New(IPC_Mail_Id_T const mail_id, IPC_Task_Id_T const sender_task, \
      IPC_Task_Id_T const receiver_task, void const * data, size_t const data_size);
/*=====================================================================================* 
 * Exported Function Like Macros
 *=====================================================================================*/
#ifdef __cplusplus
}
#endif
/*=====================================================================================* 
 * mail.h
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
#endif /*MAIL_H_*/
