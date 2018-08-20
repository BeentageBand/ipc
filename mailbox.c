#define COBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID DBG_FID_DEF(IPC_FID,3)

#define IPC_MAILBOX_LOCK_MS (200)

#include "dbg_log.h"
#include "ipc_helper.h"
#include "mailbox.h"

#define CQueue_Params Mail
#include "cqueue.c"
#undef CQueue_Params

static void mailbox_cbk_delete(struct Object * const obj);
static bool mailbox_cbk_register_mbx(union Mailbox_Cbk * const this, union Mailbox * const mailbox);
static bool mailbox_cbk_unregister_mbx(union Mailbox_Cbk * const this, union Mailbox * const mailbox);

static void mailbox_delete(struct Object * const obj);
static void mailbox_push_mail(union Mailbox * const this, union Mail * mail);
static bool mailbox_retrieve(union Mailbox * const this, union Mail * mail);
static bool mailbox_retrieve_only(union Mailbox * const this, union Mail * mail, IPC_MID_T const mid);
static void mailbox_dump_ipc(CQueue_Mail_T * const mailbox, Dbg_Lvl_T const dbg_lvl);


struct Mailbox_Cbk_Class Mailbox_Cbk_Class =
{
    {mailbox_cbk_delete, NULL},
    mailbox_cbk_register_mbx,
    mailbox_cbk_unregister_mbx
};

struct Mailbox_Class Mailbox_Class =
{
   {mailbox_delete, NULL},
   mailbox_push_mail,
   mailbox_retrieve,
   mailbox_retrieve_only
};

static union Mailbox Mailbox = {NULL};
static union Mailbox_Cbk Mailbox_Cbk = {NULL};

void mailbox_cbk_delete(struct Object * const obj)
{
}

bool mailbox_cbk_register_mbx(union Mailbox_Cbk * const this, union Mailbox * const mailbox)
{
    Isnt_Nullptr(this, false);
    union IPC_Helper * const ipc_helper = IPC_get_instance();
    Isnt_Nullptr(ipc_helper, false);
    union Mutex * mux = ipc_helper->single_mux;
    Isnt_Nullptr(mux, false);
    if(!mux->vtbl->lock(mux, 200)) return false;
    CSet_Mailbox_Ptr_T * mbx = ipc_helper->rmailboxes;
    mbx->vtbl->insert(mbx, mailbox);
    mux->vtbl->unlock(mux);
    return NULL != IPC_Helper_find_mailbox(mailbox->tid);
}

bool mailbox_cbk_unregister_mbx(union Mailbox_Cbk * const this, union Mailbox * const mailbox)
{
    Isnt_Nullptr(this, false);
    union IPC_Helper * const ipc_helper = IPC_get_instance();
    Isnt_Nullptr(ipc_helper, false);
    union Mutex * mux = ipc_helper->single_mux;
    Isnt_Nullptr(mux, false);

    if(!mux->vtbl->lock(mux, 200)) return false;
    CSet_Mailbox_Ptr_T * mbx = ipc_helper->rmailboxes;
    mbx->vtbl->erase(mbx, mailbox);
    mux->vtbl->unlock(mux);
    return NULL == IPC_Helper_find_mailbox(mailbox->tid);
}

void mailbox_delete(struct Object * const obj)
{
   union  Mailbox * const this = (Mailbox_T *) Object_Cast(&Mailbox_Class.Class, obj);
   Isnt_Nullptr(this,);

   _delete(&this->mailbox);

   if(NULL == this->picked_mail.vtbl)
   {
       _delete(&this->picked_mail);
   }

   this->cbk->vtbl->unregister_mbx(this->cbk, this);
   _delete(this->cbk);
   free(this->cbk);
   this->cbk = NULL;
}

void mailbox_push_mail(union Mailbox * const this, union Mail * mail)
{
   CQueue_Mail_T * const mailbox = &this->mailbox;
    union Conditional * const cond = &this->cond;
    union Mutex * const mutex = & this->mux;

   Dbg_Info("%s:mid %d", __func__, mail->mid);

    if(mutex->vtbl->lock(mutex, IPC_MAILBOX_LOCK_MS))
    {
      mailbox->vtbl->push_front(mailbox, *mail);
      mailbox_dump_ipc(mailbox, DBG_INFO_LVL);
      cond->vtbl->signal(cond);
      mutex->vtbl->unlock(mutex);
    }
}
   
