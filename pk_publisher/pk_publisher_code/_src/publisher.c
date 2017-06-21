/*=====================================================================================*/
/**
 * publisher.cpp
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
#include "mailbox.h"
#include "mailbox_ringbuffer.h"
#include "publisher_ext.h"
#include "publisher.h"
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
static void Publisher_Ctor(Publisher_T * const this, uint32_t const mail_elems, size_t const data_size);
/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEFINITION
static Publisher_T * Publisher_Singleton = NULL;
/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/
void Publisher_init(void)
{
   printf("%s \n", __FUNCTION__);
   Publisher_Obj.mailboxes = NULL;

   Publisher_Vtbl.Object.rtti = &Publisher_Rtti;
   Publisher_Vtbl.Object.destroy = Publisher_Dtor;
   Publisher_Vtbl.ctor = Publisher_Ctor;
   Publisher_Vtbl.subscribe = NULL;
   Publisher_Vtbl.unsubscribe = NULL;
   Publisher_Vtbl.publish_mail = NULL;
}

void Publisher_shut(void) {}

void Publisher_Dtor(Object_T * const obj)
{
   Publisher_T * this = _dynamic_cast(Publisher, obj);
   _delete(this->mailboxes);
}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void Publisher_Ctor(Publisher_T * const this, uint32_t const mail_elems, size_t const data_size)
{
   this->mailboxes = Vector_Mailbox_new();
}

bool_t Publisher_subscribe(Mailbox_T * const mailbox, IPC_Mail_Id_T const mail_id)
{
   Publisher_get_instance(&Publisher_Singleton);
   Isnt_Nullptr(Publisher_Singleton->vtbl->subscribe, false);
   return Publisher_Singleton->vtbl->subscribe(Publisher_Singleton, mailbox, mail_id);
}
bool_t Publisher_unsubscribe(Mailbox_T * const mailbox, IPC_Mail_Id_T const mail_id)
{
   Publisher_get_instance(&Publisher_Singleton);
   Isnt_Nullptr(Publisher_Singleton->vtbl->subscribe, false);
   return Publisher_Singleton->vtbl->unsubscribe(Publisher_Singleton, mailbox, mail_id);
}
void Publisher_publish_mail(Mail_T * const mail)
{
   Publisher_get_instance(&Publisher_Singleton);
   Isnt_Nullptr(Publisher_Singleton->vtbl->subscribe, );
   Publisher_Singleton->vtbl->publish_mail(Publisher_Singleton, mail);
}
/*=====================================================================================* 
 * publisher.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
