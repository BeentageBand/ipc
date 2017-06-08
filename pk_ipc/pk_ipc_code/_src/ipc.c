/*=====================================================================================*/
/**
 * ipc.cpp
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/

/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "ipc.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Local X-Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Define Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Type Definitions
 *=====================================================================================*/
 
/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
 IPC_Task_Id_T IPC_self_task_id(void)
 {
    IPC_OS tb = IPC_OS_get_instance(0,0);
    Isnt_Nullptr(tb, 0);
    return IPC_OS_search_task(tb);
 }


 IPC_Process_Id_T IPC_self_pid(void)
 {
    IPC_OS tb = IPC_OS_get_instance(0,0);
    Isnt_Nullptr(tb, 0);
    return IPC_OS_search_pid(tb);
 }

 void IPC_task_ready(void)
 {
    IPC_OS tb = IPC_OS_get_instance(0,0);
    IPC_OS_notify_ready(tb, IPC_self_task_id());
 }

 void IPC_create_mailbox(uint32_t const max_mails, size_t const mail_size)
 {
    IPC_OS tb = IPC_OS_get_instance(0,0);
    IPC_OS_set_mailbox(tb, max_mails, mail_size);
 }

/**
 * TB Mail Comm
 */
 bool_t IPC_subscribe_mail_list(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems)
 {
    Publisher publisher = Publisher_get_instance(0,0);
    return Publisher_subscribe(publisher,IPC_self_task_id(),  mail_list, mail_elems);
 }

 bool_t IPC_unsubscribe_mail_list(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems)
 {
    Publisher publisher = Publisher_get_instance(0,0);
    return Publisher_unsubscribe(publisher,IPC_self_task_id(),  mail_list, mail_elems);
 }

 void IPC_send(IPC_Task_Id_T const receiver_task, IPC_Process_Id_T const receiver_pid,
      IPC_Mail_Id_T mail_id, void const * data, size_t const data_size)
 {
    IPC_OS tb = IPC_OS_get_instance(0,0);
    Mailbox mailbox = IPC_OS_search_mailbox(tb, receiver_task, receiver_pid);
    IPC_Mail mail = IPC_Mail_New(mail_id, IPC_OS_search_task(tb), receiver_task, receiver_pid, data, data_size);
    Mailbox_push_mail(mailbox, mail);
 }

 void IPC_publish(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size)
 {
    Publisher publisher = Publisher_get_instance(0,0);
    IPC_Mail mail = IPC_Mail_New(mail_id, IPC_self_task_id(), 0, 0, data, data_size);
    Publisher_publish(publisher, mail);
 }

 void IPC_broadcast(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size)
 {
    IPC_OS tb = IPC_OS_get_instance(0,0);
    IPC_Task_Id_T task = 0;
    IPC_Process_Id_T process = 0;
    Mailbox mailbox = NULL;
    IPC_Mail mail = NULL;

    for(process = 0 ; process < MAX_PROCESSES; ++process)
    {
       for(task = 0 ; task < MAX_TASKS; ++task)
       {
          mailbox = IPC_OS_search_mailbox(tb, task, process);
          if(NULL != mailbox)
          {
             mail = IPC_Mail_New(mail_id, IPC_OS_search_task(tb), task, process, data, data_size);
             Mailbox_push_mail(mailbox, mail);
             IPC_Mail_delete(mail);
          }
       }
    }
 }

Mail_T * const IPC_retreive_from_mail_list(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems,
      uint32_t const timeout_ms)
 {
    uint32_t i;
    IPC_OS tb = IPC_OS_get_instance(0,0);
    IPC_Mail mail = NULL;
    Mailbox mailbox = IPC_OS_search_mailbox(tb, IPC_self_task_id(), IPC_self_pid());

    Isnt_Nullptr(mail_list, NULL);

    for(i = 0; i < mail_elems; ++i)
    {
       mail = Mailbox_get_mail_by_mail_id(mailbox, mail_list[i]);
       if(NULL != mail)
       {
          IPC_Mail_dump(mail);
          break;
       }
    }
    return mail;
 }

Mail_T * const IPC_retreive_mail(uint32_t const timeout_ms)
 {
    IPC_OS tb = IPC_OS_get_instance(0,0);
    IPC_Mail mail = NULL;
    Mailbox mailbox = NULL;
    mailbox = IPC_OS_search_mailbox(tb, IPC_self_task_id(), IPC_self_pid());

    mail = Mailbox_get_first_mail(mailbox);
    IPC_Mail_dump(mail);

    return mail;
 }

 void IPC_put_date_string(char * date_str)
 {
    IPC_OS tb = IPC_OS_get_instance(0,0);

    Isnt_Nullptr(tb, );
    Isnt_Nullptr(date_str, );
    if(strlen(date_str) >= IPC_OS_get_date_length(tb) )
    {
       memcpy(date_str, IPC_OS_get_date(tb), IPC_OS_get_date_length(tb));
    }
 }
/*=====================================================================================* 
 * ipc.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
