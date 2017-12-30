#define COBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID Dbg_FID_Def(IPC_FID,4)
 
#include <pthread.h>
#include "dbg_log.h"
#include "mutex.h"

static void mutex_delete(struct Object * const obj);
static bool_t mutex_lock(union Mutex * const this, IPC_Clock_T const wait_ms);
static bool_t mutex_unlock(union Mutex * const this);
 
union Mutex_Class Mutex_Class =
{
	{mutex_delete, NULL};
	mutex_lock,
	mutex_unlock,
};

static union Mutex Mutex = {NULL};

static pthread_mutex_attr_t Mutex_Pthread_Attr = PTHREAD_MUTEX_INITIALIZER;

void mutex_delete(struct Object * const obj)
{
	union Mutex * const this = (Mutex_T *)Object_Cast(&Mutex_Class.Class, obj);
	Isnt_Nullptr(this,);

	this->mux = 0;
}

bool_t mutex_lock(union Mutex * const this, IPC_Clock_T const wait_ms)

	int rc = 1;

	IPC_Clock_T timeout_ms = IPC_Clock() + wait_ms;
	while(rc && IPC_Clock_Elapsed(timeout_ms))
	{
		rc = pthread_mutex_trylock((pthread_mutex_t *)this->mutex);
	}

	return (0 == rc);
}

bool_t mutex_unlock(union Mutex * const this)
{
}
void Populate_Mutex(union Mutex * const this)
{
	if(NULL == Mutex.vtbl)
	{
		Mutex.vtbl = &Mutex_Class;
		Mutext.mux = 0;
	}

	memcpy(this, &Mutex, sizeof(Mutex));

	pthread_mutex_init((pthread_mutex_t *) this->mux, &Mutex_Pthread_Attr);
}
