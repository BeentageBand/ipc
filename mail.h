#ifndef MAIL_H_
#define MAIL_H_
 
#include "ipc_types.h"

typedef uint8_t Payload_T;

#ifdef __cplusplus
extern "C" {
#endif

#define CVector_Params Payload
#include "cvector.h"
#undef CVector_Params

union Mail
{
	struct Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		IPC_TID_T _private sender;
		IPC_TID_T _private receiver;
		IPC_MID_T _private mid;
		void _private * _private payload;
		size_t _private pay_size;
		Alloc_Payload_T _private * _private pay_allocator;
	};

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

extern void Populate_Mail(union Mail * const this, IPC_TID_T const sender, IPC_TID_T const receiver, Alloc_Payload_T * const pay_allocator, payload, pay_size);

extern union Mail_Class _private Mail_Class;

#ifdef __cplusplus
}
#endif
#endif /*MAIL_H_*/
