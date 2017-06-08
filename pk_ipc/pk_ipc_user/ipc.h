/*=====================================================================================*/
/**
 * ipc.h
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#ifndef IPC_H_
#define IPC_H_
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "ipc_types.h"
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

#define CLASS_NAME IPC
#define CLASS_INHERITS Object
#define CLASS_MEMBERS(_member) \

#define CLASS_METHODS(_method, _void_method) \


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
/**
 * IPC Task Handling
 * This apis should be used in task running
 */
extern IPC_Task_Id_T IPC_self_task_id(void);

extern IPC_Process_Id_T IPC_self_pid(void);

extern void IPC_task_ready(void);

extern void IPC_create_mailbox(uint32_t const max_mails, size_t const mail_size);

/**
 * IPC Mail Comm
 */
extern bool_t IPC_subscribe_mail_list(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems);

extern bool_t IPC_unsubscribe_mail_list(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems);

extern void IPC_send(IPC_Task_Id_T const receiver_task, IPC_Process_Id_T const receiver_pid,
      IPC_Mail_Id_T mail_id, void const * data, size_t const data_size);

extern void IPC_publish(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size);

extern void IPC_broadcast(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size);

extern Mail_T * const IPC_retreive_from_mail_list(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems,
      uint32_t const timeout_ms);

extern Mail_T * const IPC_retreive_mail(uint32_t const timeout_ms);

/**
 * IPC Miscellaneous
 */
extern void IPC_put_date_string(char * date_str);
/*=====================================================================================* 
 * Exported Function Like Macros
 *=====================================================================================*/
#ifdef __cplusplus
}
#endif
/*=====================================================================================* 
 * ipc.h
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
#endif /*IPC_H_*/
