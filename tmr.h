#ifndef TMR_H_
#define TMR_H_

#include "ipc_types.h"
#include "cobject.h"

typedef union Timer
{
    struct Timer_Class _private * _private vtbl;
    struct
    {
        struct Object Object;
        IPC_MID_T _private mid;
        IPC_TID_T _private tid;
        void _private * _private tmr;
        bool _private is_periodic;
        bool _private is_active;
        IPC_Clock_T _private tout_ms;
    };
}Timer_T;

typedef struct Timer_Class
{
    struct Class Class;
    void (*_private start)(union Timer * const);
    void (*_private set_time)(union Timer * const, IPC_Clock_T const, bool const);
    void (*_private reset)(union Timer * const);
    void (*_private stop)(union Timer * const);
    void (*_private on_tout)(union Timer * const);
}Timer_Class_T;

extern struct Timer_Class _private Timer_Class;
extern void Populate_Timer(union Timer * const tmr, IPD_MID_T const mid);

#endif /*TMR_H_*/