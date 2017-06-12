/*=====================================================================================*/
/**
 * worker.cpp
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
#include "ipc.h"
#include "worker.h"
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
static void Worker_ctor(Worker_T * const this, IPC_Task_Id_T const tid, uint32_t const mailbox_size);
static void Worker_run(Task_T * const super);
static bool_t Worker_is_alive(Worker_T * const this);
/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/

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
void Worker_init(void)
{
   printf("%s \n", __FUNCTION__);
   Worker_Obj.Task = Task();

   memcpy(&Worker_Vtbl.Task, Worker_Obj.Task.vtbl, sizeof(Worker_Vtbl.Task));
   Worker_Vtbl.Task.Object.rtti = &Worker_Rtti;
   Worker_Vtbl.Task.Object.destroy = Worker_Dtor;
   Worker_Vtbl.Task.run = Worker_run;
   Worker_Vtbl.ctor = Worker_ctor;

   Worker_Obj.vtbl = &Worker_Vtbl;

   Object_update_info(&Worker_Obj.Task.Object, Task().rtti->rtti);
}
void Worker_shut(void) {}

void Worker_Dtor(Object_T * const obj)
{
}

bool_t Worker_is_alive(Worker_T * const this)
{
   IPC_Mail_Id_T mailist[] = {WORKER_SHUTDOWN};
   Mail_T * const is_alive = IPC_retreive_from_mail_list(mailist, sizeof(mailist), 50);
   return NULL == is_alive;
}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void Worker_ctor(Worker_T * const this, IPC_Task_Id_T const tid, uint32_t const mailbox_size)
{
   this->Task.vtbl->ctor(&this->Task, tid);
   this->mailbox_size = mailbox_size;
}

void Worker_run(Task_T * const super)
{
   Worker_T * const this = _dynamic_cast(Worker, super);

   IPC_create_mailbox(this->mailbox_size, 80);

   this->vtbl->on_start(this);

   IPC_notify_ready();

   while(Worker_is_alive(this))
   {
      this->vtbl->on_loop(this);
   }

   this->vtbl->on_stop(this);

   IPC_destroy_mailbox();
}
/*=====================================================================================* 
 * worker.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
