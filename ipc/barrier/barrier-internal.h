#ifndef BARRIER_INT_H
#define BARRIER_INT_H
#define BARRIER_IMPLEMENTATION

#include "barrier.h"

#ifdef __cplusplus
extern "C" {
#endif

static void barrier_override(union Barrier_Class * const barrier);

union Barrier_Class * Get_Barrier_Class(void)
{
  static union Barrier_Class clazz = {};
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  barrier_override(&clazz);
  return &clazz;
}
void Barrier_wait(union Barrier * const barrier, IPC_Clock_T const ms)
{
  return barrier->vtbl->wait(barrier, ms);
}

void Barrier_ready(union Barrier * const barrier)
{
  return barrier->vtbl->ready(barrier);
}


#ifdef __cplusplus
}
#endif
#endif /*BARRIER_INT_H*/
