#define OBJECT_IMPLEMENTATION
 
#include "mail.h"

static void mail_delete(struct Object * const obj);
static void mail_set_data(union Mail * const this,void const *, size_t const);
static void const * mail_get_data(union Mail * const this,void);
static size_t mail_get_data_size(union Mail * const this,void);
static void mail_dump(union Mail * const this,void);
static void mail_set_mail_id(union Mail * const this,IPC_Mail_Id_T const);
static void mail_set_sender_task(union Mail * const this,IPC_Task_Id_T const);
static void mail_set_receiver_task(union Mail * const this,IPC_Task_Id_T const);
static IPC_Mail_Id_T mail_get_mail_id(union Mail * const this,void);
static IPC_Task_Id_T mail_get_sender_task(union Mail * const this,void);
static IPC_Task_Id_T mail_get_receiver_task(union Mail * const this,void);

union Mail_Class Mail_Class =
{
	{mail_delete},
	mail_set_data,
	mail_get_data,
	mail_get_data_size,
	mail_dump,
	mail_set_mail_id,
	mail_set_sender_task,
	mail_set_receiver_task,
	mail_get_mail_id,
	mail_get_sender_task,
	mail_get_receiver_task
};

static union Mail Mail = {NULL};

void Mail_init(void)
{
}

void mail_delete(struct Object * const obj)
{
   union Mail * const this = Object_Cast(&Mail_Class.Class, obj);
   Isnt_Nullptr(this, );

   if(0 != this->data_size)
   {
      free(this->data);
      this->data = NULL;
   }
   this->data_size = 0;
}
 
void Populate_Mail(union Mail * const this, IPC_Mail_Id_T const mail_id, IPC_Task_Id_T const sender_task,
      IPC_Task_Id_T const receiver_task, void const * data, size_t const data_size)
{
	if(NULL == Mail.vtbl)
	{
		Mail.vtbl = &Mail_Class;
		Mail.mail_id = IPC_BEGIN_PRIVATE_MAIL_LIST_ID;
		Mail.sender_task = IPC_BEGIN_PRIVATE_MAIL_LIST_ID;
		Mail.receiver_task = IPC_BEGIN_PRIVATE_MAIL_LIST_ID;
		Mail.is_dumpable = false;
	}

	memcpy(this, &Mail, sizeof(Mail));

	this->mail_id = mail_id;
	this->sender_task = sender_task;
	this->receiver_task = receiver_task;

	this->vtbl->set_data(this, data, data_size);
}

void mail_set_data(Mail_T * const this, void const * data, size_t const data_size)
{
   Isnt_Nullptr(this,);
   Isnt_Nullptr(data,);

   if(NULL != this->data)
   {
      if(this->data_size != data_size)
      {

         free(this->data);
      }
   }

   this->data_size = data_size;
   this->data = malloc(this->data_size);
   memcpy(this->data, data, data_size);
}

void const * mail_get_data(Mail_T * const this)
{
   Isnt_Nullptr(this, NULL);
   return this->data;
}

size_t mail_get_data_size(Mail_T * const this)
{
   Isnt_Nullptr(this, 0);
   return this->data_size;
}

void mail_dump(Mail_T * const this)
{
   Isnt_Nullptr(this, );
   this->is_dumpable = true;
}

void mail_set_mail_id(Mail_T * const this, IPC_Mail_Id_T const mail_id)
{
   this->mail_id = mail_id;
}

void mail_set_sender_task(Mail_T * const this, IPC_Task_Id_T const sender_task)
{
   this->sender_task = sender_task;
}

void mail_set_receiver_task(Mail_T * const this, IPC_Task_Id_T const receiver_task)
{
   this->receiver_task = receiver_task;
}

IPC_Mail_Id_T mail_get_mail_id(Mail_T * const this)
{
   return this->mail_id;
}

IPC_Task_Id_T mail_get_sender_task(Mail_T * const this)
{
   return this->sender_task;
}

IPC_Task_Id_T mail_get_receiver_task(Mail_T * const this)
{
   return this->receiver_task;
}
