/*=====================================================================================*/
/**
 * system_clk.cpp
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#define CLASS_IMPLEMENTATION
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "system_clk.h"
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
static void System_Clk_Ctor(System_Clk_T * const this, System_Clk_Process_Id_T const pid, uint32_t const max_tasks);
static System_Clk_Ms_T  System_Clk_timestamp(System_Clk_T * const this);
/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEFINITION

/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/
void System_Clk_init(void)
{
   printf("%s \n", __FUNCTION__);

   System_Clk_PID = 0;
   System_Clk_Task_Stack = Vector_Task();
   System_Clk_Mailbox_Stack = Vector_Mailbox();
   System_Clk_Max_Tasks = 0;

   System_Clk_Vtbl.Object.rtti = &System_Clk_Rtti;
   System_Clk_Vtbl.Object.destroy = System_Clk_Dtor;
   System_Clk_Vtbl.ctor = System_Clk_Ctor;
   System_Clk_Vtbl.get_tid = NULL;
   System_Clk_Vtbl.get_pid = NULL;
   System_Clk_Vtbl.search_task = System_Clk_search_task;
   System_Clk_Vtbl.set_mailbox = NULL;
   System_Clk_Vtbl.search_mailbox = System_Clk_search_mailbox;
   System_Clk_Vtbl.notify_ready = NULL;
   System_Clk_Vtbl.get_date_length = NULL;
   System_Clk_Vtbl.get_date = NULL;
}

void System_Clk_shut(void) {}

void System_Clk_Dtor(Object_T * const obj)
{
   System_Clk_T * const this = _dynamic_cast(System_Clk, obj);
   Isnt_Nullptr(this,);

}

bool_t System_Clk_Is_This_Task(Task_T * const task, uint32_t const tid)
{
   Isnt_Nullptr(task, false);
   return tid == task->tid;
}
bool_t System_Clk_Is_This_Mailbox(Mailbox_T * const mbx, uint32_t const tid)
{
   Isnt_Nullptr(mbx, false);
   return tid == mbx->owner;
}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void System_Clk_Ctor(System_Clk_T * const this, System_Clk_Process_Id_T const pid, uint32_t const max_tasks)
{
   System_Clk_PID = pid;
   System_Clk_Max_Tasks = max_tasks;
}

Task_T * const System_Clk_search_task(System_Clk_T * const this, System_Clk_Task_Id_T const task_id)
{
   Task_T * const it = Vector_Task_find_if(System_Clk_Task_Stack.vtbl->front(&System_Clk_Task_Stack),
         System_Clk_Task_Stack.vtbl->back(&System_Clk_Task_Stack), task_id, System_Clk_Is_This_Task);
   return it;
}

Mailbox_T * const System_Clk_search_mailbox(System_Clk_T * const this, System_Clk_Task_Id_T const task_id, System_Clk_Process_Id_T const pid)
{
   Mailbox_T * const it = Vector_Mailbox_find_if(System_Clk_Mailbox_Stack.vtbl->front(&System_Clk_Mailbox_Stack),
         System_Clk_Mailbox_Stack.vtbl->back(&System_Clk_Mailbox_Stack), task_id, System_Clk_Is_This_Mailbox);
   return it;
}

System_Clk_Task_Id_T System_Clk_self_task_id(void)
{
   Singleton_System_Clk(&System_Clk_Singleton);
   Isnt_Nullptr(System_Clk_Singleton->vtbl->get_pid, 0);
   return System_Clk_Singleton->vtbl->get_pid(System_Clk_Singleton);
}


System_Clk_Process_Id_T System_Clk_self_pid(void)
{
   Singleton_System_Clk(&System_Clk_Singleton);
   Isnt_Nullptr(System_Clk_Singleton->vtbl->get_pid, 0);
   return System_Clk_Singleton->vtbl->get_pid(System_Clk_Singleton);
}

void System_Clk_task_ready(void)
{
   Singleton_System_Clk(&System_Clk_Singleton);
   Isnt_Nullptr(System_Clk_Singleton->vtbl->notify_ready, );
   System_Clk_Singleton->vtbl->notify_ready(System_Clk_Singleton, System_Clk_self_task_id());
}

void System_Clk_create_mailbox(uint32_t const max_mails, size_t const mail_size)
{
   Singleton_System_Clk(&System_Clk_Singleton);
   Isnt_Nullptr(System_Clk_Singleton->vtbl->set_mailbox, );
   System_Clk_Singleton->vtbl->set_mailbox(System_Clk_Singleton, max_mails, mail_size);
}

/**
 * TB Mail Comm
 */
bool_t System_Clk_subscribe_mail_list(System_Clk_Mail_Id_T const * mail_list, uint32_t const mail_elems)
{
   bool_t success = true;
   Mailbox_T * mailbox = NULL;
   uint32_t i;
   for(i = 0; i < mail_elems && success; ++i)
   {
      mailbox = System_Clk_Singleton->vtbl->search_mailbox(System_Clk_Singleton, System_Clk_self_task_id(), System_Clk_self_pid());
      success = Publisher_subscribe(mailbox, mail_list[i]);
   }
   return success;
}

