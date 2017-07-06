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
#include "mailbox.h"
#include "task.h"
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
typedef struct
{
   IPC_Task_Id_T tid;
   Task_T * task;
}Task_Stack_T;

typedef struct
{
   IPC_Task_Id_T tid;
   Mailbox_T * mbx;
}Mailbox_Stack_T;
/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static IPC_Task_Id_T IPC_get_tid(IPC_T * const this);
static int IPC_run_task(IPC_T * const this, Task_T * const task);
static void IPC_set_mailbox(IPC_T * const this, IPC_Task_Id_T const tid, uint32_t const mail_elems);
static void IPC_task_ready(IPC_T * const this);
static int IPC_wait_task(IPC_T * const this, Task_T * const task);
static bool_t IPC_register_task(IPC_T * const this, Task_T * const task);
static bool_t IPC_unregister_task(IPC_T * const this, Task_T * const task);
static void IPC_sleep(IPC_T * const this, uint32_t const ms);
static uint32_t IPC_timestamp(IPC_T * const this);
static size_t IPC_get_date_length(IPC_T * const this);
static char const * IPC_get_date(IPC_T * const this);

/** Private **/
static Task_T * const IPC_search_task(IPC_Task_Id_T const task_id);
static Mailbox_T * const IPC_search_mailbox(IPC_Task_Id_T const task_id);
static int IPC_Is_This_Task(void const * a,void const * b);
static int IPC_Is_This_Mailbox(void const * a,void const * b);
/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEFINITION

#undef TASK_ID
#define TASK_ID(pid, tid, desc) {tid##_WORKER, NULL},

static Task_Stack_T Task_Stack[] =
{
      {IPC_BEGIN_TASK_ID, NULL},
      IPC_WORKERS_IDS(TASK_ID)
};
static Mailbox_Stack_T Mbx_Stack[] =
{
      {IPC_BEGIN_TASK_ID, NULL},
      IPC_WORKERS_IDS(TASK_ID)
};
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
   qsort(Task_Stack,Num_Elems(Task_Stack), sizeof(*Task_Stack), IPC_Is_This_Task);
   qsort(Mbx_Stack,Num_Elems(Mbx_Stack), sizeof(*Mbx_Stack), IPC_Is_This_Mailbox);

   IPC_Vtbl.Object.rtti = &IPC_Rtti;
   IPC_Vtbl.Object.destroy = IPC_Dtor;
   IPC_Vtbl.get_tid = IPC_get_tid;
   IPC_Vtbl.run_task = IPC_run_task;
   IPC_Vtbl.task_ready = IPC_task_ready;
   IPC_Vtbl.wait_task = IPC_wait_task;
   IPC_Vtbl.register_task = IPC_register_task;
   IPC_Vtbl.unregister_task = IPC_unregister_task;
   IPC_Vtbl.sleep = IPC_sleep;
   IPC_Vtbl.set_mailbox = IPC_set_mailbox;
   IPC_Vtbl.timestamp = IPC_timestamp;
   IPC_Vtbl.get_date_length = IPC_get_date_length;
   IPC_Vtbl.get_date = IPC_get_date;
}

void IPC_shut(void) {}

void IPC_Dtor(Object_T * const obj)
{
   IPC_T * const this = _dynamic_cast(IPC, obj);
   Isnt_Nullptr(this,);

}

int IPC_Is_This_Task(void const * a,void const * b)
{
   IPC_Task_Id_T const * ta = (IPC_Task_Id_T const *) a;
   IPC_Task_Id_T const * tb = (IPC_Task_Id_T const *) b;
   return (*ta - *tb);
}

int IPC_Is_This_Mailbox(void const * a,void const * b)
{
   IPC_Task_Id_T const * ta = (IPC_Task_Id_T const *) a;
   IPC_Task_Id_T const * tb = (IPC_Task_Id_T const *) b;
   return (*ta - *tb);
}

Task_T * const IPC_search_task(IPC_Task_Id_T const task_id)
{
   Task_Stack_T * const it = (Task_Stack_T * const) bsearch(&task_id, Task_Stack,Num_Elems(Task_Stack),
         sizeof(*Task_Stack), IPC_Is_This_Task);
   Isnt_Nullptr(it, NULL);
   return it->task;
}

