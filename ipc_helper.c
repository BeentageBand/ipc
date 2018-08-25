#define COBJECT_IMPLEMENTATION
#define Dbg_FID DBG_FID_DEF(IPC_FID, 0)

#include "dbg_log.h"
#include "ipc_helper.h"
#include "ipc.h"

typedef union Thread * Thread_Ptr_T;

#define CSet_Params Thread_Ptr
#include "cset.c"
#undef CSet_Params

typedef union Mailbox * Mailbox_Ptr_T;

#define CSet_Params Mailbox_Ptr
#include "cset.c"
#undef CSet_Params

static void ipc_helper_delete(struct Object * const obj);

static IPC_Clock_T ipc_helper_time(union IPC_Helper * const helper);
static void ipc_helper_sleep(union IPC_Helper * const helper, IPC_Clock_T const sleep_ms);

static IPC_TID_T ipc_helper_self_thread(union IPC_Helper * const helper);
static bool ipc_helper_alloc_thread(union IPC_Helper * const helper, union Thread * const thread);
static bool ipc_helper_alloc_mailbox(union IPC_Helper * const helper, union Mailbox * const mailbox);
static bool ipc_helper_alloc_mutex(union IPC_Helper * const helper, union Mutex * const mutex);
static bool ipc_helper_alloc_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore,
                   uint8_t const value);
static bool ipc_helper_alloc_conditional(union IPC_Helper * const helper, union Conditional * const conditional);
static bool ipc_helper_alloc_timer(union IPC_Helper * const helper, union Timer * const tmr);

static int ipc_helper_thread_cmp(Thread_Ptr_T const * a, Thread_Ptr_T const *b);
static int ipc_helper_mailbox_cmp(Mailbox_Ptr_T const * a,Mailbox_Ptr_T const *b);

IPC_Helper_Class_T IPC_Helper_Class = 
    {{
   { ipc_helper_delete, NULL},
   ipc_helper_time,
   ipc_helper_sleep,
   ipc_helper_self_thread,
   
   ipc_helper_alloc_thread,
   ipc_helper_alloc_mailbox,
   ipc_helper_alloc_mutex,
   ipc_helper_alloc_semaphore,
   ipc_helper_alloc_conditional,
   ipc_helper_alloc_timer,
    }};

static union IPC_Helper IPC_Helper = {NULL};
static union IPC_Helper * IPC_Helper_Singleton = NULL;
static CSet_Thread_Ptr_T Rthreads = {NULL};
static CSet_Mailbox_Ptr_T Rmailboxes = {NULL};
static Thread_Ptr_T Thread_Set[IPC_MAX_TID] = {0};
static Mailbox_Ptr_T Mailbox_Set[IPC_MAX_TID] = {0};
static union Mutex Singleton_Mux = {NULL};

void ipc_helper_delete(struct Object * const obj)
{
  IPC_Helper_T * this = (IPC_Helper_T *) Object_Cast(&IPC_Helper_Class.Class, obj);

  union Mutex * const mux = this->single_mux;

  mux->vtbl->lock(mux, 10000);
  _delete(this->rthreads);
  _delete(this->rmailboxes);

  mux->vtbl->unlock(mux);
  _delete(this->single_mux);
}

IPC_Clock_T ipc_helper_time(union IPC_Helper * const helper)
{
  IPC_Clock_T clock = 0;
  Dbg_Warn("%s is not implemented", __func__);
  return clock;
}

void ipc_helper_sleep(union IPC_Helper * const helper, IPC_Clock_T const sleep_ms)
{
  Dbg_Warn("%s is not implemented", __func__);
}

IPC_TID_T ipc_helper_self_thread(union IPC_Helper * const helper)
{
  IPC_TID_T tid = IPC_MAX_TID;
  Dbg_Warn("%s is not implemented", __func__);
  return tid;
}

bool ipc_helper_alloc_thread(union IPC_Helper * const helper, union Thread * const thread)
{
  bool rc = false;
  Dbg_Warn("%s is not implemented", __func__);
  return rc;
}

bool ipc_helper_alloc_mailbox(union IPC_Helper * const helper, union Mailbox * const mailbox)
{
  Dbg_Warn("%s is not implemented", __func__);
  return false;
}

bool ipc_helper_alloc_mutex(union IPC_Helper * const helper, union Mutex * const mutex)
{
  bool rc = false;
  Dbg_Warn("%s is not implemented", __func__);
  return rc;
}

bool ipc_helper_alloc_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore,
            uint8_t const value)
{
  bool rc = false;
  Dbg_Warn("%s is not implemented", __func__);
  return rc;
}

