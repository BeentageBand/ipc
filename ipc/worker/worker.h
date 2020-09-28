#ifndef WORKER_H
#define WORKER_H
#include "ipc/mailbox/mailbox.h"
#include "ipc/thread/thread.h"

#ifdef WORKER_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union Worker;
union Worker_Class
{
    union Thread_Class Thread;

    struct
    {
    struct Class Class;
    void (* _private run)(union Worker * const worker);
void (* _private ready)(union Worker * const worker);
void (* _private wait)(union Worker * const worker, IPC_Clock_T const wait_ms);
bool (* _private joinable)(union Worker * const worker);
void (* _private join)(union Worker * const worker, IPC_Clock_T const wait_ms);
void (* _private runnable)(union Worker * const worker);
void (* _private on_loop)(union Worker * const worker);
void (* _private on_start)(union Worker * const worker);
void (* _private on_stop)(union Worker * const worker);
void (* _private on_mail)(union Worker * const worker, union Mail * const mail);

    };
};

union Worker
{
    union Worker_Class * vtbl;
    union Thread Thread;
    struct
    {
      union Object Object;
      union ThreadCbk *  _private cbk;
bool _private ready;
IPC_TID_T _private id;
union Mutex * _private mux;
union Conditional * _private cv;
union Mailbox * _private mailbox;
IPC_MID_T _private shutdown_mid;

    };
};

extern union Worker_Class * Get_Worker_Class(void);

extern void Worker_populate(union Worker * const worker, union ThreadCbk *  const cbk, bool const ready, IPC_TID_T const id, union Mutex * const mux, union Conditional * const cv, union Mailbox * const mailbox, IPC_MID_T const shutdown_mid);

extern void Worker_run(union Worker * const worker);

extern void Worker_ready(union Worker * const worker);

extern void Worker_wait(union Worker * const worker, IPC_Clock_T const wait_ms);

extern bool Worker_joinable(union Worker * const worker);

extern void Worker_join(union Worker * const worker, IPC_Clock_T const wait_ms);

extern void Worker_runnable(union Worker * const worker);

extern void Worker_on_loop(union Worker * const worker);

extern void Worker_on_start(union Worker * const worker);

extern void Worker_on_stop(union Worker * const worker);

extern void Worker_on_mail(union Worker * const worker, union Mail * const mail);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*WORKER_H*/