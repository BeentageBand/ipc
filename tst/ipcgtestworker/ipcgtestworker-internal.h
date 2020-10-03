#ifndef IPCGTESTWORKER_INT_H
#define IPCGTESTWORKER_INT_H
#define IPCGTESTWORKER_IMPLEMENTATION

#include "ipcgtestworker.h"

#ifdef __cplusplus
extern "C" {
#endif

static void ipcgtestworker_override(union IPCGTestWorker_Class * const ipcgtestworker);

union IPCGTestWorker_Class * Get_IPCGTestWorker_Class(void)
{
  static union IPCGTestWorker_Class clazz = {};
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), &Get_Worker_Class()->Class);
  ipcgtestworker_override(&clazz);
  return &clazz;
}
void IPCGTestWorker_run(union IPCGTestWorker * const ipcgtestworker)
{
  return ipcgtestworker->vtbl->run(ipcgtestworker);
}

void IPCGTestWorker_ready(union IPCGTestWorker * const ipcgtestworker)
{
  return ipcgtestworker->vtbl->ready(ipcgtestworker);
}

void IPCGTestWorker_wait(union IPCGTestWorker * const ipcgtestworker, IPC_Clock_T const wait_ms)
{
  return ipcgtestworker->vtbl->wait(ipcgtestworker, wait_ms);
}

bool IPCGTestWorker_joinable(union IPCGTestWorker * const ipcgtestworker)
{
  return ipcgtestworker->vtbl->joinable(ipcgtestworker);
}

void IPCGTestWorker_join(union IPCGTestWorker * const ipcgtestworker, IPC_Clock_T const wait_ms)
{
  return ipcgtestworker->vtbl->join(ipcgtestworker, wait_ms);
}

void IPCGTestWorker_runnable(union IPCGTestWorker * const ipcgtestworker)
{
  return ipcgtestworker->vtbl->runnable(ipcgtestworker);
}

void IPCGTestWorker_on_loop(union IPCGTestWorker * const ipcgtestworker)
{
  return ipcgtestworker->vtbl->on_loop(ipcgtestworker);
}

void IPCGTestWorker_on_start(union IPCGTestWorker * const ipcgtestworker)
{
  return ipcgtestworker->vtbl->on_start(ipcgtestworker);
}

void IPCGTestWorker_on_stop(union IPCGTestWorker * const ipcgtestworker)
{
  return ipcgtestworker->vtbl->on_stop(ipcgtestworker);
}

void IPCGTestWorker_on_mail(union IPCGTestWorker * const ipcgtestworker, union Mail * const mail)
{
  return ipcgtestworker->vtbl->on_mail(ipcgtestworker, mail);
}


#ifdef __cplusplus
}
#endif
#endif /*IPCGTESTWORKER_INT_H*/