bool_t System_Clk_unsubscribe_mail_list(System_Clk_Mail_Id_T const * mail_list, uint32_t const mail_elems)
{

   bool_t success = true;
   Mailbox_T * mailbox = NULL;
   uint32_t i;
   for(i = 0; i < mail_elems && success; ++i)
   {
      mailbox = System_Clk_Singleton->vtbl->search_mailbox(System_Clk_Singleton, System_Clk_self_task_id(), System_Clk_self_pid());
      success = Publisher_unsubscribe(mailbox,  mail_list[i]);
   }
   return success;
}

void System_Clk_send(System_Clk_Task_Id_T const receiver_task, System_Clk_Process_Id_T const receiver_pid,
      System_Clk_Mail_Id_T mail_id, void const * data, size_t const data_size)
{
   Singleton_System_Clk(&System_Clk_Singleton);
   Mailbox_T * mailbox = System_Clk_Singleton->vtbl->search_mailbox(System_Clk_Singleton, receiver_task, receiver_pid);
   Isnt_Nullptr(System_Clk_Singleton->vtbl->get_pid,);
   Mail_T * mail = Mail_new();
   mail->vtbl->ctor(mail, mail_id, System_Clk_self_task_id(), receiver_task, data, data_size);
   mailbox->vtbl->push_mail(mailbox, mail);
}

void System_Clk_publish(System_Clk_Mail_Id_T const mail_id, void const * data, size_t const data_size)
{
   Mail_T * mail = Mail_new();
   mail->vtbl->ctor(mail, mail_id, System_Clk_self_task_id(), 0, data, data_size);
   Publisher_publish_mail(mail);
}

void System_Clk_broadcast(System_Clk_Mail_Id_T const mail_id, void const * data, size_t const data_size)
{
   Singleton_System_Clk(&System_Clk_Singleton);
   Isnt_Nullptr(System_Clk_Singleton->vtbl->search_mailbox, );
   Isnt_Nullptr(System_Clk_Singleton->vtbl->get_pid, );
   System_Clk_Task_Id_T task = 0;
   System_Clk_Process_Id_T process = 0;
   Mailbox_T * mailbox = NULL;
   Mail_T * mail = NULL;

   for(process = 0 ; process < System_Clk_MAX_PROCESSES; ++process)
   {
      for(task = 0 ; task < System_Clk_MAX_TASKS; ++task)
      {
         mailbox = System_Clk_Singleton->vtbl->search_mailbox(System_Clk_Singleton, task, process);
         if(NULL != mailbox)
         {
            mail = Mail_new();
            mail->vtbl->ctor(mail, mail_id, System_Clk_self_task_id(), task, data, data_size);
            mailbox->vtbl->push_mail(mailbox, mail);
            _delete(mail);
         }
      }
   }
}

Mail_T * const System_Clk_retreive_from_mail_list(System_Clk_Mail_Id_T const * mail_list, uint32_t const mail_elems,
      uint32_t const timeout_ms)
{
   uint32_t i;
   Singleton_System_Clk(&System_Clk_Singleton);
   Isnt_Nullptr(System_Clk_Singleton->vtbl->search_mailbox, NULL);
   Mail_T * mail = NULL;
   Mailbox_T * const mailbox = System_Clk_Singleton->vtbl->search_mailbox(System_Clk_Singleton, System_Clk_self_task_id(), System_Clk_self_pid());

   Isnt_Nullptr(mail_list, NULL);

   for(i = 0; i < mail_elems; ++i)
   {
      mail = mailbox->vtbl->get_mail_by_mail_id(mailbox, mail_list[i]);
      if(NULL != mail)
      {
         mail->vtbl->dump(mail);
         break;
      }
   }
   return mail;
}

Mail_T * const System_Clk_retreive_mail(uint32_t const timeout_ms)
{
   Singleton_System_Clk(&System_Clk_Singleton);
   Mail_T * mail = NULL;
   Mailbox_T * mailbox = NULL;
   mailbox = System_Clk_Singleton->vtbl->search_mailbox(System_Clk_Singleton, System_Clk_self_task_id(), System_Clk_self_pid());

   mail = mailbox->vtbl->get_first_mail(mailbox);
   mail->vtbl->dump(mail);

   return mail;
}

void System_Clk_put_date_string(char * date_str)
{
   Singleton_System_Clk(&System_Clk_Singleton);

   Isnt_Nullptr(System_Clk_Singleton->vtbl->get_date_length, );
   Isnt_Nullptr(System_Clk_Singleton->vtbl->get_date, );
   Isnt_Nullptr(date_str, );

   if(strlen(date_str) >= System_Clk_Singleton->vtbl->get_date_length(System_Clk_Singleton) )
   {
      memcpy(date_str, System_Clk_Singleton->vtbl->get_date(System_Clk_Singleton), System_Clk_Singleton->vtbl->get_date_length(System_Clk_Singleton));
   }
}
/*=====================================================================================* 
 * system_clk.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
