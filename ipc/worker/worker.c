#include "worker-internal.h"
#include "ipc/ipc.h"
#include "logger/logger.h"
#define WORKER_SHUTDOWN_TOUT_MS (5000U)

#define Dbg_Fault(...) Logger_error(get_log(), __VA_ARGS__)
#define Dbg_Warn(...) Logger_warn(get_log(), __VA_ARGS__)

static union Logger * get_log(void);
static void worker_delete(union Worker * const worker);
static void worker_runnable(union Worker * const worker);

union Logger * get_log(void)
{
  static union Logger log = {NULL};
  if (NULL == log.vtbl)
  {
    Logger_populate(&log, NULL, NULL);
  }
  return &log;
}

void worker_override(union Worker_Class * const clazz)
{
  clazz->Class.destroy = worker_delete;
  clazz->runnable = worker_runnable;
}

void worker_delete(union Worker * const worker)
{
   if (worker->id == 0)
     {
       Dbg_Fault("%s deleted worker",__func__);
       return;
     }
   IPC_Send(worker->id, worker->shutdown_mid, NULL, 0);
   Thread_wait(&worker->Thread, WORKER_SHUTDOWN_TOUT_MS);
}

void worker_runnable(union Worker * const worker)
{
   if (!worker) return;

   union Mailbox * const mailbox = &worker->mailbox;

   Worker_on_start(worker);
   
   while(true)
   {
      Worker_on_loop(worker);

      union Mail mail = {NULL};

      if(IPC_Retrieve_Mail(&mail, 500))
      {
          Worker_on_mail(worker, &mail);
          if(worker->shutdown_mid == mail.mid)
          {
              break;
          }
      }
   }

   Dbg_Warn("shutdown %d\n", worker->id);
   Worker_on_stop(worker);
}

void Worker_populate(union Worker * const worker, union ThreadCbk *  const cbk,
    IPC_TID_T const id, union Mutex * const mux, union Conditional * const cv, 
    union Mailbox * const mailbox, IPC_MID_T const shutdown_mid)
{
  Thread_populate(&worker->Thread, cbk, id, mux, cv);
  worker->mailbox = mailbox;
  worker->shutdown_mid = shutdown_mid;
}
