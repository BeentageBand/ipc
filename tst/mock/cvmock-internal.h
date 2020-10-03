#ifndef CVMOCK_INT_H
#define CVMOCK_INT_H
#define CVMOCK_IMPLEMENTATION

#include "cvmock.h"

#ifdef __cplusplus
extern "C" {
#endif

static void cvmock_override(union CVMock_Class * const cvmock);

union CVMock_Class * Get_CVMock_Class(void)
{
  static union CVMock_Class clazz = {};
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), &Get_Conditional_Class()->Class);
  cvmock_override(&clazz);
  return &clazz;
}
bool CVMock_signal(union CVMock * const cvmock)
{
  return cvmock->vtbl->signal(cvmock);
}

bool CVMock_wait(union CVMock * const cvmock, IPC_Clock_T const ms)
{
  return cvmock->vtbl->wait(cvmock, ms);
}


#ifdef __cplusplus
}
#endif
#endif /*CVMOCK_INT_H*/
