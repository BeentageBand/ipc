#define COBJECT_IMPLEMENTATION
#define Dbg_FID DBG_FID_DEF(IPC_FID, 4)

#include "dbg_log.h"
#include "mail.h"
#include "mail_payload.h"

static void mail_delete(struct Object * const obj);
static void mail_set_data(union Mail * const this,void const * const, size_t const);
static void mail_dump(union Mail * const this);
static void mail_set_mid(union Mail * const this,IPC_MID_T const);
static void mail_set_sender(union Mail * const this,IPC_TID_T const);
static void mail_set_receiver(union Mail * const this,IPC_TID_T const);

union Mail_Class Mail_Class =
{
	{mail_delete, NULL},
	mail_set_data,
	mail_dump,
	mail_set_mid,
	mail_set_sender_task,
	mail_set_receiver_task,
};

static union Mail Mail = {NULL};

void mail_delete(struct Object * const object)
{
	union Mail * const this = (union Mail * ) Object_Cast(&Mail_Class, object);

	if(NULL == this) return;

	Mail_Payload_Free(&(this->payload));
}
 
void Populate_Mail(union Mail * const this, IPC_MID_T const mid, IPC_TID_T const sender_task,
      IPC_TID_T const receiver_task, void const * const payload, size_t const pay_size)
{
	if(NULL == Mail.vtbl)
	{
		Mail.vtbl = &Mail_Class;
		Mail.mid = IPC_BEGIN_PRIVATE_MAIL_LIST_ID;
		Mail.sender_task = IPC_BEGIN_PRIVATE_MAIL_LIST_ID;
		Mail.receiver_task = IPC_BEGIN_PRIVATE_MAIL_LIST_ID;
		Mail.is_dumpable = false;
	}

	memcpy(this, &Mail, sizeof(Mail));
	this->sender = sender;
	this->receiver = receiver;
	this->pay_size = pay_size;

	Mail_Payload_Alloc(&(this->payload), this->pay_size);
}
	
void mail_set_data(union Mail * const this, void const * const payload, size_t const pay_size)
{
	Isnt_Nullptr(this,);
	Isnt_Nullptr(data,);

	if(NULL != this->data)
	{
	   if(this->pay_size != pay_size)
	   {
		  Mail_Payload_Free(this->payload);
	   }
	}

	this->pay_size = pay_size;
	Mail_Payload_Alloc(&(this->payload), this->pay_size);
	memcpy(this->data, data, data_size);
}

void mail_dump(union Mail * const this)
{
   Isnt_Nullptr(this, );
   this->is_dumpable = true;
}

void mail_set_mid(union Mail * const this, IPC_MID_T const mid)
{
   this->mid = mid;
}

void mail_set_sender(union Mail * const this, IPC_TID_T const sender_task)
{
   this->sender_task = sender_task;
}

void mail_set_receiver(union Mail * const this, IPC_TID_T const receiver_task)
{
   this->receiver_task = receiver_task;
}