Mailbox_T * const IPC_search_mailbox(IPC_Task_Id_T const task_id)
{
   Mailbox_Stack_T * const it = (Mailbox_Stack_T * const)bsearch(&task_id, Mbx_Stack,Num_Elems(Mbx_Stack),
         sizeof(*Mbx_Stack), IPC_Is_This_Mailbox);
   Isnt_Nullptr(it, NULL);
   return it->mbx;
}
/*=====================================================================================*
 * Exported Function Definitions
 *=====================================================================================*/
void IPC_Ctor(IPC_T * const this)
{
}
/** Object Methods **/
IPC_Task_Id_T IPC_get_tid(IPC_T * const this)
{
   return IPC_TOTAL_TASK_IDS_ITEMS;
}

int IPC_run_task(IPC_T * const this, Task_T * const task)
{
   return -1;
};

void IPC_set_mailbox(IPC_T * const this, uint32_t const mail_elems, uint32_t const mail_size)
{}

void IPC_task_ready(IPC_T * const this)
{}

int IPC_wait_task(IPC_T * const this, Task_T * const task)
{
   return -1;
}


bool_t IPC_register_task(IPC_T * const this, Task_T * const task)
{
   Isnt_Nullptr(task, false);
   Task_Stack_T * const it = (Task_Stack_T * const) bsearch(&task->tid, Task_Stack,Num_Elems(Task_Stack),
          sizeof(*Task_Stack), IPC_Is_This_Task);
   it->task = task;
   return NULL != it->task;
}

bool_t IPC_unregister_task(IPC_T * const this, Task_T * const task)
{
   Isnt_Nullptr(task, false);
   Task_Stack_T * const it = (Task_Stack_T * const) bsearch(&task->tid, Task_Stack,Num_Elems(Task_Stack),
          sizeof(*Task_Stack), IPC_Is_This_Task);

   it->task = NULL;
   return NULL == it->task;
}

void IPC_sleep(IPC_T * const this, uint32_t const ms)
{}

uint32_t IPC_timestamp(IPC_T * const this)
{
   return 0;
}

size_t IPC_get_date_length(IPC_T * const this)
{
   return 0;
}

char const * IPC_get_date(IPC_T * const this)
{
   return NULL;
}

/** IPC APIs **/
IPC_Task_Id_T IPC_Self_Task_Id(void)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, 0);
   return this->vtbl->get_tid(this);
}

void IPC_Task_Ready(void)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, );
   this->vtbl->task_ready(this);
}

int IPC_Run(IPC_Task_Id_T const tid)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, -1);
   Task_T * const task = IPC_search_task(tid);
   Isnt_Nullptr(task, -1);
   return this->vtbl->run_task(this, task);
}

int IPC_Wait(IPC_Task_Id_T const tid)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, -1);
   Task_T * const task = IPC_search_task(tid);
   Isnt_Nullptr(task, -1);

   return this->vtbl->wait_task(this, task);
}

void IPC_Sleep(uint32_t const ms)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, );
   this->vtbl->sleep(this, ms);
}

void IPC_Create_Mailbox(uint32_t const max_mails, size_t const mail_size)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, );

   this->vtbl->set_mailbox(this, max_mails, mail_size);
}

void IPC_Destroy_Mailbox(void)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, );

   Mailbox_T * const mailbox = IPC_search_mailbox(this->vtbl->get_tid(this));
   Isnt_Nullptr(mailbox, );
   /*Vector_Task_remove_if(Task_Stack.vtbl->front(&Task_Stack),
            Task_Stack.vtbl->back(&Task_Stack), task_id, IPC_Is_This_Task); */
}
/**
 * TB Mail Comm
 */
bool_t IPC_Subscribe_Mail_List(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems)
{
   bool_t success = true;
   Mailbox_T * mailbox = NULL;
   uint32_t i;

   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, false);

   mailbox = IPC_search_mailbox(this->vtbl->get_tid(this));
   Isnt_Nullptr(mailbox, false);

   for(i = 0; i < mail_elems && success; ++i)
   {
      success = Publisher_subscribe(mailbox, mail_list[i]);
   }

   return success;
}