bool ipc_helper_alloc_conditional(union IPC_Helper * const helper, union Conditional * const conditional)
{
  bool rc = false;
  Dbg_Warn("%s is not implemented", __func__);
  return rc;
}


bool ipc_helper_alloc_timer(union IPC_Helper * const helper, union Timer * const tmr)
{
  bool rc = false;
  Dbg_Warn("%s is not implemented", __func__);
  return rc;
}

union IPC_Helper * IPC_get_instance(void)
{
  return IPC_Helper_Singleton;
}

int ipc_helper_thread_cmp(Thread_Ptr_T const * a, Thread_Ptr_T const * b)
{
  if((*a)->tid < (*b)->tid) return -1;
  if((*a)->tid ==  (*b)->tid) return 0;
  if((*a)->tid >  (*b)->tid) return 1;

  return 0;
}

int ipc_helper_mailbox_cmp(Mailbox_Ptr_T const * a,Mailbox_Ptr_T const *b)
{
  if((*a)->tid <  (*b)->tid) return -1;
  if((*a)->tid == (*b)->tid) return 0;
  if((*a)->tid >  (*b)->tid) return 1;

  return 0;
}

void Populate_IPC_Helper(union IPC_Helper * const this)
{
  if(NULL == IPC_Helper.vtbl)
    {
      memset(Thread_Set, 0, sizeof(Thread_Set));
      memset(Mailbox_Set, 0, sizeof(Mailbox_Set));

      IPC_Helper.vtbl = &IPC_Helper_Class;

     Populate_CSet_Cmp_Thread_Ptr(&Rthreads, Thread_Set, Num_Elems(Thread_Set),
               (CSet_Cmp_T) ipc_helper_thread_cmp);
      Populate_CSet_Cmp_Mailbox_Ptr(&Rmailboxes, Mailbox_Set, Num_Elems(Mailbox_Set),
                (CSet_Cmp_T) ipc_helper_mailbox_cmp);
      IPC_Helper.rthreads = &Rthreads;
      IPC_Helper.rmailboxes = &Rmailboxes;
      IPC_Helper.single_mux = &Singleton_Mux;
    }
    if(NULL == this->vtbl)
    {
      memcpy(this, &IPC_Helper, sizeof(IPC_Helper));
    }
}

union Thread * IPC_Helper_find_thread(IPC_TID_T const thread)
{
  (void)IPC_get_instance();
  Isnt_Nullptr(IPC_Helper_Singleton->rthreads, NULL);
  Isnt_Nullptr(IPC_Helper_Singleton->single_mux, NULL);

  union Thread t = {NULL};
  t.tid = thread;

  union Mutex * const mux = IPC_Helper_Singleton->single_mux;

  bool is_found = false;
  Thread_Ptr_T * found = NULL;

  if(mux->vtbl->lock(mux, 1000))
    {
      CSet_Thread_Ptr_T * const thread_stack = IPC_Helper_Singleton->rthreads;
      found = thread_stack->vtbl->find(thread_stack, &t);
      is_found = (found != thread_stack->vtbl->end(thread_stack));
      mux->vtbl->unlock(mux);
    }
  else
    {
      Dbg_Fault("unable to lock %s", __func__);
    }

  Dbg_Verb("%s tid %d %s found!!\n", __func__, thread, (is_found)? "is": "is not");
  return (is_found)? *found : NULL;
}

union Mailbox * IPC_Helper_find_mailbox(IPC_TID_T const mailbox)
{
  (void)IPC_get_instance();
  Isnt_Nullptr(IPC_Helper_Singleton->rmailboxes, NULL);

  union Mailbox m = {NULL};
  m.tid = mailbox;

  union Mutex * const mux = IPC_Helper_Singleton->single_mux;
  bool is_found = false;
  Mailbox_Ptr_T* found = NULL;

  if(mux->vtbl->lock(mux, 1000))
  {
      CSet_Mailbox_Ptr_T * const mailbox_stack = IPC_Helper_Singleton->rmailboxes;
      found = mailbox_stack->vtbl->find(mailbox_stack, &m);
      is_found = (found != mailbox_stack->vtbl->end(mailbox_stack));
      mux->vtbl->unlock(mux);
  }
  else
  {
      Dbg_Fault("unable to lock %s", __func__);
  }

  Dbg_Verb("%s tid %d %s found!!\n", __func__, mailbox, (is_found)? "is": "is not");
  return (is_found)? *found : NULL;
}

void IPC_Helper_Append(union IPC_Helper * appendable)
{
  if(NULL == IPC_Helper_Singleton)
  {
    IPC_Helper_Singleton = appendable;
  }
}
