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
#undef CLASS_VIRTUAL_METHODS
#define CLASS_VIRTUAL_METHODS(_ovr) \
   _ovr(Task,run)
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
   CHILD_CLASS_INITIALIZATION
   Worker_Vtbl.ctor = Worker_ctor;
}

void Worker_shut(void) {}

void Worker_Dtor(Object_T * const obj)
{
}

bool_t Worker_is_alive(Worker_T * const this)
{
   IPC_Mail_Id_T mailist[] = {WORKER_SHUTDOWN};
   Mail_T * const is_alive = IPC_Retreive_From_Mail_List(mailist, sizeof(mailist), 50);
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

   IPC_Create_Mailbox(this->mailbox_size, 80);

   IPC_Task_Ready();

   this->vtbl->on_start(this);

   while(Worker_is_alive(this))
   {
      this->vtbl->on_loop(this);
   }

   printf("shutdown %d\n", super->tid);

   this->vtbl->on_stop(this);

   IPC_Destroy_Mailbox();
}
/*=====================================================================================* 
 * worker.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
