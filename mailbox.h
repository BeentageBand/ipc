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
		void (*_private push_mail)(union Mailbox * const, union Mail * const);
		bool (*_private retrieve)(union Mailbox * const, union Mail * const);
		bool (*_private retrieve_only)(union Mailbox * const, union Mail * const, IPC_MID_T const);
	};
};

typedef union Mailbox
{
	union Mailbox_Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		IPC_TID_T _private tid;
		CQueue_Mail_T _private mailbox;
		union Mail picked_mail;
	};
}Mailbox_T;

extern union Mailbox_Class _private Mailbox_Class;

extern void Populate_Mailbox(union Mailbox * const mbx, IPC_TID_T const tid, union Mail * const mailbox, size_t const mailbox_size);
		
#ifdef __cplusplus
}
#endif
 
#endif /*MAILBOX_H_*/
