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
static bool_t Mailbox_subscribe(Mailbox_T * const this, Publisher_T * publisher, Mail_Id_T const mail_id);
static bool_t Mailbox_unsubscribe(Mailbox_T * const this, Publisher_T * publisher, Mail_Id_T const mail_id);
static void Mailbox_push_mail(Mailbox_T * const this, Mail_T * const mail);
static Mail_T const * Mailbox_pop_mail(Mailbox_T * const this);
static void Mailbox_dump(Mailbox_T * const this);
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
   Mailbox_Obj.owner = NULL;
   Mailbox_Obj.mailbox = NULL;
   Mailbox_Obj.data_size = 0;

   Mailbox_Vtbl.Object.rtti = &Mailbox_Rtti;
   Mailbox_Vtbl.Object.destroy = Mailbox_Dtor;
   Mailbox_Vtbl.ctor = Mailbox_Ctor;
   Mailbox_Vtbl.subscribe = Mailbox_subscribe;
   Mailbox_Vtbl.unsubscribe = Mailbox_unsubscribe;
   Mailbox_Vtbl.push_mail = Mailbox_push_mail;
   Mailbox_Vtbl.pop_mail= Mailbox_pop_mail;
   Mailbox_Vtbl.dump = Mailbox_dump;

}
void Mailbox_shut(void) {}

void Mailbox_Dtor(Object_T * const obj)
{
}

/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void Mailbox_Ctor(Mailbox_T * const this, Task_T * const owner, uint32_t const mail_elems, size_t const data_size)
{
   this->owner = owner;
   this->mailbox = NULL; /*FIXME ring buffer ctor*/
   this->data_size = data_size;
}

bool_t Mailbox_subscribe(Mailbox_T * const this, Publisher_T * publisher, Mail_Id_T const mail_id)
{

}

bool_t Mailbox_unsubscribe(Mailbox_T * const this, Publisher_T * publisher, Mail_Id_T const mail_id)
{

}

void Mailbox_push_mail(Mailbox_T * const this,  Mail_T * const mail)
{

}

Mail_T const * Mailbox_pop_mail(Mailbox_T * const this)
{

}

void Mailbox_dump(Mailbox_T * const this)
{

}
/*=====================================================================================* 
 * mailbox.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
