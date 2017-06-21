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
static void IPC_Ctor(IPC_T * const this, uint32_t const max_tasks);
static Task_T * const IPC_search_task(Vector_Task_T * const this, IPC_Task_Id_T const task_id);
static Mailbox_T * const IPC_search_mailbox(Vector_Mailbox_T * const this, IPC_Task_Id_T const task_id);
static bool_t IPC_Is_This_Task(Task_T * const task, uint32_t const tid);
static bool_t IPC_Is_This_Mailbox(Mailbox_T * const mbx, uint32_t const tid);
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
void IPC_init(void)
{
   printf("%s \n", __FUNCTION__);

   IPC_Obj.task_stack= Vector_Task();
   IPC_Obj.mbx_stack = Vector_Mailbox();
   IPC_Obj.max_tasks = 0;

   IPC_Vtbl.Object.rtti = &IPC_Rtti;
   IPC_Vtbl.Object.destroy = IPC_Dtor;
   IPC_Vtbl.ctor = IPC_Ctor;
   IPC_Vtbl.get_tid = NULL;
   IPC_Vtbl.get_pid = NULL;
   IPC_Vtbl.set_mailbox = NULL;
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

Task_T * const IPC_search_task(Vector_Task_T * const this, IPC_Task_Id_T const task_id)
{
   Task_T * const it = NULL;
   Isnt_Nullptr(this, NULL);
   /*Vector_Task_find_if(this.vtbl->front(&this->task_stack),
         this->task_stack.vtbl->back(&this->task_stack), task_id, IPC_Is_This_Task); */
   return it;
}

Mailbox_T * const IPC_search_mailbox(Vector_Mailbox_T * const this, IPC_Task_Id_T const task_id)
{
   Mailbox_T * const it = NULL;
   Isnt_Nullptr(this, NULL);
   /* FIXME Vector_Mailbox_find_if(this->mbx_task.vtbl->front(&this->mbx_task),
         this->mbx_task.vtbl->back(&this->mbx_task), task_id, IPC_Is_This_Mailbox); */
   return it;
}
/*=====================================================================================*
 * Exported Function Definitions
 *=====================================================================================*/
void IPC_Ctor(IPC_T * const this, uint32_t const max_tasks)
{
   this->max_tasks = max_tasks;
}



IPC_Task_Id_T IPC_self_task_id(void)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, 0);

   return this->vtbl->get_tid(this);
}

void IPC_task_ready(void)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, );
   this->vtbl->notify_ready(this, IPC_self_task_id());
}

int IPC_wait(void)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this->vtbl->wait, -1);

   return this->vtbl->wait(this);
}

void IPC_create_mailbox(uint32_t const max_mails, size_t const mail_size)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, );

   this->vtbl->set_mailbox(this, max_mails, mail_size);
}

void IPC_destroy_mailbox(void)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, );

   Mailbox_T * const mailbox = IPC_search_mailbox(&this->mbx_stack, IPC_self_task_id());
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

   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, false);

   mailbox = IPC_search_mailbox(&this->mbx_stack, this->vtbl->get_tid(this));
   Isnt_Nullptr(mailbox, false);

   for(i = 0; i < mail_elems && success; ++i)
   {
      success = Publisher_subscribe(mailbox, mail_list[i]);
   }

   return success;
}

bool_t IPC_unsubscribe_mail_list(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems)
{
   bool_t success = true;
   Mailbox_T * mailbox = NULL;
   uint32_t i;
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, false);

   for(i = 0; i < mail_elems && success; ++i)
   {
      mailbox = IPC_search_mailbox(&this->mbx_stack, this->vtbl->get_tid(this));
      success = Publisher_unsubscribe(mailbox,  mail_list[i]);
   }
   return success;
}

void IPC_send(IPC_Task_Id_T const receiver_task, IPC_Mail_Id_T mail_id,
		void const * data, size_t const data_size)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, );

   Mailbox_T * const mailbox = IPC_search_mailbox(&this->mbx_stack, receiver_task);
   Isnt_Nullptr(this->vtbl->get_pid,);
   Mail_T mail = Mail();

   mail.vtbl->ctor(&mail, mail_id, IPC_self_task_id(), receiver_task, data, data_size);
   mailbox->vtbl->push_mail(mailbox, &mail);

   mail.vtbl->Object.destroy((Object_T *)&mail);
}

void IPC_publish(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size)
{
   Mail_T mail = Mail();
   mail.vtbl->ctor(&mail, mail_id, IPC_self_task_id(), 0, data, data_size);
   Publisher_publish_mail(&mail);
   mail.vtbl->Object.destroy((Object_T *)&mail);
}

void IPC_broadcast(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size)
{
   IPC_Task_Id_T task = 0;
   IPC_Process_Id_T process = 0;
   Mailbox_T * mailbox = NULL;
   Mail_T mail = Mail();
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, );

   for(process = 0 ; process < IPC_MAX_PROCESSES; ++process)
   {
      for(task = 0 ; task < IPC_MAX_TASKS; ++task)
      {
         mailbox = IPC_search_mailbox(&this->mbx_stack, task);
         if(NULL != mailbox)
         {
            mail.vtbl->ctor(&mail, mail_id, IPC_self_task_id(), task, data, data_size);
            mailbox->vtbl->push_mail(mailbox, &mail);
         }
      }
   }
   mail.vtbl->Object.destroy((Object_T *)&mail);
}

Mail_T * const IPC_retreive_from_mail_list(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems,
      uint32_t const timeout_ms)
{
   uint32_t i;
   IPC_T * this = NULL;
   Mail_T * mail = NULL;
   Isnt_Nullptr(mail_list, NULL);
   IPC_get_instance(&this);
   Isnt_Nullptr(this, NULL);

   Mailbox_T * const mailbox = IPC_search_mailbox(&this->mbx_stack, this->vtbl->get_tid(this));
   Isnt_Nullptr(mailbox, NULL);

   IPC_Timestamp_T tout_timestamp = IPC_timestamp() + timeout_ms;
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
   }while(mail != NULL && IPC_time_elapsed(tout_timestamp));
   return mail;
}

Mail_T * const IPC_retreive_mail(uint32_t const timeout_ms)
{
   Mail_T * mail = NULL;
   Mailbox_T * mailbox = NULL;
   IPC_T * this = NULL;
   IPC_Timestamp_T tout_timestamp = IPC_timestamp() + timeout_ms;

   IPC_get_instance(&this);
   Isnt_Nullptr(this, NULL);

   mailbox = IPC_search_mailbox(&this->mbx_stack, this->vtbl->get_tid(this));


   do
   {
      mail = mailbox->vtbl->get_first_mail(mailbox);
   }while(mail != NULL && IPC_time_elapsed(tout_timestamp));

   return mail;
}


uint32_t IPC_timestamp(void)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, 0);

   return this->vtbl->timestamp(this);
}

bool_t IPC_time_elapsed(uint32_t const timestamp)
{
   return timestamp > IPC_timestamp();
}

void IPC_put_date_string(char * date_str)
{
   IPC_T * this = NULL;
   Isnt_Nullptr(date_str, );

   IPC_get_instance(&this);
   Isnt_Nullptr(this, );

   if(strlen(date_str) >= this->vtbl->get_date_length(this) )
   {
      memcpy(date_str, this->vtbl->get_date(this), this->vtbl->get_date_length(this));
   }
}
/*=====================================================================================* 
 * ipc.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
