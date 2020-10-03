#ifndef BARRIER_H
#define BARRIER_H
#include "ipc/common/ipc_types.h"
#include "cobject/cobject.h"

#ifdef BARRIER_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union Barrier;
union Barrier_Class
{
    
    struct
    {
    struct Class Class;
    void (* _private wait)(union Barrier * const barrier, IPC_Clock_T const ms);
void (* _private ready)(union Barrier * const barrier);

    };
};

union Barrier
{
    union Barrier_Class * vtbl;
        struct
    {
      union Object Object;
      
    };
};

extern union Barrier_Class * Get_Barrier_Class(void);

extern void Barrier_populate(union Barrier * const barrier);

extern void Barrier_wait(union Barrier * const barrier, IPC_Clock_T const ms);

extern void Barrier_ready(union Barrier * const barrier);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*BARRIER_H*/