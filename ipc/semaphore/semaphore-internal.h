#ifndef SEMAPHORE_INT_H
#define SEMAPHORE_INT_H
#define SEMAPHORE_IMPLEMENTATION

#include "semaphore.h"

static void semaphore_override(union Semaphore_Class * const semaphore);

union Semaphore_Class * Get_Semaphore_Class(void)
{
  static union Semaphore_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  semaphore_override(&clazz);
  return &clazz;
}
bool Semaphore_wait(union Semaphore * const semaphore, IPC_Clock_T const ms)
{
  return semaphore->vtbl->wait(semaphore, ms);
}

bool Semaphore_post(union Semaphore * const semaphore)
{
  return semaphore->vtbl->post(semaphore);
}


#endif /*SEMAPHORE_INT_H*/
