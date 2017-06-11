/*=====================================================================================*/
/**
 * ipc_linux_timestamp.c
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
#include "ipc_linux_timestamp.h"
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
 * Local Function Prototypes
 *=====================================================================================*/
static void IPC_Linux_Timestamp_Ctor(IPC_Linux_Timestamp_T * const this, IPC_Process_Id_T const pid, uint32_t const max_tasks,
      IPC_T * const ipc);
static IPC_Task_Id_T IPC_Linux_Timestamp_get_tid(IPC_T * const super);
static IPC_Process_Id_T IPC_Linux_Timestamp_get_pid(IPC_T * const super);
static void IPC_Linux_Timestamp_set_mailbox(IPC_T * const super, IPC_Task_Id_T const tid, uint32_t const mailbox_size);
static void IPC_Linux_Timestamp_notify_ready(IPC_T * const super, IPC_Task_Id_T const tid);
static size_t IPC_Linux_Timestamp_get_date_length(IPC_T * const super);
static char const * IPC_Linux_Timestamp_get_date(IPC_T * const super);
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
void IPC_Linux_Timestamp_init(void)
{
   printf("%s \n", __FUNCTION__);
   IPC_Linux_Timestamp_Obj.IPC_Decorator = IPC_Decorator();

   memcpy(&IPC_Linux_Timestamp_Vtbl.IPC_Decorator, IPC_Linux_Timestamp_Obj.IPC_Decorator.vtbl,
         sizeof(IPC_Linux_Timestamp_Vtbl.IPC_Decorator));
   IPC_Linux_Timestamp_Vtbl.IPC_Decorator.IPC.Object.rtti = &IPC_Linux_Timestamp_Rtti;
   IPC_Linux_Timestamp_Vtbl.IPC_Decorator.IPC.Object.destroy = IPC_Linux_Timestamp_Dtor;
   IPC_Linux_Timestamp_Vtbl.IPC_Decorator.IPC.get_tid = NULL;
   IPC_Linux_Timestamp_Vtbl.IPC_Decorator.IPC.get_pid = NULL;
   IPC_Linux_Timestamp_Vtbl.IPC_Decorator.IPC.set_mailbox = NULL;
   IPC_Linux_Timestamp_Vtbl.IPC_Decorator.IPC.notify_ready = NULL;
   IPC_Linux_Timestamp_Vtbl.IPC_Decorator.IPC.get_date_length = NULL;
   IPC_Linux_Timestamp_Vtbl.IPC_Decorator.IPC.get_date = NULL;
   IPC_Linux_Timestamp_Vtbl.ctor = IPC_Linux_Timestamp_Ctor;

}
void IPC_Linux_Timestamp_shut(void) {}

void IPC_Linux_Timestamp_Dtor(Object_T * const obj)
{
}
/*=====================================================================================*
 * Exported Function Definitions
 *=====================================================================================*/
void IPC_Linux_Timestamp_Ctor(IPC_Linux_Timestamp_T * const this, IPC_Process_Id_T const pid, uint32_t const max_tasks, IPC_T * const ipc)
{
   this->IPC_Decorator.vtbl->ctor(&this->IPC_Decorator, pid, max_tasks, ipc);
}

IPC_Task_Id_T IPC_Linux_Timestamp_get_tid(IPC_T * const super)
{
   return IPC_TOTAL_TASK_IDS_ITEMS;
}

IPC_Process_Id_T IPC_Linux_Timestamp_get_pid(IPC_T * const super)
{
   return IPC_TOTAL_PROCESS_IDS_ITEMS;
}

void IPC_Linux_Timestamp_set_mailbox(IPC_T * const super, IPC_Task_Id_T const tid, uint32_t const mailbox_size)
{

}

void IPC_Linux_Timestamp_notify_ready(IPC_T * const super, IPC_Task_Id_T const task_id)
{

}

size_t IPC_Linux_Timestamp_get_date_length(IPC_T * const super)
{
   return 0;
}

char const * IPC_Linux_Timestamp_get_date(IPC_T * const super)
{
   return NULL;
}
/*=====================================================================================* 
 * ipc_linux_timestamp.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
