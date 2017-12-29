#define COBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID Dbg_FID_Def(IPC_FID,6)
 
#include <pthread.h>
#include "dbg_log.h"
#include "conditional.h"
#include "ipc_types.h"

 
static void conditional_delete(struct Object * const obj);
static bool_t conditional_wait(union Conditional * const this, IPC_Clock_T const wait_ms);
static bool_t conditional_signal(union Conditional * const this);

union Conditional_Class Conditional_Class =
{
	{conditional_delete, NULL},
	NULL,
	NULL
};

union Conditional Conditional = {NULL};

static pthread_cond_attr_t Conditional_Pthread_Attr = PTHREAD_COND_ATTR_INIT;

void conditional_delete(struct Object * const obj)
{
	union Conditional * const this = (Conditional_T *)Object_Cast(&Conditional_Class.Class, obj);
	Isnt_Nullptr(this, );

	this->mutex = NULL;
}

bool_t conditional_wait(union Conditional * const this, IPC_Clock_T const wait_ms)
{
	struct timespec tp = {};
	tp.tv_sec = wait_ms / IPC_CLOCK_MS_SEC;
	tp.tv_nsec = wait_ms - (tp.sec * IPC_CLOCK_MS_SEC);

	int rc = pthread_cond_timed_wait((pthread_cond_t *)&this->conditional, 
			(pthread_mutex_t *)&this->mutex->mux, &tp);
	return (0 == rc);
}

bool_t conditional_signal(union Conditional * const this)
{
	int rc = pthread_cond_signal((pthread_cond_t *)&this->conditional);
	return (0 == rc);
}

void Populate_Conditional(union Conditional * const this, union Mutex * const mutex)
{
	if(NULL == Conditional.vtbl)
	{
		Conditional.vtbl = &Conditional_Class;
		Conditional.conditional = 0;
	}
	pthread_cond_init((pthread_cond_t *)&this->conditional, &Conditional_Pthread_Attr);
	memcpy(this, &Conditional, sizeof(Conditional));
	this->mutex = mutex;
}
