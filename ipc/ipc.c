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
#undef Dbg_FID
#define Dbg_FID Dbg_FID_Def(IPC_FID,0)
#define CLASS_IMPLEMENTATION
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "dbg_log.h"
#include "ipc_set.h"
#include "ipc.h"
#include "mailbox.h"
#include "task.h"
#include "task_ext.h"
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

/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEF(IPC)

/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/
void IPC_Init(void)
{
	IPC_Obj.publisher = Publisher();
	IPC_Obj.mailboxes = Map_Mbx_Ptr();
	IPC_Obj.tasks = Map_Task_Ptr();
}

void IPC_Delete(Object_T * const obj)
{
	_delete(&IPC_Obj.publisher);
	_delete(&IPC_Obj.mailboxes);
	_delete(&IPC_Obj.tasks);
}

/*=====================================================================================*
 * Exported Function Definitions
 *=====================================================================================*/
union IPC * IPC_get_instance(void)
{
	(void)IPC_Default();
	return & IPC_Obj;
}

bool_t Task_Register_To_Process(union Task * const task)
{
	union IPC * ipc = IPC_get_instance();
	Isnt_Nullptr(ipc, false);
	ipc->tasks->vtbl->insert(&ipc->tasks, task->tid, task);
	return true;
}

/** IPC APIs **/

int IPC_Run(IPC_Task_Id_T const tid)
{
	union IPC * this = IPC_get_instance();
	Isnt_Nullptr(this, -1);
	union Task * const task = this->tasks->vtbl->find(&this->tasks, tid);
	Isnt_Nullptr(task, -1);
	return this->vtbl->run(this, task);
}

int IPC_Wait(IPC_Task_Id_T const tid, uint32_t wait_ms)
{
	union IPC * this = IPC_get_instance();
	Isnt_Nullptr(this, -1);
	union Task * const task = this->tasks->vtbl->find(&this->tasks, tid);
	Isnt_Nullptr(task, -1);

	return task->vtbl->wait(task, wait_ms);
}

void IPC_Register_Mailbox(union Mailbox * const mailbox)
{
	union IPC * this = IPC_get_instance();
	Isnt_Nullptr(this, );
	this->mailboxes->vtbl->insert(&this->mailboxes, mailbox->owner, mailbox);
}

void IPC_Unregister_Mailbox(void)
{
	union IPC * this = IPC_get_instance();
   Isnt_Nullptr(this, );

   this->mailboxes->vtbl->erase(&this->mailboxes, IPC_Self_Task_Id());

}
/**
 * TB Mail Comm
 */
bool_t IPC_Subscribe_Mail_List(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems)
{
	union IPC * this = IPC_get_instance();
	Isnt_Nullptr(this, false);

	union Mailbox * const mailbox = this->mailboxes->vtbl->find(&this->mailboxes, IPC_Self_Task_Id());

	Isnt_Nullptr(this, false);

	bool_t rc = true;
	uint32_t i;

	for(i = 0; i < mail_elems && rc; ++i)
	{
		rc= this->publisher->vtbl->subscribe(&this->publisher, mail_list[i], mailbox);
	}
	return rc;
}

bool_t IPC_Unsubscribe_Mail_List(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems)
{
	union IPC * const this = IPC_get_instance();
	Isnt_Nullptr(this, false);

	union Mailbox * mailbox = this->mailboxes->vtbl->find(&this->mailboxes, IPC_Self_Task_Id());
	Isnt_Nullptr(mailbox, false);

	bool_t rc = true;
	uint32_t i;
	for(i = 0; i < mail_elems && rc; ++i)
	{
		rc = this->publisher->vtbl->subscribe(&this->publisher, mail_list[i], mailbox);
	}
	return rc;
}

void IPC_Send(IPC_Task_Id_T const receiver_task, IPC_Mail_Id_T mail_id,
		void const * data, size_t const data_size)
{
	union IPC * this = IPC_get_instance();
	Isnt_Nullptr(this, );

	union Mailbox * const mailbox = this->mailboxes->vtbl->find(&this->mailboxes, receiver_task);
	Dbg_Verbose("%s mbx %s", __FUNCTION__, (mailbox)? "found":"not found");
	Isnt_Nullptr(mailbox, );

	union Mail mail = Mail_Fillup(mail_id, receiver_task, data, data_size);

	mailbox->vtbl->push_mail(mailbox, &mail);
}

void IPC_Publish(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size)
{
	union IPC * this = IPC_get_instance();
	Isnt_Nullptr(this, );

	union Mail mail = Mail_Fillup(mail_id, 0, data, data_size);
	this->publisher->vtbl->publish_mail(&mail);

	_delete(&mail);
}

void IPC_Broadcast(IPC_Mail_Id_T const mail_id, void const * data, size_t const data_size)
{
   union IPC * this = IPC_get_instance();
   Isnt_Nullptr(this, );

   union Mailbox * it;
   for(it = this->mailboxes->vtbl->begin(&this->mailboxes);
		it != NULL &&
		it < this->mailboxes->vtbl->end(&this->mailboxes);
		++it)
   {
      union Mail mail = Mail_Fillup(mail_id, it->owner, data, data_size);
      it->vtbl->push_mail(mailbox, &mail);
      _delete(&mail);
   }
}

union Mail const * IPC_Retreive_From_Mail_List(IPC_Mail_Id_T const * mail_list, uint32_t const mail_elems,
      uint32_t const timeout_ms)
{
	Dbg_Verbose("Retreive_From_Mail_List");
	Isnt_Nullptr(mail_list, NULL);

	union IPC * this = IPC_get_instance();
	Isnt_Nullptr(this, NULL);

	union Mailbox * mailbox = this->mailboxes->vtbl->find(&this->mailboxes, IPC_Self_Task_Id());
	Isnt_Nullptr(mailbox, NULL);

	union Mail * const mail = mailbox->vtbl->find_only(mailbox, mail_list, mail_elems, timeout_ms);
	//Dbg_Verbose("mailbox %d %s", this->vtbl->get_tid(this), (mailbox)? "found" : "NULL");
	Isnt_Nullptr(mail, NULL);


	Dbg_Verbose("%s", mail ? "found" : "NULL");
	return mail;
}

union Mail const * IPC_Retreive_Mail(uint32_t const timeout_ms)
{
	Dbg_Verbose("Retreive_From_Mail_List");
	Isnt_Nullptr(mail_list, NULL);

	union IPC * this = IPC_get_instance();
	Isnt_Nullptr(this, NULL);

	union Mailbox * mailbox = this->mailboxes->vtbl->find(&this->mailboxes, IPC_Self_Task_Id());
	Isnt_Nullptr(mailbox, NULL);

	union Mail * const mail = mailbox->vtbl->pop_mail(mailbox, timeout_ms);
	//Dbg_Verbose("mailbox %d %s", this->vtbl->get_tid(this), (mailbox)? "found" : "NULL");
	Isnt_Nullptr(mail, NULL);


	Dbg_Verbose("%s", mail ? "found" : "NULL");
   return mail;
}
/*=====================================================================================* 
 * ipc.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
