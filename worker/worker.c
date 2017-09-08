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
#undef Dbg_FID
#define Dbg_FID Dbg_FID_Def(IPC_FID,0)
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "dbg_log.h"
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

/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static void Worker_runnable(union Task * const super);
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
void Worker_Init(void)
{
   Worker_Class.runnable = Worker_runnable;
}


void Worker_Delete(Object_T * const obj)
{
}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
union Worker Worker_Tid(IPC_Task_Id_T const tid, uint32_t const mailbox_size)
{
	union Worker this = Worker_Default();
   Object_Update_Info(&this.Object, &Task_Tid(tid).Object, sizeof(this.Task), 0);
   this->mailbox_size = mailbox_size;
   return this;
}

union Worker * Worker_Tid_New(IPC_Task_Id_T const tid, uint32_t const mailbox_size)
{
   Constructor_New_Impl(Worker, Tid, tid, mailbox_size);
}

void Worker_runnnable(Task_T * const super)
{
   Worker_T * const this = _dynamic_cast(Worker, super);
   Isnt_Nullptr(this,);

   union Mailbox mailbox = Mailbox_Size(this->tid, this->mailbox_size);

   Worker_Register_Mailbox(this, &mailbox);

   IPC_Task_Ready();

   this->vtbl->on_start(this);

   while(true)
   {
      this->vtbl->on_loop(this);

      union Mail * const mail = IPC_Retrieve(500);
      if(NULL != mail)
      {
    	  this->vtbl->on_mail(this, mail);
    	  if(WORKER_BCT_SHUTDOWN_MID == mail->mid)
    	  {
    		  break;
    	  }
      }
      IPC_Sleep(200);
   }

   Dbg_Info("shutdown %d\n", super->tid);

   this->vtbl->on_stop(this);

   Worker_Unregister_Mailbox(this, &mailbox);
   _delete(&mailbox);
}

void Worker_on_start(union Worker * const this){}
void Worker_on_mail(union Worker * const this, union Mail * const mail){}
void Worker_on_loop(union Worker * const this){}
void Worker_on_stop(union Worker * const this){}
/*=====================================================================================* 
 * worker.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
