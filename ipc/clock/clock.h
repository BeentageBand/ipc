#ifndef CLOCK_H
#define CLOCK_H
#include "ipc/common/ipc_types.h"
#include "ipc/factory/ipcfactory.h"
#include "cobject/cobject.h"

#ifdef CLOCK_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union Clock;
union Clock_Class
{
    
    struct
    {
    struct Class Class;
    IPC_Clock_T (* _private clock)(union Clock * const clock);
bool (* _private is_clock_elapsed)(union Clock * const clock, IPC_Clock_T const clock_ms);
void (* _private sleep)(union Clock * const clock, IPC_Clock_T const ms);

    };
};

union Clock
{
    union Clock_Class * vtbl;
        struct
    {
      union Object Object;
      union IPCFactory * _private factory;

    };
};

extern union Clock_Class * Get_Clock_Class(void);

extern void Clock_populate(union Clock * const clock, union IPCFactory * const factory);

extern IPC_Clock_T Clock_clock(union Clock * const clock);

extern bool Clock_is_clock_elapsed(union Clock * const clock, IPC_Clock_T const clock_ms);

extern void Clock_sleep(union Clock * const clock, IPC_Clock_T const ms);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*CLOCK_H*/