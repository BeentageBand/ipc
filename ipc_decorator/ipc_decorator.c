/*=====================================================================================*/
/**
 * ipc_decorator.c
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
#include "ipc_decorator.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Local X-Macros
 *=====================================================================================*/
#undef CLASS_VIRTUAL_METHODS
#define CLASS_VIRTUAL_METHODS(_ovr_method) \
   _ovr_method(IPC,get_tid) \
   _ovr_method(IPC,run_task) \
   _ovr_method(IPC,task_ready) \
   _ovr_method(IPC,wait_task) \
   _ovr_method(IPC,sleep) \
   _ovr_method(IPC,set_mailbox) \
   _ovr_method(IPC,timestamp) \
   _ovr_method(IPC,get_date_length) \
   _ovr_method(IPC,get_date) \

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
static void IPC_Decorator_Ctor(IPC_Decorator_T * const this, IPC_T * const ipc);
static IPC_Task_Id_T IPC_Decorator_get_tid(IPC_T * const super);
static int IPC_Decorator_run_task(IPC_T * const super, Task_T * const task);
static void IPC_Decorator_set_mailbox(IPC_T * const super, uint32_t const mail_elems, uint32_t const mail_size);
static void IPC_Decorator_task_ready(IPC_T * const super);
static int IPC_Decorator_wait_task(IPC_T * const super, Task_T * const task);
static void IPC_Decorator_sleep(IPC_T * const super, uint32_t const ms);
static uint32_t IPC_Decorator_timestamp(IPC_T * const super);
static size_t IPC_Decorator_get_date_length(IPC_T * const super);
static char const * IPC_Decorator_get_date(IPC_T * const super);
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
void IPC_Decorator_init(void)
{
   CHILD_CLASS_INITIALIZATION

   IPC_Decorator_Obj.ipc = NULL;
   IPC_Decorator_Vtbl.ctor = IPC_Decorator_Ctor;

}
void IPC_Decorator_shut(void) {}

void IPC_Decorator_Dtor(Object_T * const obj)
{
}
/*=====================================================================================*
 * Exported Function Definitions
 *=====================================================================================*/
void IPC_Decorator_Ctor(IPC_Decorator_T * const this, IPC_T * const ipc)
{
   this->ipc = ipc;
}

IPC_Task_Id_T IPC_Decorator_get_tid(IPC_T * const super)
{
   IPC_Decorator_T * const this = _dynamic_cast(IPC_Decorator, super);
   Isnt_Nullptr(this, 0);
   Isnt_Nullptr(this->ipc, 0);
   return this->ipc->vtbl->get_tid(this->ipc);
}

int IPC_Decorator_run_task(IPC_T * const super, Task_T * const task)
{
   IPC_Decorator_T * const this = _dynamic_cast(IPC_Decorator, super);
      Isnt_Nullptr(this, -1);
      Isnt_Nullptr(this->ipc, 0);
      return this->ipc->vtbl->run_task(this->ipc, task);
}

void IPC_Decorator_set_mailbox(IPC_T * const super, IPC_Task_Id_T const tid, uint32_t const mail_elems)
{
   IPC_Decorator_T * const this = _dynamic_cast(IPC_Decorator, super);
   Isnt_Nullptr(this, );
   Isnt_Nullptr(this->ipc, );
   this->ipc->vtbl->set_mailbox(this->ipc, tid, mail_elems);
}

void IPC_Decorator_task_ready(IPC_T * const super)
{
   IPC_Decorator_T * const this = _dynamic_cast(IPC_Decorator, super);
   Isnt_Nullptr(this, );
   Isnt_Nullptr(this->ipc, );
   this->ipc->vtbl->task_ready(this->ipc);
}

int IPC_Decorator_wait_task(IPC_T * const super, Task_T * const task)
{
   IPC_Decorator_T * const this = _dynamic_cast(IPC_Decorator, super);
   Isnt_Nullptr(this, -1);
   Isnt_Nullptr(this->ipc, -1);
   return this->ipc->vtbl->wait_task(this->ipc, task);
}

void IPC_Decorator_sleep(IPC_T * const super, uint32_t const ms)
{
   IPC_Decorator_T * const this = _dynamic_cast(IPC_Decorator, super);
   Isnt_Nullptr(this, );
   Isnt_Nullptr(this->ipc, );
   return this->ipc->vtbl->sleep(this->ipc, ms);
}

uint32_t IPC_Decorator_timestamp(IPC_T * const super)
{
   IPC_Decorator_T * const this = _dynamic_cast(IPC_Decorator, super);
   Isnt_Nullptr(this, 0);
   Isnt_Nullptr(this->ipc, 0);
   return this->ipc->vtbl->timestamp(this->ipc);
}

size_t IPC_Decorator_get_date_length(IPC_T * const super)
{
   IPC_Decorator_T * const this = _dynamic_cast(IPC_Decorator, super);
   Isnt_Nullptr(this, 0);
   Isnt_Nullptr(this->ipc, 0);
   return this->ipc->vtbl->get_date_length(this->ipc);
}

char const * IPC_Decorator_get_date(IPC_T * const super)
{
   IPC_Decorator_T * const this = _dynamic_cast(IPC_Decorator, super);
   Isnt_Nullptr(this, NULL);
   Isnt_Nullptr(this->ipc, NULL);
   return this->ipc->vtbl->get_date(this->ipc);
}

/*=====================================================================================* 
 * ipc_decorator.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
