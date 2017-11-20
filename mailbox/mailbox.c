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
#undef Dbg_FID
#define Dbg_FID Dbg_FID_Def(IPC_FID,3)
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "dbg_log.h"
#include "ipc.h"
#include "conditional.h"
#include "mutex.h"
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

/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEF(Mailbox)
/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/
void Mailbox_Init(void)
{
   Mailbox_Obj.owner = IPC_TOTAL_TASK_IDS_ITEMS;
   Mailbox_Obj.mailbox = Queue_Mail();
   Mailbox_Obj.data_size = 0;
   Mailbox_Obj.mutex = NULL;
}

void Mailbox_Delete(struct Object * const obj)
{
   Mailbox_T * const this = _dynamic_cast(Mailbox, obj);
   Isnt_Nullptr(this,);

   _delete(&this->mailbox);
   _delete(this->mutex);
   _delete(this->cond);
}

/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
union Mailbox Mailbox_Setup(IPC_Task_Id_T const owner, uint32_t const mail_elems, size_t const data_size)
{
   union Mailbox this = Mailbox_Populate();
   this->owner = owner;
   this->mailbox.vtbl->reserve(&this->mailbox, mail_elems);
   this->data_size = data_size;
   this->mutex = (Mutex_T * const) Linux_Mutex_new();
   this->cond = (Conditional_T * const) Linux_Conditional_new();
   this->cond->vtbl->ctor(this->cond, this->mutex);
}
union Mailbox * Mailbox_Setup_New(IPC_Task_Id_T const owner, uint32_t const mail_elems, size_t const data_size)
{
   Constructor_New_Impl(Mailbox, owner, mail_elems, data_daze)

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
   Dbg_Info("%s", __FUNCTION__);
   if(this->mutex->vtbl->lock(this->mutex, 200U))
   {

      this->mailbox.vtbl->push_back(&this->mailbox, mail);

      for(Mail_T * it = this->mailbox.vtbl->begin(&this->mailbox);
            it != this->mailbox.vtbl->end(&this->mailbox); ++it)
      {
         Dbg_Verbose("mail %d {id %d, sender %d, recv %d}", this->owner,
               it->mail_id, it->sender_task, it->receiver_task);
      }

   this->mutex->vtbl->unlock(this->mutex);
   this->cond->vtbl->signal(this->cond);
   }
   else
   {
      Dbg_Warn("Mailbox_push_mail::mutex tout!!");
   }
}

Mail_T const * Mailbox_pop_mail(Mailbox_T * const this, uint32_t const tout_ms)
{
   Mail_T * mail = NULL;
   bool_t found = false;
   Dbg_Info("%s", __FUNCTION__);
   if(this->mutex->vtbl->lock(this->mutex, tout_ms))
   {
      do
      {
         for(mail = this->mailbox.vtbl->begin(&this->mailbox);
               mail != this->mailbox.vtbl->end(&this->mailbox); ++mail)
         {
            Dbg_Verbose("pop mail mid %d %s dumpable", mail->mail_id, (mail->is_dumpable)? "is": "is not");
            if(!mail->is_dumpable)
            {
               mail->vtbl->dump(mail);
               break;
            }
         }

         Dbg_Verbose("no popped mail");
         found = this->mailbox.vtbl->end(&this->mailbox) != mail;

      }while(!found && this->cond->vtbl->wait(this->cond, tout_ms));

   this->mutex->vtbl->unlock(this->mutex);

   }
   else
   {
      Dbg_Warn("Mailbox_pop_mail::mutex tout!!");
   }
   return (found)? mail : NULL;
}

void Mailbox_dump(Mailbox_T * const this)
{
   Dbg_Info("%s", __FUNCTION__);
   for(uint32_t i = 0; i < this->mailbox.vtbl->size(&this->mailbox); ++i)
   {
   }
}

Mail_T const * Mailbox_get_mail_by_mail_id(Mailbox_T * const this, IPC_Mail_Id_T const * const mid,
      uint32_t const elems, uint32_t const tout_ms)
{
   Dbg_Info("%s", __FUNCTION__);
   Mail_T * mail = NULL;
   bool_t found = false;
   if(this->mutex->vtbl->lock(this->mutex, tout_ms))
   {
      IPC_Timestamp_T tmstmp = IPC_Timestamp() + tout_ms;
      do
      {
         for(Mail_T * mail = this->mailbox.vtbl->begin(&this->mailbox);
               mail != this->mailbox.vtbl->end(&this->mailbox); ++mail)
         {
            for(uint32_t i = 0; i < elems; ++i)
            {
               if(mid[i] == mail->mail_id && !mail->is_dumpable)
               {
                  mail->vtbl->dump(mail);
                  break;
               }
            }
         }

         Dbg_Info("Mailbox_get_mail_by_mail_id no popped mail");
         found = this->mailbox.vtbl->end(&this->mailbox) != mail;

      }while(!found &&
            IPC_Time_Elapsed(tmstmp) &&
            this->cond->vtbl->wait(this->cond, tout_ms));

   this->mutex->vtbl->unlock(this->mutex);

   }
   else
   {
      Dbg_Warn("Mailbox_get_mail_by_mail_id::mutex tout!!");
   }
   return (found)? mail : NULL;
}

/*=====================================================================================* 
 * mailbox.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
