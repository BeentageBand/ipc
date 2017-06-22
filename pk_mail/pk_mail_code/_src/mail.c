/**
 * mail.c
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
#include "mail.h"
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
static void Mail_Ctor(Mail_T * const this, IPC_Mail_Id_T const mail_id, IPC_Task_Id_T const sender_task,
      IPC_Task_Id_T const receiver_task, void const * data, size_t const data_size);
static void Mail_set_data(Mail_T * const this, void const * data, size_t const data_size);
static void const * Mail_get_data(Mail_T * const this);
static size_t Mail_get_data_size(Mail_T * const this);
static void Mail_dump(Mail_T * const this);
static void Mail_set_mail_id(Mail_T * const this, IPC_Mail_Id_T const mail_id);
static void Mail_set_sender_task(Mail_T * const this, IPC_Task_Id_T const sender_task);
static void Mail_set_receiver_task(Mail_T * const this, IPC_Task_Id_T const receiver_task);
static IPC_Mail_Id_T Mail_get_mail_id(Mail_T * const this);
static IPC_Task_Id_T Mail_get_sender_task(Mail_T * const this);
static IPC_Task_Id_T Mail_get_receiver_task(Mail_T * const this);
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
void Mail_init(void)
{
   printf("%s \n", __FUNCTION__);
   Mail_Obj.mail_id = 0;
   Mail_Obj.mail_id = 0;
   Mail_Obj.mail_id = 0;
   Mail_Obj.is_dumpable = false;

   Mail_Vtbl.ctor = Mail_Ctor;
   Mail_Vtbl.set_data = Mail_set_data;
   Mail_Vtbl.get_data = Mail_get_data;
   Mail_Vtbl.get_data_size = Mail_get_data_size;
   Mail_Vtbl.dump= Mail_dump;
   Mail_Vtbl.set_mail_id = Mail_set_mail_id;
   Mail_Vtbl.set_sender_task = Mail_set_sender_task;
   Mail_Vtbl.set_receiver_task = Mail_set_receiver_task;
   Mail_Vtbl.get_mail_id = Mail_get_mail_id;
   Mail_Vtbl.get_sender_task = Mail_get_sender_task;
   Mail_Vtbl.get_receiver_task = Mail_get_receiver_task;

}
void Mail_shut(void) {}

void Mail_Dtor(Object_T * const obj)
{
}

/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void Mail_Ctor(Mail_T * const this, IPC_Mail_Id_T const mail_id, IPC_Task_Id_T const sender_task,
      IPC_Task_Id_T const receiver_task, void const * data, size_t const data_size)
{
   this->mail_id = mail_id;
   this->mail_id = sender_task;
   this->mail_id = receiver_task;

   this->vtbl->set_data(this, data, data_size);
}

void Mail_set_data(Mail_T * const this, void const * data, size_t const data_size)
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

void const * Mail_get_data(Mail_T * const this)
{
   Isnt_Nullptr(this, NULL);
   return this->data;
}

size_t Mail_get_data_size(Mail_T * const this)
{
   Isnt_Nullptr(this, 0);
   return this->data_size;
}

void Mail_dump(Mail_T * const this)
{
   Isnt_Nullptr(this, );
   this->is_dumpable = true;
}

void Mail_set_mail_id(Mail_T * const this, IPC_Mail_Id_T const mail_id)
{
   this->mail_id = mail_id;
}

void Mail_set_sender_task(Mail_T * const this, IPC_Task_Id_T const sender_task)
{
   this->sender_task = sender_task;
}

void Mail_set_receiver_task(Mail_T * const this, IPC_Task_Id_T const receiver_task)
{
   this->receiver_task = receiver_task;
}

IPC_Mail_Id_T Mail_get_mail_id(Mail_T * const this)
{
   return this->mail_id;
}

IPC_Task_Id_T Mail_get_sender_task(Mail_T * const this)
{
   return this->sender_task;
}

IPC_Task_Id_T Mail_get_receiver_task(Mail_T * const this)
{
   return this->receiver_task;
}
/*=====================================================================================* 
 * mail.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
