#define COBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID DBG_FID_DEF(IPC_FID,3)

#include "mailbox.h"

#define CQueue_Params Mail
#include "cqueue.c"
#undef CQueue_Params

static void mailbox_delete(struct Object * const obj);
static void mailbox_push_mail(union Mailbox * const this, union Mail * mail);
static bool mailbox_retrieve(union Mailbox * const this, union Mail * mail);
static bool mailbox_retrieve_only(union Mailbox * const this, union Mail * mail, IPC_MID_T const mid);
}

union Mailbox_Class _private Mailbox_Class =
{
	{NULL, mailbox_delete},
	mailbox_push_mail,
	mailbox_retrieve,
	mailbox_retrieve_only
};

static union Mailbox_Class Mailbox_Class = {NULL};

void mailbox_delete(struct Object * const obj)
{
	union  Mailbox * const this = (Mailbox_T *) Object_Cast(&Mailbox_Class.Class, obj);
	if(NULL == this) return;
	_delete(&this->mailbox);
	_delete(&this->payload_allocator);
}

void mailbox_push_mail(union Mailbox * const this, union Mail * mail)
{
	CQueue_Mail_T * const mailbox = this->mailbox;

	mailbox->vtbl->push(mailbox, *mail);
	
}
	
bool mailbox_retrieve(union Mailbox * const this, union Mail * mail)
{
	CQueue_Mail_T * const mailbox = this->mailbox;

	bool rc = false;
	if(!mailbox->vtbl->empty(mailbox))
	{
		if(0 == this->picked_mail->mid)
		{
			_delete(&this->picked_mail);
		}
		memcpy(&this->picked_mail, mailbox->vtbl->back(mailbox), sizeof(this->picked_mail));
		memcpy(mail, found, sizeof(this->picked_mail));

		mailbox->vtbl->pop(mailbox);
		rc = true;
	}
	return rc;
}
	
bool mailbox_retrieve_only(union Mailbox * const this, union Mail * mail, IPC_MID_T const mid)
{
	CQueue_Mail_T * const mailbox = this->mailbox;
	bool rc = false;

	if(!mailbox->vtbl->empty(mailbox))
	{
		if(0 == this->picked_mail->mid)
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
			mailbox->vtbl->pop(mailbox);
			rc = true;
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
}
