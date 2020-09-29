#ifndef CONDITIONAL_H
#define CONDITIONAL_H
#include "ipc/mutex/mutex.h"
#include "ipc/common/ipc_types.h"

#ifdef CONDITIONAL_IMPLEMENTATION 
#define conditional_private
#else
#define conditional_private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union Conditional;
union Conditional_Class
{
    
    struct
    {
    struct Class Class;
    bool (* conditional_private signal)(union Conditional * const conditional);
bool (* conditional_private wait)(union Conditional * const conditional, IPC_Clock_T const ms);

    };
};

union Conditional
{
    union Conditional_Class * vtbl;
        struct
    {
      union Object Object;
      union Mutex * conditional_private mutex;

    };
};

extern union Conditional_Class * Get_Conditional_Class(void);

extern void Conditional_populate(union Conditional * const conditional, union Mutex * const mutex);

extern bool Conditional_signal(union Conditional * const conditional);

extern bool Conditional_wait(union Conditional * const conditional, IPC_Clock_T const ms);

#ifdef __cplusplus
}
#endif
#undef conditional_private
#endif /*CONDITIONAL_H*/