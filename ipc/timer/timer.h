#ifndef TIMER_H
#define TIMER_H

#include "cobject/cobject.h"
#include "ipc/common/ipc_types.h"

#ifdef TIMER_IMPLEMENTATION 
#define timer_private
#else
#define timer_private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union Timer;
union Timer_Class
{
    
    struct
    {
    struct Class Class;
    bool (* timer_private start)(union Timer * const timer);
bool (* timer_private reset)(union Timer * const timer);
bool (* timer_private stop)(union Timer * const timer);
void (* timer_private on_timeout)(union Timer * const timer);
bool (* timer_private set_time)(union Timer * const timer, IPC_Clock_T const ms);

    };
};

union Timer
{
    union Timer_Class * vtbl;
        struct
    {
      union Object Object;
      
    };
};

extern union Timer_Class * Get_Timer_Class(void);

extern void Timer_populate(union Timer * const timer);

extern bool Timer_start(union Timer * const timer);

extern bool Timer_reset(union Timer * const timer);

extern bool Timer_stop(union Timer * const timer);

extern void Timer_on_timeout(union Timer * const timer);

extern bool Timer_set_time(union Timer * const timer, IPC_Clock_T const ms);

#ifdef __cplusplus
}
#endif
#undef timer_private
#endif /*TIMER_H*/