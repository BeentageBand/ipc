/*=====================================================================================*/
/**
 * ipc_linux_task.c
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
#include "ipc_linux_task.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/
#include <pthread.h>
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
 * Local Function Prototypes
 *=====================================================================================*/
static void IPC_Linux_Task_Ctor(IPC_Linux_Task_T * const this, IPC_Process_Id_T const pid, uint32_t const max_tasks,
      IPC_T * const ipc);
static IPC_Task_Id_T IPC_Linux_Task_get_tid(IPC_T * const super);
static IPC_Process_Id_T IPC_Linux_Task_get_pid(IPC_T * const super);
static void IPC_Linux_Task_set_mailbox(IPC_T * const super, uint32_t const tid, uint32_t const max_size);
static void IPC_Linux_Task_notify_ready(IPC_T * const super, IPC_Task_Id_T const);
static size_t IPC_Linux_Task_get_date_length(IPC_T * const super);
static char const * IPC_Linux_Task_get_date(IPC_T * const super);
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
void IPC_Linux_Task_init(void)
{
   printf("%s \n", __FUNCTION__);
   IPC_Linux_Task_Obj.IPC_Decorator = IPC_Decorator();

   memcpy(&IPC_Linux_Task_Vtbl.IPC_Decorator, IPC_Linux_Task_Obj.IPC_Decorator.vtbl,
         sizeof(IPC_Linux_Task_Vtbl.IPC_Decorator));
   IPC_Linux_Task_Vtbl.IPC_Decorator.IPC.Object.rtti = &IPC_Linux_Task_Rtti;
   IPC_Linux_Task_Vtbl.IPC_Decorator.IPC.Object.destroy = IPC_Linux_Task_Dtor;
   IPC_Linux_Task_Vtbl.IPC_Decorator.IPC.get_tid = IPC_Linux_Task_get_tid;
   IPC_Linux_Task_Vtbl.IPC_Decorator.IPC.get_pid = IPC_Linux_Task_get_pid;
   IPC_Linux_Task_Vtbl.IPC_Decorator.IPC.set_mailbox = IPC_Linux_Task_set_mailbox;
   IPC_Linux_Task_Vtbl.IPC_Decorator.IPC.notify_ready = IPC_Linux_Task_notify_ready;
   IPC_Linux_Task_Vtbl.IPC_Decorator.IPC.get_date_length = IPC_Linux_Task_get_date_length;
   IPC_Linux_Task_Vtbl.IPC_Decorator.IPC.get_date = IPC_Linux_Task_get_date;
   IPC_Linux_Task_Vtbl.ctor = IPC_Linux_Task_Ctor;

}
void IPC_Linux_Task_shut(void) {}

void IPC_Linux_Task_Dtor(Object_T * const obj)
{
}
/*=====================================================================================*
 * Exported Function Definitions
 *=====================================================================================*/
void IPC_Linux_Task_Ctor(IPC_Linux_Task_T * const this, IPC_Process_Id_T const pid, uint32_t const max_tasks, IPC_T * const ipc)
{
   this->IPC_Decorator.vtbl->ctor(&this->IPC_Decorator, pid, max_tasks, ipc);
}

IPC_Task_Id_T IPC_Linux_Task_get_tid(IPC_T * const super)
{
   pthread_self();
   return IPC_TOTAL_TASK_IDS_ITEMS;
}

IPC_Process_Id_T IPC_Linux_Task_get_pid(IPC_T * const super)
{
   return IPC_TOTAL_PROCESS_IDS_ITEMS;
}

void IPC_Linux_Task_set_mailbox(IPC_T * const super, IPC_Task_Id_T const tid, uint32_t const mail_elements)
{
}

void IPC_Linux_Task_notify_ready(IPC_T * const super, IPC_Task_Id_T const task_id)
{
}

size_t IPC_Linux_Task_get_date_length(IPC_T * const super)
{
   return 0;
}

char const * IPC_Linux_Task_get_date(IPC_T * const super)
{
   return NULL;
}
/*=====================================================================================* 
 * ipc_linux_task.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
