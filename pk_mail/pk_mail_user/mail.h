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

#define CLASS_NAME Mail
#define CLASS_INHERITS Object

#define CLASS_MEMBERS(_member) \
_member(TB_Mail_Id_T _private, mail_id) \
_member(TB_Task_Id_T _private, sender_task) \
_member(TB_Task_Id_T _private, receiver_task) \
_member(void * _private, data) \
_member(size_t _private, data_size) \
_member(bool_t _private, is_dumpable) \

#define CLASS_METHODS(_method, _void_method) \
void _method(ctor, IPC_Mail_Id_T const mail_id, IPC_Task_Id_T const sender_task, \
      IPC_Task_Id_T const receiver_task, void const * data, size_t const data_size) \
void _method(set_data, void const *, size_t const) \
void const * _void_method( get_data) \
size_t _void_method(get_data_size) \
void _void_method(dump) \
void _method(set_mail_id, IPC_Mail_Id_T const) \
void _method(set_sender_task, IPC_Task_Id_T const) \
void _method(set_receiver_task, IPC_Task_Id_T const) \
IPC_Mail_Id_T _void_method(get_mail_id) \
IPC_Task_Id_T _void_method(get_sender_task) \
IPC_Task_Id_T _void_method(get_receiver_task) \

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
 * mail.h
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
#endif /*MAIL_H_*/
