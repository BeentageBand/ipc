#define CLASS_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID Dbg_FID_Def(IPC_FID,0)
 
#include "dbg_log.h"
#include "ipc.h"
#include "worker.h"
 
static void worker_delete(Object_T * const obj);
static void worker_runnable(union Thread * const super);
static void worker_on_start(union Worker * const this);
static void worker_on_mail(union Worker * const this, union Mail * const mail);
static void worker_on_loop(union Worker * const this);
static void worker_on_stop(union Worker * const this);

union Worker_Class Worker_Class =
{
	{worker_delete, NULL},
	worker_on_start,
	worker_on_mail,
	worker_on_loop,
	worker_on_stop
};

static union Worker Worker = {NULL};

 
void worker_delete(struct Object * const obj)
{
	union Worker * const this = (union Worker *) Object_Cast(&Worker_Class.Class, obj);
	Isnt_Nullptr(this,);

	_delete(&this->mailbox);
}
 
void Populate_Worker(union Worker * const this, IPC_Task_Id_T const tid, union Mail * const mail_buff, 
		size_t const mailsize)
{
	if(NULL == Worker.vtbl)
	{
		Populate_Thread(&Worker.Thread, tid);
		Object_Init(&Worker.Object, &Worker_Class.Class, Worker.vtbl, sizeof(Worker_Class.Thread));	
		Worker_Class.Thread.runnable = worker_runnable;
	}
	memcpy(this, &Worker, sizeof(Worker));

	Populate_Mailbox(&this->mailbox, tid, mail_buff, mailsize);
	return this;
}

void worker_runnable(union Thread * const super)
{
   union Worker * const this = _dynamic_cast(Worker, super);
   Isnt_Nullptr(this,);

   union Mailbox * const mailbox = &this->mailbox;
   Worker_Register_Mailbox(this, mailbox);

   if(NULL == mailbox->vtbl)
   {
	   Dbg_Error("No mailbox was initialized");
	   return;
   }

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

   Worker_Unregister_Mailbox(this, mailbox);
   _delete(&mailbox);
}

void worker_on_start(union Worker * const this){}
void worker_on_mail(union Worker * const this, union Mail * const mail){}
void worker_on_loop(union Worker * const this){}
void worker_on_stop(union Worker * const this){}
