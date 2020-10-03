#ifndef MUXMOCK_H
#define MUXMOCK_H
#include "gmock-mux.h"
#include "ipc/mutex/mutex.h"

#ifdef MUXMOCK_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union MuxMock;
union MuxMock_Class
{
    union Mutex_Class Mutex;

    struct
    {
    struct Class Class;
    bool (* _private unlock)(union MuxMock * const muxmock);
bool (* _private lock)(union MuxMock * const muxmock, IPC_Clock_T const ms);

    };
};

union MuxMock
{
    union MuxMock_Class * vtbl;
    union Mutex Mutex;
    struct
    {
      union Object Object;
      mock::GMux * _private mock;

    };
};

extern union MuxMock_Class * Get_MuxMock_Class(void);

extern void MuxMock_populate(union MuxMock * const muxmock, mock::GMux * const mock);

extern bool MuxMock_unlock(union MuxMock * const muxmock);

extern bool MuxMock_lock(union MuxMock * const muxmock, IPC_Clock_T const ms);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*MUXMOCK_H*/