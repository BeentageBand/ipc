#ifndef IPCGTESTWORKER_H
#define IPCGTESTWORKER_H
#include "ipc/worker/worker.h"

#ifdef IPCGTESTWORKER_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union IPCGTestWorker;
union IPCGTestWorker_Class
{
    union Worker_Class Worker;

    struct
    {
    struct Class Class;
    void (* _private run)(union IPCGTestWorker * const ipcgtestworker);
void (* _private ready)(union IPCGTestWorker * const ipcgtestworker);
void (* _private wait)(union IPCGTestWorker * const ipcgtestworker, IPC_Clock_T const wait_ms);
bool (* _private joinable)(union IPCGTestWorker * const ipcgtestworker);
void (* _private join)(union IPCGTestWorker * const ipcgtestworker, IPC_Clock_T const wait_ms);
void (* _private runnable)(union IPCGTestWorker * const ipcgtestworker);
void (* _private on_loop)(union IPCGTestWorker * const ipcgtestworker);
void (* _private on_start)(union IPCGTestWorker * const ipcgtestworker);
void (* _private on_stop)(union IPCGTestWorker * const ipcgtestworker);
void (* _private on_mail)(union IPCGTestWorker * const ipcgtestworker, union Mail * const mail);

    };
};

union IPCGTestWorker
{
    union IPCGTestWorker_Class * vtbl;
    union Worker Worker;
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

extern union IPCGTestWorker_Class * Get_IPCGTestWorker_Class(void);

extern void IPCGTestWorker_populate(union IPCGTestWorker * const ipcgtestworker, union ThreadCbk *  const cbk, bool const ready, IPC_TID_T const id, union Mutex * const mux, union Conditional * const cv, union Mailbox * const mailbox, IPC_MID_T const shutdown_mid);

extern void IPCGTestWorker_run(union IPCGTestWorker * const ipcgtestworker);

extern void IPCGTestWorker_ready(union IPCGTestWorker * const ipcgtestworker);

extern void IPCGTestWorker_wait(union IPCGTestWorker * const ipcgtestworker, IPC_Clock_T const wait_ms);

extern bool IPCGTestWorker_joinable(union IPCGTestWorker * const ipcgtestworker);

extern void IPCGTestWorker_join(union IPCGTestWorker * const ipcgtestworker, IPC_Clock_T const wait_ms);

extern void IPCGTestWorker_runnable(union IPCGTestWorker * const ipcgtestworker);

extern void IPCGTestWorker_on_loop(union IPCGTestWorker * const ipcgtestworker);

extern void IPCGTestWorker_on_start(union IPCGTestWorker * const ipcgtestworker);

extern void IPCGTestWorker_on_stop(union IPCGTestWorker * const ipcgtestworker);

extern void IPCGTestWorker_on_mail(union IPCGTestWorker * const ipcgtestworker, union Mail * const mail);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*IPCGTESTWORKER_H*/