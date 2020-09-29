#include "logger/logger.h"
#include "mutex-internal.h"

static void mutex_delete(union Mutex * const mutex);
static bool mutex_lock(union Mutex * const mutex, IPC_Clock_T const wait_ms);
static bool mutex_unlock(union Mutex * const mutex);

void mutex_override(union Mutex_Class * const clazz)
{
  clazz->Class.destroy = (Class_Delete_T) mutex_delete;
}

void mutex_delete(union Mutex * const mutex)
{}

void Mutex_populate(union Mutex * const mutex)
{
  Object_populate(&mutex->Object, &Get_Mutex_Class()->Class);
}
