#ifndef BARRIERMOCK_H
#define BARRIERMOCK_H
#include "gmock-barrier.h"
#include "ipc/barrier/barrier.h"

#ifdef BARRIERMOCK_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union BarrierMock;
union BarrierMock_Class
{
    union Barrier_Class Barrier;

    struct
    {
    struct Class Class;
    void (* _private wait)(union BarrierMock * const barriermock, IPC_Clock_T const ms);
void (* _private ready)(union BarrierMock * const barriermock);

    };
};

union BarrierMock
{
    union BarrierMock_Class * vtbl;
    union Barrier Barrier;
    struct
    {
      union Object Object;
      mock::GBarrier * _private mock;

    };
};

extern union BarrierMock_Class * Get_BarrierMock_Class(void);

extern void BarrierMock_populate(union BarrierMock * const barriermock, mock::GBarrier * const mock);

extern void BarrierMock_wait(union BarrierMock * const barriermock, IPC_Clock_T const ms);

extern void BarrierMock_ready(union BarrierMock * const barriermock);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*BARRIERMOCK_H*/