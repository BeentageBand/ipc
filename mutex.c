#define COBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID DBG_FID_DEF(IPC_FID,4)

#include "dbg_log.h"
#include "ipc.h"
#include "mutex.h"
#include "ipc_helper.h"

static void mutex_cbk_delete(struct Object * const obj);

static void mutex_delete(struct Object * const obj);
static bool_t mutex_lock(union Mutex * const this, IPC_Clock_T const wait_ms);
static bool_t mutex_unlock(union Mutex * const this);

struct Mutex_Cbk_Class Mutex_Cbk_Class = 
{
  {mutex_cbk_delete, NULL},
  NULL,
  NULL
};

struct Mutex_Class Mutex_Class =
    {
   {mutex_delete, NULL},
   mutex_lock,
   mutex_unlock,
    };

static union Mutex Mutex = {NULL};
static union Mutex_Cbk Mutex_Cbk = {NULL};

void mutex_cbk_delete(struct Object * const obj)
{}

void mutex_delete(struct Object * const obj)
{
  union Mutex * const this = (Mutex_T *)Object_Cast(&Mutex_Class.Class, obj);
  Isnt_Nullptr(this,);
  _delete(this->cbk);
  free(this->cbk);
  this->cbk = NULL;
}

bool_t mutex_lock(union Mutex * const this, IPC_Clock_T const wait_ms)
{
  return this->cbk->vtbl->lock(this->cbk, this, wait_ms);
}

bool_t mutex_unlock(union Mutex * const this)
{
  return this->cbk->vtbl->unlock(this->cbk, this);
}

void Populate_Mutex(union Mutex * const this)
{
  if(NULL == Mutex.vtbl)
    {
      Mutex.vtbl = &Mutex_Class;
      Mutex.cbk = NULL;
    }

  _clone(this, Mutex);

  union IPC_Helper * const ipc_helper = IPC_get_instance();
  Isnt_Nullptr(ipc_helper, );

  ipc_helper->vtbl->alloc_mutex(ipc_helper, this);
}
