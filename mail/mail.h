#ifndef MAIL_H_
#define MAIL_H_
 
#include "ipc_types.h"
#include "cobject.h"
 
#ifdef __cplusplus
extern "C" {
#endif

typedef union Mail
{
	union Mail_Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		IPC_Mail_Id_T _private mail_id;
		IPC_Task_Id_T _private sender_task;
		IPC_Task_Id_T _private receiver_task;
		void * _private data;
		size_t _private data_size;
		bool_t _private is_dumpable;
	};
}Mail_T;

typedef union Mail_Class
{
	struct
	{
		struct Class Class;
		void (* _private set_data)(union Mail * const this,void const *, size_t const);
		void const * (* _private get_data)(union Mail * const this,void);
		size_t (* _private get_data_size)(union Mail * const this,void);
		void (* _private dump)(union Mail * const this,void);
		void (* _private set_mail_id)(union Mail * const this,IPC_Mail_Id_T const);
		void (* _private set_sender_task)(union Mail * const this,IPC_Task_Id_T const);
		void (* _private set_receiver_task)(union Mail * const this,IPC_Task_Id_T const);
		IPC_Mail_Id_T (* _private get_mail_id)(union Mail * const this,void);
		IPC_Task_Id_T (* _private get_sender_task)(union Mail * const this,void);
		IPC_Task_Id_T (* _private get_receiver_task)(union Mail * const this,void);
	};
};

extern void Populate_Mail(union Mail * const this, IPC_Mail_Id_T const mail_id, IPC_Task_Id_T const sender_task, \
      IPC_Task_Id_T const receiver_task, void const * data, size_t const data_size);

extern union Mail_Class _private Mail_Class;

#ifdef __cplusplus
}
#endif
#endif /*MAIL_H_*/
