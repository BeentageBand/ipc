
#define COBJECT_IMPLEMENTATION

#include "sem.h"
#include "ipc_helper.h"


static void semaphore_delete(struct Object * obj);
static bool semaphore_wait(union Semaphore * const this, IPC_Clock_T const wait_ms);
static bool semaphore_post(union Semaphore * const this);

union Semaphore_Class Semaphore_Class =
{
        {semaphore_delete, NULL},
        semaphore_wait,
        semaphore_post
};

static union Semaphore Semaphore = {NULL};

void semaphore_delete(struct Object * obj)
{
    union Semaphore * const this = (Semaphore_T *)Object_Cast(&Semaphore_Class.Class, obj);
    Isnt_Nullptr(this, );

    IPC_Helper_T * ipc_help = IPC_get_instance();
    ipc_help->vtbl->free_semaphore(ipc_help, this);
}

bool semaphore_wait(union Semaphore * const this, IPC_Clock_T const wait_ms)
{
    IPC_Helper_T * ipc_help = IPC_get_instance();
    return ipc_help->vtbl->wait_semaphore(ipc_help, this, wait_ms);
}

bool semaphore_post(union Semaphore * const this)
{
    IPC_Helper_T * ipc_help = IPC_get_instance();
    return ipc_help->vtbl->post_semaphore(ipc_help, this);
}

void Populate_Semaphore(union Semaphore * const this, uint32_t value)
{
    if(NULL == Semaphore.vtbl)
    {
        Semaphore.vtbl = &Semaphore_Class;
    }

    IPC_Helper_T * ipc_help = IPC_get_instance();
    ipc_help->vtbl->alloc_semaphore(ipc_help, this, value);
}


