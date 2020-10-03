#define CHASHMAP_T_IMPLEMENTATION
#define CHASHSET_T_IMPLEMENTATION
#include "ipchelper-internal.h"
#include "logger/logger.h"

#define CHashMap_Params int, Thread_Ptr
#include "ctemplate-lib/hashmap/chashmap-int-template.h"
#undef CHashMap_Params

#define CHashMap_Params int, Mailbox_Ptr
#include "ctemplate-lib/hashmap/chashmap-int-template.h"
#undef CHashMap_Params

#define Dbg_Fault(...) Logger_error(get_log(), __VA_ARGS__)
#define Dbg_Verb(...) Logger_debug(get_log(), __VA_ARGS__)

static union Logger * get_log(void);
static void ipchelper_delete(union IPCHelper * const ipchelper);
static union IPCFactory * ipchelper_get_factory(union IPCHelper * ipchelper);
static void ipchelper_append(union IPCHelper * const ipchelper, union IPCHelper * const appendable);
static bool ipchelper_register_thread(union IPCHelper * ipchelper, union Thread * thread);
static bool ipchelper_register_mbx(union IPCHelper * ipchelper, union Mailbox * mailbox);
static bool ipchelper_unregister_thread(union IPCHelper * ipchelper, union Thread * thread);
static bool ipchelper_unregister_mbx(union IPCHelper * ipchelper, union Mailbox * mailbox);
static union Thread * ipchelper_find_thread(union IPCHelper * ipchelper, IPC_TID_T const tid);
static union Mailbox * ipchelper_find_mailbox(union IPCHelper * ipchelper, IPC_TID_T const mid);

union Logger * get_log(void)
{
  static union Logger log = {NULL};
  static union Formatter fmt = {NULL};
  static union LoggerHandler handler = {NULL};
  if (NULL == log.vtbl) {
      LoggerHandler_populate(&handler);
      Formatter_populate(&fmt, __FILE__, LOG_DEBUG_LEVEL, " ");
      Logger_populate(&log, &fmt, &handler);
  }
  return &log;
}

void ipchelper_override(union IPCHelper_Class * const clazz)
{
  clazz->Class.destroy = (Class_Delete_T) ipchelper_delete;
  clazz->append = ipchelper_append;
  clazz->get_factory = ipchelper_get_factory;
  clazz->register_thread = ipchelper_register_thread;
  clazz->register_mbx = ipchelper_register_mbx;
  clazz->register_thread = ipchelper_register_thread;
  clazz->register_mbx = ipchelper_register_mbx;
  clazz->find_mailbox = ipchelper_find_mailbox;
  clazz->find_thread = ipchelper_find_thread;
}

void ipchelper_delete(union IPCHelper * const ipchelper)
{
  _delete(&ipchelper->threadpool);
  _delete(&ipchelper->mailboxpool);
  _delete(ipchelper->mux);
  _delete(ipchelper->clock);
}

bool ipchelper_register_thread(union IPCHelper * ipchelper, union Thread * thread)
{
  if (IPCHelper_find_thread(ipchelper, thread->id)) return false;
  ThreadPool_T * threadpool = &ipchelper->threadpool;
  union Mutex * mux = ipchelper->mux;

  if (Mutex_lock(mux, 1000))
  {
    CHashMap_int_Thread_Ptr_insert(threadpool, thread->id, thread);
    Mutex_unlock(mux);
    return NULL != IPCHelper_find_thread(ipchelper, thread->id);
  }
  Dbg_Fault("unable to lock %s", __func__);
  return false;
}

bool ipchelper_register_mbx(union IPCHelper * ipchelper, union Mailbox * mailbox)
{
  if (IPCHelper_find_mailbox(ipchelper, mailbox->tid)) return false;
  MailboxPool_T * mailboxpool = &ipchelper->mailboxpool;
  union Mutex * mux = ipchelper->mux;

  if (Mutex_lock(mux, 1000))
  {
    CHashMap_int_Mailbox_Ptr_insert(mailboxpool, mailbox->tid, mailbox);
    Mutex_unlock(mux);
    return NULL != IPCHelper_find_mailbox(ipchelper, mailbox->tid);
  }
  Dbg_Fault("unable to lock %s", __func__);
  return false;
}

