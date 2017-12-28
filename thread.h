#ifndef TASK_H_
#define TASK_H_
 
#include "ipc_types.h"
#include "cobject.h"
 
#ifdef __cplusplus
extern "C" {
#endif

typedef union Thread
{
	union Thread_Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		IPC_TID_T _private tid;
	};
}Thread_T;

typedef union Thread_Class
{
	struct
	{
		struct Class Class;
		void (* _private run)(union Thread * const);
		void (* _private wait)(union Thread * const);
		void (* _private runnable)(union Thread * const);
	};
}Thread_Class_T;

extern union Thread_Class _private Thread_Class;

extern void Populate_Thread(union Task * const task, IPC_TID_T const tid);
 
#ifdef __cplusplus
}
#endif
#endif /*TASK_H_*/
