#ifndef MUTEX_INT_H
#define MUTEX_INT_H
#define MUTEX_IMPLEMENTATION

#include "mutex.h"

static void mutex_override(union Mutex_Class * const mutex);

union Mutex_Class * Get_Mutex_Class(void)
{
  static union Mutex_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  mutex_override(&clazz);
  return &clazz;
}
bool Mutex_unlock(union Mutex * const mutex)
{
  return mutex->vtbl->unlock(mutex);
}

bool Mutex_lock(union Mutex * const mutex, IPC_Clock_T const ms)
{
  return mutex->vtbl->lock(mutex, ms);
}


#endif /*MUTEX_INT_H*/
