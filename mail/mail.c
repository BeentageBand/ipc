/**
 * mail.c
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#define OBJECT_IMPLEMENTATION
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

/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEF(Mail)
/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/
void Mail_Init(void)
{
   Mail_Obj.mail_id = IPC_BEGIN_PRIVATE_MAIL_LIST_ID;
   Mail_Obj.sender_task = IPC_BEGIN_PRIVATE_MAIL_LIST_ID;
   Mail_Obj.receiver_task = IPC_BEGIN_PRIVATE_MAIL_LIST_ID;
   Mail_Obj.is_dumpable = false;
}

void Mail_Delete(union Object * const obj)
{
   union Mail * const this = _dynamic_cast(Mail, obj);
   if(0 != this->data_size)
   {
      free(this->data);
      this->data = NULL;
   }
   this->data_size = 0;
}

/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
union Mail Mail_Fillup(IPC_Mail_Id_T const mail_id, IPC_Task_Id_T const sender_task,
      IPC_Task_Id_T const receiver_task, void const * data, size_t const data_size)
{
   union Mail this = Populate_Mail();
   this->mail_id = mail_id;
   this->sender_task = sender_task;
   this->receiver_task = receiver_task;

   this->vtbl->set_data(this, data, data_size);
}

union Mail * Mail_Fillup_New(IPC_Mail_Id_T const mail_id, IPC_Task_Id_T const sender_task,
      IPC_Task_Id_T const receiver_task, void const * data, size_t const data_size)
{
   Contructor_New_Impl(Mail, Fillup, mail_id, sender_task, receiver_task, data, data_size);
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
