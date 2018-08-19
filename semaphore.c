
#define COBJECT_IMPLEMENTATION
#define Dbg_FID DBG_FID_DEF(IPC_FID, 4)

#include "dbg_log.h"
#include "sem.h"
#include "ipc_helper.h"


static void semaphore_cbk_delete(struct Object * obj);

static void semaphore_delete(struct Object * obj);
static bool semaphore_wait(union Semaphore * const this, IPC_Clock_T const wait_ms);
static bool semaphore_post(union Semaphore * const this);

struct Semaphore_Cbk_Class  Semaphore_Cbk_Class =
{
    {semaphore_cbk_delete, NULL},
    NULL,
    NULL
};

struct Semaphore_Class Semaphore_Class =
{
        {semaphore_delete, NULL},
        semaphore_wait,
        semaphore_post
};

static union Semaphore Semaphore = {NULL};
static union Semaphore_Cbk Semaphore_Cbk = {NULL};

void semaphore_cbk_delete(struct Object * obj){}

void semaphore_delete(struct Object * obj)
{
    union Semaphore * const this = (Semaphore_T *)Object_Cast(&Semaphore_Class.Class, obj);
    Isnt_Nullptr(this, );
    _delete(this->cbk)
    free(this->cbk)
    this->cbk = NULL;
}

bool semaphore_wait(union Semaphore * const this, IPC_Clock_T const wait_ms)
{
    return this->cbk->vtbl->wait(this->cbk, this, wait_ms);
}

bool semaphore_post(union Semaphore * const this)
{
    return this->cbk->vtbl->post(this->cbk, this);
}

void Populate_Semaphore(union Semaphore * const this, uint32_t value)
{
    if(NULL == Semaphore.vtbl)
    {
        Semaphore.vtbl = &Semaphore_Class;
        Semaphore.cbk = NULL;
    }

    memcpy(this, &Semaphore, sizeof(Semaphore));
    IPC_Helper_T * ipc_help = IPC_get_instance();
    ipc_help->vtbl->alloc_semaphore(ipc_help, this, value);
}