bool_t IPC_Unsubscribe_Mail_List(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems)
{
   bool_t success = true;
   Mailbox_T * mailbox = NULL;
   uint32_t i;
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, false);

   for(i = 0; i < mail_elems && success; ++i)
   {
      mailbox = IPC_search_mailbox(this->vtbl->get_tid(this));
      success = Publisher_unsubscribe(mailbox,  mail_list[i]);
   }
   return success;
}

void IPC_Send(IPC_Task_Id_T const receiver_task, IPC_Mail_Id_T mail_id,
		void const * data, size_t const data_size)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, );

   Mailbox_T * const mailbox = IPC_search_mailbox(receiver_task);
   Mail_T mail = Mail();

   mail.vtbl->ctor(&mail, mail_id, IPC_Self_Task_Id(), receiver_task, data, data_size);
   mailbox->vtbl->push_mail(mailbox, &mail);

   mail.vtbl->Object.destroy((Object_T *)&mail);
}

void IPC_Publish(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size)
{
   Mail_T mail = Mail();
   mail.vtbl->ctor(&mail, mail_id, IPC_Self_Task_Id(), 0, data, data_size);
   Publisher_publish_mail(&mail);
   mail.vtbl->Object.destroy((Object_T *)&mail);
}

void IPC_Broadcast(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size)
{
   IPC_Task_Id_T task = 0;
   Mailbox_T * mailbox = NULL;
   Mail_T mail = Mail();
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, );

   for(task = 0 ; task < IPC_MAX_TASKS; ++task)
   {
      mailbox = IPC_search_mailbox(task);
      if(NULL != mailbox)
      {
         mail.vtbl->ctor(&mail, mail_id, IPC_Self_Task_Id(), task, data, data_size);
         mailbox->vtbl->push_mail(mailbox, &mail);
      }
   }
   mail.vtbl->Object.destroy((Object_T *)&mail);
}

Mail_T const * IPC_Retreive_From_Mail_List(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems,
      uint32_t const timeout_ms)
{
   uint32_t i;
   IPC_T * this = NULL;
   Mail_T const * mail = NULL;
   Isnt_Nullptr(mail_list, NULL);
   IPC_get_instance(&this);
   Isnt_Nullptr(this, NULL);

   Mailbox_T * const mailbox = IPC_search_mailbox(this->vtbl->get_tid(this));
   printf("mailbox %s\n", (mailbox)? "found" : "NULL");
   Isnt_Nullptr(mailbox, NULL);

   IPC_Timestamp_T tout_timestamp = this->vtbl->timestamp(this) + timeout_ms;
   printf("IPC_Retreive_From_Mail_List ms = %d\n", tout_timestamp);
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
   }while(mail != NULL && !IPC_Time_Elapsed(tout_timestamp));
   return mail;
}

Mail_T const * IPC_Retreive_Mail(uint32_t const timeout_ms)
{
   Mail_T const * mail = NULL;
   IPC_T * this = NULL;

   IPC_get_instance(&this);
   Isnt_Nullptr(this, NULL);
   IPC_Timestamp_T tout_timestamp = this->vtbl->timestamp(this) + timeout_ms;

   Mailbox_T * const mailbox = IPC_search_mailbox(this->vtbl->get_tid(this));
   printf("mailbox %s\n", (mailbox)? "found" : "NULL");
   Isnt_Nullptr(mailbox, NULL);

   do
   {
      mail = mailbox->vtbl->pop_mail(mailbox);
   }while(mail != NULL && !IPC_Time_Elapsed(tout_timestamp));

   return mail;
}


uint32_t IPC_Timestamp(void)
{
   IPC_T * this = NULL;
   IPC_get_instance(&this);
   Isnt_Nullptr(this, 0);

   return this->vtbl->timestamp(this);
}

bool_t IPC_Time_Elapsed(uint32_t const timestamp)
{
   return timestamp < IPC_Timestamp();
}

void IPC_Put_Date_String(char * date_str)
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
