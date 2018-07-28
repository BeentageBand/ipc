#define COBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID DBG_FID_DEF(IPC_FID,6)

#include "dbg_log.h"
#include "conditional.h"
#include "ipc_helper.h"


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

void conditional_delete(struct Object * const obj)
{
  union Conditional * const this = (Conditional_T *)Object_Cast(&Conditional_Class.Class, obj);
  Isnt_Nullptr(this, );

  union IPC_Helper * const ipc_helper = IPC_get_instance();
  Isnt_Nullptr(ipc_helper, );

  ipc_helper->vtbl->free_conditional(ipc_helper, this);
  this->mutex = NULL;
}

bool_t conditional_wait(union Conditional * const this, IPC_Clock_T const wait_ms)
{
  union IPC_Helper * const ipc_helper = IPC_get_instance();
  Isnt_Nullptr(ipc_helper, false);
  return ipc_helper->vtbl->wait_conditional(ipc_helper, this, &this->mutex, wait_ms);
}

bool_t conditional_signal(union Conditional * const this)
{
  union IPC_Helper * const ipc_helper = IPC_get_instance();
  Isnt_Nullptr(ipc_helper, false);
  return ipc_helper->vtbl->post_conditional(ipc_helper, this);
}

void Populate_Conditional(union Conditional * const this, union Mutex * const mutex)
{
  if(NULL == Conditional.vtbl)
    {
      Conditional.vtbl = &Conditional_Class;
      Conditional.conditional = NULL;
    }

  union IPC_Helper * const ipc_helper = IPC_get_instance();
  Isnt_Nullptr(ipc_helper, );

  memcpy(this, &Conditional, sizeof(Conditional));
  this->mutex = mutex;
  ipc_helper->vtbl->alloc_conditional(ipc_helper, this);
}
