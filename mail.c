#define COBJECT_IMPLEMENTATION
#define Dbg_FID DBG_FID_DEF(IPC_FID, 4)

#include "dbg_log.h"
#include "mail.h"
#include "mail_payload.h"

static void mail_delete(struct Object * const obj);
static void mail_set_payload(union Mail * const this, void const * const payload, size_t const pay_size);
static void mail_dump(union Mail * const this);
static void mail_set_mid(union Mail * const this,IPC_MID_T const);
static void mail_set_sender(union Mail * const this,IPC_TID_T const);
static void mail_set_receiver(union Mail * const this,IPC_TID_T const);

struct Mail_Class Mail_Class =
    {
   {mail_delete, NULL},
   mail_set_payload,
   mail_dump,
   mail_set_mid,
   mail_set_sender,
   mail_set_receiver,
    };

static union Mail Mail = {NULL};

void mail_delete(struct Object * const object)
{
  union Mail * const this = (union Mail * ) Object_Cast(&Mail_Class.Class, object);

  Isnt_Nullptr(this, );

  Mail_Payload_Free(&(this->payload));
  this->payload = NULL;
  this->pay_size = 0;
}

void Populate_Mail(union Mail * const this, IPC_MID_T const mid, IPC_TID_T const sender,
         IPC_TID_T const receiver, void const * const payload, size_t const pay_size)
{
  if(NULL == Mail.vtbl)
    {
      Mail.vtbl = &Mail_Class;
      Mail.mid = IPC_MAX_MID;
      Mail.sender = IPC_MAX_TID;
      Mail.receiver = IPC_MAX_TID;
    }

  memcpy(this, &Mail, sizeof(Mail));
  this->sender = sender;
  this->receiver = receiver;
  this->mid = mid;
  this->vtbl->set_payload(this, payload, pay_size);
}

void mail_set_payload(union Mail * const this, void const * const payload, size_t const pay_size)
{
  Isnt_Nullptr(this, );

  if(NULL != this->payload)
    {
      if(this->pay_size != pay_size)
   {
     Mail_Payload_Free(this->payload);
     this->payload = NULL;
     this->pay_size = 0;
   }
    }

  this->pay_size = pay_size;

  if(this->pay_size)
    {
      Mail_Payload_Alloc(&(this->payload), this->pay_size);
      memcpy(this->payload, payload, pay_size);
    }
}

void mail_dump(union Mail * const this)
{
  Isnt_Nullptr(this, );
  //TODO remove this
}

void mail_set_mid(union Mail * const this, IPC_MID_T const mid)
{
  this->mid = mid;
}

void mail_set_sender(union Mail * const this, IPC_TID_T const sender_task)
{
  this->sender = sender_task;
}

void mail_set_receiver(union Mail * const this, IPC_TID_T const receiver_task)
{
  this->receiver = receiver_task;
}
