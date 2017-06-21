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
#define CLASS_IMPLEMENTATION
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "ipc_set.h"
#include "ipc.h"
#include "ipc_ext.h"
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
static void IPC_Ctor(IPC_T * const this, IPC_Process_Id_T const pid, uint32_t const max_tasks);
static Task_T * const IPC_search_task(IPC_T * const this, IPC_Task_Id_T const task_id);
static Mailbox_T * const IPC_search_mailbox(IPC_T * const this, IPC_Task_Id_T const task_id, IPC_Process_Id_T const pid);
static bool_t IPC_Is_This_Task(Task_T * const task, uint32_t const tid);
/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEFINITION
static IPC_T * IPC_Singleton = NULL;

/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/
void IPC_init(void)
{
   printf("%s \n", __FUNCTION__);

   IPC_Obj.pid= 0;
   IPC_Obj.task_stack= Vector_Task();
   IPC_Obj.mbx_stack = Vector_Mailbox();
   IPC_Obj.max_tasks = 0;

   IPC_Vtbl.Object.rtti = &IPC_Rtti;
   IPC_Vtbl.Object.destroy = IPC_Dtor;
   IPC_Vtbl.ctor = IPC_Ctor;
   IPC_Vtbl.get_tid = NULL;
   IPC_Vtbl.get_pid = NULL;
   IPC_Vtbl.search_task = IPC_search_task;
   IPC_Vtbl.set_mailbox = NULL;
   IPC_Vtbl.search_mailbox = IPC_search_mailbox;
   IPC_Vtbl.notify_ready = NULL;
   IPC_Vtbl.get_date_length = NULL;
   IPC_Vtbl.get_date = NULL;
}

void IPC_shut(void) {}

void IPC_Dtor(Object_T * const obj)
{
   IPC_T * const this = _dynamic_cast(IPC, obj);
   Isnt_Nullptr(this,);

}

bool_t IPC_Is_This_Task(Task_T * const task, uint32_t const tid)
{
   Isnt_Nullptr(task, false);
   return tid == task->tid;
}
bool_t IPC_Is_This_Mailbox(Mailbox_T * const mbx, uint32_t const tid)
{
   Isnt_Nullptr(mbx, false);
   return tid == mbx->owner;
}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void IPC_Ctor(IPC_T * const this, IPC_Process_Id_T const pid, uint32_t const max_tasks)
{
   this->pid = pid;
   this->max_tasks = max_tasks;
}

Task_T * const IPC_search_task(IPC_T * const this, IPC_Task_Id_T const task_id)
{
   Task_T * const it = NULL; /*Vector_Task_find_if(this->task_stack.vtbl->front(&this->task_stack),
         this->task_stack.vtbl->back(&this->task_stack), task_id, IPC_Is_This_Task); */
   return it;
}

Mailbox_T * const IPC_search_mailbox(IPC_T * const this, IPC_Task_Id_T const task_id, IPC_Process_Id_T const pid)
{
   Mailbox_T * const it = NULL; /* FIXME Vector_Mailbox_find_if(this->mbx_task.vtbl->front(&this->mbx_task),
         this->mbx_task.vtbl->back(&this->mbx_task), task_id, IPC_Is_This_Mailbox); */
   return it;
}

IPC_Task_Id_T IPC_self_task_id(void)
{
   IPC_get_instance(&IPC_Singleton);
   Isnt_Nullptr(IPC_Singleton, 0);
   return IPC_Singleton->vtbl->get_tid(IPC_Singleton);
}

void IPC_task_ready(void)
{
   IPC_get_instance(&IPC_Singleton);
   Isnt_Nullptr(IPC_Singleton, );
   IPC_Singleton->vtbl->notify_ready(IPC_Singleton, IPC_self_task_id());
}

int IPC_wait(void)
{
   IPC_get_instance(&IPC_Singleton);
   Isnt_Nullptr(IPC_Singleton->vtbl->wait, -1);
   return IPC_Singleton->vtbl->wait(IPC_Singleton);
}

void IPC_create_mailbox(uint32_t const max_mails, size_t const mail_size)
{
   IPC_get_instance(&IPC_Singleton);
   Isnt_Nullptr(IPC_Singleton->vtbl->set_mailbox, );
   IPC_Singleton->vtbl->set_mailbox(IPC_Singleton, max_mails, mail_size);
}

void IPC_destroy_mailbox(void)
{
   IPC_get_instance(&IPC_Singleton);
   Isnt_Nullptr(IPC_Singleton->vtbl->set_mailbox, );
   Mailbox_T * const mailbox = IPC_Singleton->vtbl->search_mailbox(IPC_Singleton,
         IPC_self_task_id(), IPC_self_pid());
   Isnt_Nullptr(mailbox, );
   /*Vector_Task_remove_if(this->task_stack.vtbl->front(&this->task_stack),
            this->task_stack.vtbl->back(&this->task_stack), task_id, IPC_Is_This_Task); */
}
/**
 * TB Mail Comm
 */
bool_t IPC_subscribe_mail_list(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems)
{
   bool_t success = true;
   Mailbox_T * mailbox = NULL;
   uint32_t i;
   for(i = 0; i < mail_elems && success; ++i)
   {
      mailbox = IPC_Singleton->vtbl->search_mailbox(IPC_Singleton, IPC_self_task_id(), IPC_self_pid());
      success = Publisher_subscribe(mailbox, mail_list[i]);
   }
   return success;
}

