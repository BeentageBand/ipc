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
#include "publisher_ext.h"
#include "mailbox.h"
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
static void Publisher_Ctor(Publisher_T * const this, IPC_Task_Id_T const owner,
      uint32_t const mail_elems, size_t const data_size);
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
}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void Publisher_Ctor(Publisher_T * const this, Task_T * const owner, uint32_t const mail_elems, size_t const data_size)
{
   this->mailboxes = NULL; /*FIXME publisher ringbuffer*/
}

bool_t Publisher_subscribe(Mailbox_T * const mailbox)
{
   Singleton_Publisher(&Publisher_Obj);
   Isnt_Nullptr(Publisher_Obj.vtbl.subscribe, false);
   return Publisher_Obj.vtbl.subscribe(&Publisher_Obj, mailbox);
}
bool_t Publisher_unsubscribe(Mailbox_T * const mailbox)
{
   Singleton_Publisher(&Publisher_Obj);
   Isnt_Nullptr(Publisher_Obj.vtbl.subscribe, false);
   return Publisher_Obj.vtbl.unsubscribe(&Publisher_Obj, mailbox);
}
void Publisher_publish_mail(Mail_T * const mail)
{
   Singleton_Publisher(&Publisher_Obj);
   Isnt_Nullptr(Publisher_Obj.vtbl.subscribe, );
   Publisher_Obj.vtbl.publish_mail(&Publisher_Obj, mail);
}
/*=====================================================================================* 
 * publisher.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