bool ipchelper_unregister_thread(union IPCHelper * ipchelper, union Thread * thread)
{
  if (!IPCHelper_find_thread(ipchelper, thread->id)) return false;
  ThreadPool_T * threadpool = &ipchelper->threadpool;
  union Mutex * mux = ipchelper->mux;

  if (Mutex_lock(mux, 1000))
  {
    CHashMap_int_Thread_Ptr_erase(threadpool, thread->id);
    Mutex_unlock(mux);
    return NULL == IPCHelper_find_thread(ipchelper, thread->id);
  }
  Dbg_Fault("unable to lock %s", __func__);
  return false;

}

bool ipchelper_unregister_mbx(union IPCHelper * ipchelper, union Mailbox * mailbox)
{
  if (IPCHelper_find_mailbox(ipchelper, mailbox->tid)) return false;
  MailboxPool_T * mailboxpool = &ipchelper->mailboxpool;
  union Mutex * mux = ipchelper->mux;

  if (Mutex_lock(mux, 1000))
  {
    CHashMap_int_Mailbox_Ptr_erase(mailboxpool, mailbox->tid);
    Mutex_unlock(mux);
    return NULL == IPCHelper_find_mailbox(ipchelper, mailbox->tid);
  }
  Dbg_Fault("unable to lock %s", __func__);
  return false;

}

union Thread * ipchelper_find_thread(union IPCHelper * const ipchelper, IPC_TID_T const tid)
{
  union Mutex * const mux = ipchelper->mux;

  if(Mutex_lock(mux, 1000))
  {
    ThreadPool_T * const threadpool = &ipchelper->threadpool;
    HashPair_int_Thread_Ptr * found = CHashMap_int_Thread_Ptr_find(threadpool, tid);
    bool is_found = found != CHashMap_int_Thread_Ptr_end(threadpool);
    Dbg_Verb("%s tid %d %sfound!!\n", __func__, tid, (is_found) ? "is" : "is not ");
    Mutex_unlock(mux);
    return (is_found) ? found->value : NULL;
  }
  Dbg_Fault("unable to lock %s", __func__);
  return NULL;
}

union Mailbox * ipchelper_find_mailbox(union IPCHelper * ipchelper, IPC_TID_T const tid)
{

  union Mutex * const mux = ipchelper->mux;
  if(Mutex_lock(mux, 1000))
  {
      MailboxPool_T * const mailboxpool = &ipchelper->mailboxpool;
      HashPair_int_Mailbox_Ptr * found = CHashMap_int_Mailbox_Ptr_find(mailboxpool, tid);
      bool is_found = (found != CHashMap_int_Mailbox_Ptr_end(mailboxpool));
      Mutex_unlock(mux);
      Dbg_Verb("%s tid %d %s found!!", __func__, tid, (is_found)? "is": "is not");
      return (is_found) ? found->value : NULL;
  }
  Dbg_Fault("unable to lock %s", __func__);
  return NULL;
}

union IPCFactory * ipchelper_get_factory(union IPCHelper * ipchelper)
{
  return ipchelper->factory;
}

void ipchelper_append(union IPCHelper * const ipchelper, union IPCHelper * appendable)
{
  union IPCHelper * it = ipchelper;
  while (NULL != it->next) it = it->next;
  it->next = appendable;
}

void IPCHelper_populate(union IPCHelper * const ipchelper, union IPCFactory * const factory)
{
  Object_populate(&ipchelper->Object, &Get_IPCHelper_Class()->Class);
  CHashMap_int_Thread_Ptr_populate(&ipchelper->threadpool, NULL, NULL);
  CHashMap_int_Mailbox_Ptr_populate(&ipchelper->mailboxpool, NULL, NULL);
  ipchelper->factory = factory;
  ipchelper->mux = IPCFactory_alloc_mutex(factory);
  ipchelper->clock = IPCFactory_alloc_clock(factory);
}
