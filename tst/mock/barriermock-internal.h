#ifndef BARRIERMOCK_INT_H
#define BARRIERMOCK_INT_H
#define BARRIERMOCK_IMPLEMENTATION

#include "barriermock.h"

#ifdef __cplusplus
extern "C" {
#endif

static void barriermock_override(union BarrierMock_Class * const barriermock);

union BarrierMock_Class * Get_BarrierMock_Class(void)
{
  static union BarrierMock_Class clazz = {};
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), &Get_Barrier_Class()->Class);
  barriermock_override(&clazz);
  return &clazz;
}
void BarrierMock_wait(union BarrierMock * const barriermock, IPC_Clock_T const ms)
{
  return barriermock->vtbl->wait(barriermock, ms);
}

void BarrierMock_ready(union BarrierMock * const barriermock)
{
  return barriermock->vtbl->ready(barriermock);
}


#ifdef __cplusplus
}
#endif
#endif /*BARRIERMOCK_INT_H*/
