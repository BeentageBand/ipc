#ifndef CVMOCK_H
#define CVMOCK_H
#include "gmock-cv.h"
#include "ipc/conditional/conditional.h"
#include "tst/mock/cvmock.h"

#ifdef CVMOCK_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union CVMock;
union CVMock_Class
{
    union Conditional_Class Conditional;

    struct
    {
    struct Class Class;
    bool (* _private signal)(union CVMock * const cvmock);
bool (* _private wait)(union CVMock * const cvmock, IPC_Clock_T const ms);

    };
};

union CVMock
{
    union CVMock_Class * vtbl;
    union Conditional Conditional;
    struct
    {
      union Object Object;
      union Mutex * _private mutex;
mock::GCV * _private mock;

    };
};

extern union CVMock_Class * Get_CVMock_Class(void);

extern void CVMock_populate(union CVMock * const cvmock, union Mutex * const mutex, mock::GCV * const mock);

extern bool CVMock_signal(union CVMock * const cvmock);

extern bool CVMock_wait(union CVMock * const cvmock, IPC_Clock_T const ms);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*CVMOCK_H*/