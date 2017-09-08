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
extern IPC_Task_Id_T IPC_Self_Task_Id(void);

extern void IPC_Task_Ready(void);

extern int IPC_Run(IPC_Task_Id_T const tid);

extern int IPC_Wait(IPC_Task_Id_T const tid);

extern void IPC_Sleep(uint32_t const ms);
/**
 * IPC Mail Comm
 */
extern void IPC_Register_Mailbox(uint32_t const max_mails, size_t const mail_size);

extern void IPC_Unregister_Mailbox(void);

extern bool_t IPC_Subscribe_Mail_List(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems);

extern bool_t IPC_Unsubscribe_Mail_List(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems);

extern void IPC_Send(IPC_Task_Id_T const receiver_task, IPC_Mail_Id_T mail_id,
      void const * data, size_t const data_size);

extern void IPC_Publish(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size);

extern void IPC_Broadcast(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size);

extern Mail_T const * IPC_Retreive_From_Mail_List(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems,
      uint32_t const timeout_ms);

extern Mail_T const * IPC_Retreive_Mail(uint32_t const timeout_ms);

/**
 * IPC Miscellaneous
 */

extern uint32_t IPC_Timestamp(void);

extern bool_t IPC_Time_Elapsed(uint32_t const timestamp);

extern void IPC_Put_Date_String(char * date_str);
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
