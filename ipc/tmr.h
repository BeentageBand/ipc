#ifndef TMR_H_
#define TMR_H_

#include "common/ipc_types.h"
#include "cobject/cobject.h"

union Timer;

typedef union Timer_Cbk
{
      struct Timer_Cbk_Class _private * _private vtbl;
      union Object Object;
}Timer_Cbk_T;

typedef struct Timer_Cbk_Class
{
      struct Class Class;
      bool (* _private start)(union Timer_Cbk * const, union Timer * const);
      bool (* _private stop)(union Timer_Cbk * const, union Timer * const);
}Timer_Cbk_Class_T;

typedef union Timer
{
    struct Timer_Class _private * _private vtbl;
    struct
    {
        union Object Object;
        IPC_MID_T _private mid;
        IPC_TID_T _private tid;
        union Timer_Cbk _private * _private cbk;
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
extern struct Timer_Cbk_Class _private Timer_Cbk_Class;
extern void Populate_Timer(union Timer * const tmr, IPC_MID_T const mid);

#endif /*TMR_H_*/