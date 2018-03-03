#define COBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID DBG_FID_DEF(IPC_FID,3)

#define IPC_MAILBOX_LOCK_MS (200)

#include "dbg_log.h"
#include "mailbox.h"

#define CQueue_Params Mail
#include "cqueue.c"
#undef CQueue_Params

#ifdef CAlgo_Find_Params
static int calgo_mail_cmp(union Mail const * a, union Mail const * b);
#endif
static void mailbox_delete(struct Object * const obj);
static void mailbox_push_mail(union Mailbox * const this, union Mail * mail);
static bool mailbox_retrieve(union Mailbox * const this, union Mail * mail);
static bool mailbox_retrieve_only(union Mailbox * const this, union Mail * mail, IPC_MID_T const mid);

struct Mailbox_Class Mailbox_Class =
{
	{mailbox_delete, NULL},
	mailbox_push_mail,
	mailbox_retrieve,
	mailbox_retrieve_only
};

static union Mailbox Mailbox = {NULL};

#ifdef CAlgo_Find_Params
int calgo_mail_cmp(union Mail const * a, union Mail const * b)
{
    if(a->mid < b->mid) return -1;
    if(a->mid == b->mid) return 0;
    if(a->mid > b->mid) return 1;
}
#endif

void mailbox_delete(struct Object * const obj)
{
	union  Mailbox * const this = (Mailbox_T *) Object_Cast(&Mailbox_Class.Class, obj);
	Isnt_Nullptr(this,);

	_delete(&this->mailbox);

	if(NULL == this->picked_mail.vtbl)
	{
	    _delete(&this->picked_mail);
	}
}

void mailbox_push_mail(union Mailbox * const this, union Mail * mail)
{
	CQueue_Mail_T * const mailbox = &this->mailbox;
    union Conditional * const cond = &this->cond;
    union Mutex * const mutex = & this->mux;

    if(mutex->vtbl->lock(mutex, IPC_MAILBOX_LOCK_MS))
    {
	mailbox->vtbl->push_front(mailbox, *mail);
    cond->vtbl->signal(cond);
    }
    mutex->vtbl->unlock(mutex);
	
}
	
bool mailbox_retrieve(union Mailbox * const this, union Mail * mail)
{
    CQueue_Mail_T * const mailbox = &this->mailbox;
    union Conditional * const cond = &this->cond;
    union Mutex * const mutex = & this->mux;

    bool rc = false;
    if(mutex->vtbl->lock(mutex, IPC_MAILBOX_LOCK_MS))
    {
        while(!mailbox->vtbl->size(mailbox))
        {
            cond->vtbl->wait(cond, IPC_MAILBOX_LOCK_MS);
        }

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
    }

    mutex->vtbl->unlock(mutex);
    return rc;
}
	
bool mailbox_retrieve_only(union Mailbox * const this, union Mail * mail, IPC_MID_T const mid)
{
    CQueue_Mail_T * const mailbox = &this->mailbox;
    union Conditional * const cond = &this->cond;
    bool rc = false;

    if(cond->vtbl->wait(cond, IPC_MAILBOX_LOCK_MS))
    {
        if(0 == mailbox->vtbl->size(mailbox))
        {
            //check picked mail is an active cobject
            if(NULL != this->picked_mail.vtbl)
            {
                _delete(&this->picked_mail);
            }
            //TODO use calgorithm find
#ifdef CAlgo_find_Params
            mail->mid = mid;
            union Mail * found = CAlgo_Mail_find_first(mailbox->vtbl->begin(mailbox),
                    mailbox->vtbl->end(mailbox),
                    mail,
                    (CAlgo_Find_Cmp_T)calgo_mail_cmp);
#else
            union Mail * found;
#endif

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
                rc = true;
            }
        }
    }

    return rc;
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
}
