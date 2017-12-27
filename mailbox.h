#ifndef MAILBOX_H_
#define MAILBOX_H_
 
#include "ipc_types.h"
#include "mail.h"
#include "mailbox_ext.h"

#define CQueue_Params Mail
#include "cqueue.h"
#undef CQueue_Params

#ifdef __cplusplus
extern "C" {
#endif

typedef union Mailbox
{
	union Mailbox_Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		IPC_Task_Id_T _private owner;
		T_Queue(Mail) _private mailbox;
		T_Alloc(Pool) _private pool_data;
		size_t _private data_size;
		union Mailbox_Ext _private * _private, cbk;
	};
}Mailbox_T;

typedef union Mailbox_Class
{
	struct
	{
		struct Object Object;
		void (* _private push_mail)(union Mailbox * const, union Mail * const);
		union Mail * (* _private pop_mail)(union Mailbox * const);
		union Mail * (* _private find_mail)(union Mailbox * const, IPC_Mail_Id_T const);
	};
}Mailbox_Class_T;

extern Populate_Mailbox(union Mailbox * const this, IPC_Task_Id_T const owner, union Mail * const mail_buff, 
		uint32_t const mail_elems, void * const data_buff, size_t const data_size);

extern union Mailbox_Class _private Mailbox_Class;
 
#ifdef __cplusplus
}
#endif
 
#endif /*MAILBOX_H_*/
