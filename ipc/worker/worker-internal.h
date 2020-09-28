#ifndef WORKER_INT_H
#define WORKER_INT_H
#define WORKER_IMPLEMENTATION

#include "worker.h"

static void worker_override(union Worker_Class * const worker);

union Worker_Class * Get_Worker_Class(void)
{
  static union Worker_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), &Get_Thread_Class()->Class);
  worker_override(&clazz);
  return &clazz;
}
void Worker_run(union Worker * const worker)
{
  return worker->vtbl->run(worker);
}

void Worker_ready(union Worker * const worker)
{
  return worker->vtbl->ready(worker);
}

void Worker_wait(union Worker * const worker, IPC_Clock_T const wait_ms)
{
  return worker->vtbl->wait(worker, wait_ms);
}

bool Worker_joinable(union Worker * const worker)
{
  return worker->vtbl->joinable(worker);
}

void Worker_join(union Worker * const worker, IPC_Clock_T const wait_ms)
{
  return worker->vtbl->join(worker, wait_ms);
}

void Worker_runnable(union Worker * const worker)
{
  return worker->vtbl->runnable(worker);
}

void Worker_on_loop(union Worker * const worker)
{
  return worker->vtbl->on_loop(worker);
}

void Worker_on_start(union Worker * const worker)
{
  return worker->vtbl->on_start(worker);
}

void Worker_on_stop(union Worker * const worker)
{
  return worker->vtbl->on_stop(worker);
}

void Worker_on_mail(union Worker * const worker, union Mail * const mail)
{
  return worker->vtbl->on_mail(worker, mail);
}


#endif /*WORKER_INT_H*/
