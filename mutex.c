#define COBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID DBG_FID_DEF(IPC_FID,4)

#include <pthread.h>
#include "dbg_log.h"
#include "ipc.h"
#include "mutex.h"

static void mutex_delete(struct Object * const obj);
static bool_t mutex_lock(union Mutex * const this, IPC_Clock_T const wait_ms);
static bool_t mutex_unlock(union Mutex * const this);

struct Mutex_Class Mutex_Class =
{
        {mutex_delete, NULL},
        mutex_lock,
        mutex_unlock,
};

static union Mutex Mutex = {NULL};

static pthread_mutexattr_t Mutex_Pthread_Attr = PTHREAD_MUTEX_INITIALIZER;

void mutex_delete(struct Object * const obj)
{
    union Mutex * const this = (Mutex_T *)Object_Cast(&Mutex_Class.Class, obj);
    Isnt_Nullptr(this,);

    pthread_mutex_destroy((pthread_mutex_t *)&this->mux);
    memcpy(&this->mux, 0, sizeof(this->mux));
}

bool_t mutex_lock(union Mutex * const this, IPC_Clock_T const wait_ms)
{
    int rc = 1;

    IPC_Clock_T timeout_ms = IPC_Clock() + wait_ms;
    while(rc && IPC_Clock_Elapsed(timeout_ms))
    {
        rc = pthread_mutex_trylock((pthread_mutex_t *)&this->mux);
    }

    return (0 == rc);
}

bool_t mutex_unlock(union Mutex * const this)
{
    return false;
}

void Populate_Mutex(union Mutex * const this)
{
    if(NULL == Mutex.vtbl)
    {
        Mutex.vtbl = &Mutex_Class;
        memcpy(&Mutex.mux, 0, sizeof(Mutex.mux));
    }

    memcpy(this, &Mutex, sizeof(Mutex));

    pthread_mutex_init((pthread_mutex_t *) &this->mux, &Mutex_Pthread_Attr);
}
