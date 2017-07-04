/*=====================================================================================*/
/**
 * mailbox.cpp
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
#include "publisher.h"
#include "mailbox.h"
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
static void Mailbox_Ctor(Mailbox_T * const this, IPC_Task_Id_T const owner, uint32_t const mail_elems, size_t const data_size);
static bool_t Mailbox_subscribe(Mailbox_T * const this,  IPC_Mail_Id_T const mid);
static bool_t Mailbox_unsubscribe(Mailbox_T * const this,   IPC_Mail_Id_T const mid);
static void Mailbox_push_mail(Mailbox_T * const this, Mail_T * const mail);
static Mail_T const * Mailbox_pop_mail(Mailbox_T * const this);
static void Mailbox_dump(Mailbox_T * const this);
static Mail_T * const Mailbox_get_mail_by_mail_id(Mailbox_T * const this, IPC_Mail_Id_T const mid);
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
void Mailbox_init(void)
{
   printf("%s \n", __FUNCTION__);
   Mailbox_Obj.owner = IPC_TOTAL_TASK_IDS_ITEMS;
   Mailbox_Obj.mailbox = Vector_Mail_Ptr();
   Mailbox_Obj.data_size = 0;

   Mailbox_Vtbl.ctor = Mailbox_Ctor;
   Mailbox_Vtbl.subscribe = Mailbox_subscribe;
   Mailbox_Vtbl.unsubscribe = Mailbox_unsubscribe;
   Mailbox_Vtbl.push_mail = Mailbox_push_mail;
   Mailbox_Vtbl.pop_mail = Mailbox_pop_mail;
   Mailbox_Vtbl.dump = Mailbox_dump;
   Mailbox_Vtbl.get_mail_by_mail_id = Mailbox_get_mail_by_mail_id;
}

void Mailbox_shut(void) {}

void Mailbox_Dtor(Object_T * const obj)
{}

/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void Mailbox_Ctor(Mailbox_T * const this, IPC_Task_Id_T const owner, uint32_t const mail_elems, size_t const data_size)
{
   this->owner = owner;
   this->mailbox.vtbl->reserve(&this->mailbox, mail_elems);
   this->data_size = data_size;
}

bool_t Mailbox_subscribe(Mailbox_T * const this,  IPC_Mail_Id_T const mid)
{
   return Publisher_subscribe(this, mid);
}

bool_t Mailbox_unsubscribe(Mailbox_T * const this,   IPC_Mail_Id_T const mid)
{
   return Publisher_unsubscribe(this, mid);
}

void Mailbox_push_mail(Mailbox_T * const this, Mail_T * const mail)
{
  this->mailbox.vtbl->push_back(&this->mailbox, &mail); 
}

Mail_T const * Mailbox_pop_mail(Mailbox_T * const this)
{
   Mail_T * mail = this->mailbox.vtbl->front(&this->mailbox);

   mail->vtbl->dump(mail);
   return mail;
}

void Mailbox_dump(Mailbox_T * const this)
{
   //TODO remove_if is_dumpable
}

Mail_T * const Mailbox_get_mail_by_mail_id(Mailbox_T * const this, IPC_Mail_Id_T const mid)
{
   // find mail by id
   Mail_T * const mail = NULL;
   return mail;
}

/*=====================================================================================* 
 * mailbox.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