bool mailbox_retrieve(union Mailbox * const this, union Mail * mail)
{
    CQueue_Mail_T * const mailbox = &this->mailbox;
    union Conditional * const cond = &this->cond;
    union Mutex * const mutex = & this->mux;

    bool rc = false;
    if(mutex->vtbl->lock(mutex, IPC_MAILBOX_LOCK_MS))
    {
        while(!mailbox->vtbl->size(mailbox) &&
            cond->vtbl->wait(cond, IPC_MAILBOX_LOCK_MS))
        {}//Wait until mailbox has a mail

       if(mailbox->vtbl->size(mailbox))
        {
            if(NULL != this->picked_mail.vtbl)
            {
                //Delete previously picked mail
                _delete(&this->picked_mail);
            }
            //Copy from previous mail
            union Mail * mend = mailbox->vtbl->end(mailbox);
            memcpy(&this->picked_mail, --mend, sizeof(this->picked_mail));
            memcpy(mail, &this->picked_mail, sizeof(this->picked_mail));

            mailbox->vtbl->pop_back(mailbox);
            rc = true;
        }
       mailbox_dump_ipc(mailbox, DBG_INFO_LVL);
        mutex->vtbl->unlock(mutex);
    }

    return rc;
}
   
bool mailbox_retrieve_only(union Mailbox * const this, union Mail * mail, IPC_MID_T const mid)
{
    CQueue_Mail_T * const mailbox = &this->mailbox;
    union Conditional * const cond = &this->cond;
    union Mutex * const mutex = & this->mux;
    bool rc = false;

    if(mutex->vtbl->lock(mutex, IPC_MAILBOX_LOCK_MS))
    {
      while(!mailbox->vtbl->size(mailbox) &&
         cond->vtbl->wait(cond, IPC_MAILBOX_LOCK_MS))
      {}//Wait until mailbox has a mail

        if(mailbox->vtbl->size(mailbox))
        {
            //check picked mail is an active cobject
            if(NULL != this->picked_mail.vtbl)
            {
                _delete(&this->picked_mail);
            }
            union Mail * found;

            for(found = mailbox->vtbl->begin(mailbox); found != mailbox->vtbl->end(mailbox); ++found)
            {
                if(mid == found->mid)
                {
                    break;
                }
            }

            if(found != mailbox->vtbl->end(mailbox))
            {
                memcpy(&this->picked_mail, found, sizeof(this->picked_mail));

                memcpy(mail, found, sizeof(this->picked_mail));
                mailbox->vtbl->pop_back(mailbox);
             mailbox_dump_ipc(mailbox, DBG_INFO_LVL);
                rc = true;
            }
        }
      mutex->vtbl->unlock(mutex);
    }

    return rc;
}

void mailbox_dump_ipc(CQueue_Mail_T * const mailbox, Dbg_Lvl_T const dbg_lvl)
{
   union Mail * it;
   Dbg_Verb("[");
   for(it = mailbox->vtbl->begin(mailbox);
      it != mailbox->vtbl->end(mailbox);
      ++it)
   {
      Dbg_Verb("%d, ", it->mid);
   }

   Dbg_Verb("]");
}

void Populate_Mailbox(union Mailbox * const this, IPC_TID_T const tid, union Mail * const mailbox, size_t const mailbox_size)
{
   if(NULL == Mailbox.vtbl)
   {
      Mailbox.vtbl = &Mailbox_Class;
   }
   memcpy(this, &Mailbox, sizeof(Mailbox));
   this->tid = tid;
   Populate_CQueue_Mail(&this->mailbox, mailbox, mailbox_size);
   Populate_Mutex(&this->mux);
   Populate_Conditional(&this->cond, &this->mux);

   union IPC_Helper * const ipc_helper = IPC_Get_instance();
   Isnt_Nullptr(ipc_helper, );
   ipc_helper->vtbl->alloc_mailbox(ipc_helper, this);
   this->cbk->vtbl->register_mbx(this->cbk, this);
}