bool_t IPC_unsubscribe_mail_list(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems)
{

   bool_t success = true;
   Mailbox_T * mailbox = NULL;
   uint32_t i;
   for(i = 0; i < mail_elems && success; ++i)
   {
      mailbox = IPC_Singleton->vtbl->search_mailbox(IPC_Singleton, IPC_self_task_id(), IPC_self_pid());
      success = Publisher_unsubscribe(mailbox,  mail_list[i]);
   }
   return success;
}

void IPC_send(IPC_Task_Id_T const receiver_task, IPC_Process_Id_T const receiver_pid,
      IPC_Mail_Id_T mail_id, void const * data, size_t const data_size)
{
   IPC_get_instance(&IPC_Singleton);
   Mailbox_T * mailbox = IPC_Singleton->vtbl->search_mailbox(IPC_Singleton, receiver_task, receiver_pid);
   Isnt_Nullptr(IPC_Singleton->vtbl->get_pid,);
   Mail_T * mail = Mail_new();
   mail->vtbl->ctor(mail, mail_id, IPC_self_task_id(), receiver_task, data, data_size);
   mailbox->vtbl->push_mail(mailbox, mail);
}

void IPC_publish(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size)
{
   Mail_T * mail = Mail_new();
   mail->vtbl->ctor(mail, mail_id, IPC_self_task_id(), 0, data, data_size);
   Publisher_publish_mail(mail);
}

void IPC_broadcast(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size)
{
   IPC_get_instance(&IPC_Singleton);
   Isnt_Nullptr(IPC_Singleton->vtbl->search_mailbox, );
   Isnt_Nullptr(IPC_Singleton->vtbl->get_pid, );
   IPC_Task_Id_T task = 0;
   IPC_Process_Id_T process = 0;
   Mailbox_T * mailbox = NULL;
   Mail_T * mail = NULL;

   for(process = 0 ; process < IPC_MAX_PROCESSES; ++process)
   {
      for(task = 0 ; task < IPC_MAX_TASKS; ++task)
      {
         mailbox = IPC_Singleton->vtbl->search_mailbox(IPC_Singleton, task, process);
         if(NULL != mailbox)
         {
            mail = Mail_new();
            mail->vtbl->ctor(mail, mail_id, IPC_self_task_id(), task, data, data_size);
            mailbox->vtbl->push_mail(mailbox, mail);
            _delete(mail);
         }
      }
   }
}

Mail_T * const IPC_retreive_from_mail_list(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems,
      uint32_t const timeout_ms)
{
   uint32_t i;
   IPC_get_instance(&IPC_Singleton);
   Isnt_Nullptr(IPC_Singleton, NULL);

   Mail_T * mail = NULL;
   Mailbox_T * const mailbox = IPC_Singleton->vtbl->search_mailbox(IPC_Singleton, IPC_self_task_id(), IPC_self_pid());

   Isnt_Nullptr(mail_list, NULL);
   IPC_Linux_Timestamp_T tout_timestamp = IPC_timestamp() + timeout_ms;
   do
   {
      for(i = 0; i < mail_elems; ++i)
      {
         mail = mailbox->vtbl->get_mail_by_mail_id(mailbox, mail_list[i]);
         if(NULL != mail)
         {
            break;
         }
      }
   }while(mail != NULL && IPC_time_elapse(tout_timestamp));
   return mail;
}

Mail_T * const IPC_retreive_mail(uint32_t const timeout_ms)
{
   IPC_get_instance(&IPC_Singleton);
   Mail_T * mail = NULL;
   Mailbox_T * mailbox = NULL;

   Isnt_Nullptr(IPC_Singleton, NULL);

   mailbox = IPC_Singleton->vtbl->search_mailbox(IPC_Singleton, IPC_self_task_id(), IPC_self_pid());

   IPC_Linux_Timestamp_T tout_timestamp = IPC_timestamp() + timeout_ms;

   do
   {
      mail = mailbox->vtbl->get_first_mail(mailbox);
   }while(mail != NULL && IPC_time_elapse(tout_timestamp));

   return mail;
}


uint32_t IPC_timestamp(void)
{
   IPC_get_instance(&IPC_Singleton);
   Isnt_Nullptr(IPC_Singleton, NULL);
   return IPC_Singleton->vtbl->timestamp(IPC_Singleton);
}

bool_t IPC_time_elapse(uint32_t const timestamp)
{
   return timestamp > IPC_timestamp();
}

void IPC_put_date_string(char * date_str)
{
   IPC_get_instance(&IPC_Singleton);

   Isnt_Nullptr(IPC_Singleton->vtbl->get_date_length, );
   Isnt_Nullptr(IPC_Singleton->vtbl->get_date, );
   Isnt_Nullptr(date_str, );

   if(strlen(date_str) >= IPC_Singleton->vtbl->get_date_length(IPC_Singleton) )
   {
      memcpy(date_str, IPC_Singleton->vtbl->get_date(IPC_Singleton), IPC_Singleton->vtbl->get_date_length(IPC_Singleton));
   }
}
/*=====================================================================================* 
 * ipc.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
