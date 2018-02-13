#define COBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID Dbg_FID_DEF(IPC_FID,6)
 
#include <pthread.h>
#include <time.h>
#include "dbg_log.h"
#include "conditional.h"

 
static void conditional_delete(struct Object * const obj);
static bool_t conditional_wait(union Conditional * const this, IPC_Clock_T const wait_ms);
static bool_t conditional_signal(union Conditional * const this);

struct Conditional_Class Conditional_Class =
{
	{conditional_delete, NULL},
	conditional_wait,
	conditional_signal
};

union Conditional Conditional = {NULL};

static pthread_condattr_t Conditional_Pthread_Attr = PTHREAD_COND_INITIALIZER;

void conditional_delete(struct Object * const obj)
{
	union Conditional * const this = (Conditional_T *)Object_Cast(&Conditional_Class.Class, obj);
	Isnt_Nullptr(this, );

	pthread_cond_destroy((pthread_cond_t *)&this->conditional); 
	this->mutex = NULL;
}

bool_t conditional_wait(union Conditional * const this, IPC_Clock_T const wait_ms)
{
	struct timespec tp = {0, 0};
	tp.tv_sec = wait_ms / IPC_CLOCK_MS_SEC;
	tp.tv_nsec = wait_ms - (tp.tv_sec * IPC_CLOCK_MS_SEC);

	tp.tv_nsec -= IPC_CLOCK_NS_MS;

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
