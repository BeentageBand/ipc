#ifndef THREAD_H
#define THREAD_H
#include "threadcbk.h"
#include "ipc/barrier/barrier.h"
#include "ipc/common/ipc_types.h"

#ifdef THREAD_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union Thread;
union Thread_Class
{
    
    struct
    {
    struct Class Class;
    void (* _private run)(union Thread * const thread);
void (* _private ready)(union Thread * const thread);
void (* _private wait)(union Thread * const thread, IPC_Clock_T const wait_ms);
bool (* _private joinable)(union Thread * const thread);
void (* _private join)(union Thread * const thread, IPC_Clock_T const wait_ms);
void (* _private runnable)(union Thread * const thread);

    };
};

union Thread
{
    union Thread_Class * vtbl;
        struct
    {
      union Object Object;
      union ThreadCbk *  _private cbk;
bool _private ready;
IPC_TID_T _private id;
union Barrier * _private barrier;

    };
};

extern union Thread_Class * Get_Thread_Class(void);

extern void Thread_populate(union Thread * const thread, union ThreadCbk *  const cbk, IPC_TID_T const id, union Barrier * const barrier);

extern void Thread_run(union Thread * const thread);

extern void Thread_ready(union Thread * const thread);

extern void Thread_wait(union Thread * const thread, IPC_Clock_T const wait_ms);

extern bool Thread_joinable(union Thread * const thread);

extern void Thread_join(union Thread * const thread, IPC_Clock_T const wait_ms);

extern void Thread_runnable(union Thread * const thread);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*THREAD_H*/