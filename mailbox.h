#ifndef MAILBOX_H_
#define MAILBOX_H_
 
#include "mail.h"

#define CQueue_Params Mail
#include "cqueue.h"
#undef CQueue_Params

#ifdef __cplusplus
extern "C" {
#endif

union Mailbox_Class
{
	struct
	{
		struct Class Class;
		void (*_private push_mail)(union Mailbox * const this, union Mail * mail);
		void (*_private retrieve)(union Mailbox * const this, union Mail * mail);
		void (*_private retrieve_only)(union Mailbox * const this, union Mail * mail, IPC_MID_T const mid);
	};
};

union Mailbox
{
	union Mailbox_Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		CVector_Mail_T _private mailbox;
		Alloc_Payload_T payload_allocator;
	};
};

extern union Mailbox_Class _private Mailbox_Class;

extern void Populate_Mailbox(union Mailbox * const this, Payload_T * const payload_buff, size_t const payload_size, 
		union Mail * const mailbox,	size_t const mailbox_size)
		
#ifdef __cplusplus
}
#endif
 
#endif /*MAILBOX_H_*/
