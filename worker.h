#ifndef WORKER_H_
#define WORKER_H_
 
#include "thread.h"
#include "mailbox.h"
 
#ifdef __cplusplus
extern "C" {
#endif

typedef union Worker
{
	union Worker_Class _private * _private vtbl;
	struct Object Object;
	struct
	{
		union Thread Thread;
		union Mailbox _private Mailbox;
	};
}Worker_T;

typedef union Worker_Class
{
	struct Class Class;
	struct
	{
		union Thread_Class Thread;
		void (* _private on_mail)(union Worker * const, union Mail * const);
		void (* _private on_start)(union Worker * const);
		void (* _private on_loop)(union Worker * const);
		void (* _private on_stop)(union Worker * const);
	};
}Worker_Class_T;

extern union Worker_Class _private Worker_Class;
 
extern void Populate_Worker(union Worker * const worker, IPC_TID_T const tid, union Mail * const mail_buff,
		size_t const mail_size);
 
#ifdef __cplusplus
}
#endif
 
#endif /*WORKER_H_*/
