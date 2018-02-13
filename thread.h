#ifndef THREAD_H_
#define THREAD_H_
 
#include "ipc_types.h"
#include "sem.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef union Thread
{
	struct Thread_Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		IPC_TID_T _private tid;
		union Semaphore sem_ready;
		void _private * _private attr;
	};
}Thread_T;

typedef struct Thread_Class
{
	struct Class Class;
	void (* _private run)(union Thread * const);
	void (* _private wait)(union Thread * const, IPC_Clock_T const);
	void (* _private runnable)(union Thread * const);
}Thread_Class_T;

extern struct Thread_Class _private Thread_Class;

extern void Populate_Thread(union Thread * const thread, IPC_TID_T const tid);

extern IPC_TID_T Thread_self(void);
 
#ifdef __cplusplus
}
#endif
#endif /*THREAD_H_*/
