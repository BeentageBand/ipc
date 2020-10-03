#ifndef MUXMOCK_INT_H
#define MUXMOCK_INT_H
#define MUXMOCK_IMPLEMENTATION

#include "muxmock.h"

#ifdef __cplusplus
extern "C" {
#endif

static void muxmock_override(union MuxMock_Class * const muxmock);

union MuxMock_Class * Get_MuxMock_Class(void)
{
  static union MuxMock_Class clazz = {};
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), &Get_Mutex_Class()->Class);
  muxmock_override(&clazz);
  return &clazz;
}
bool MuxMock_unlock(union MuxMock * const muxmock)
{
  return muxmock->vtbl->unlock(muxmock);
}

bool MuxMock_lock(union MuxMock * const muxmock, IPC_Clock_T const ms)
{
  return muxmock->vtbl->lock(muxmock, ms);
}


#ifdef __cplusplus
}
#endif
#endif /*MUXMOCK_INT_H*/
