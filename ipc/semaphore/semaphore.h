#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "cobject/cobject.h"
#include "ipc/common/ipc_types.h"

#ifdef SEMAPHORE_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union Semaphore;
union Semaphore_Class
{
    
    struct
    {
      struct Class Class;
      bool (* _private wait)(union Semaphore * const semaphore, IPC_Clock_T const ms);
      bool (* _private post)(union Semaphore * const semaphore);

    };
};

union Semaphore
{
    union Semaphore_Class * vtbl;
        struct
    {
      union Object Object;
      
    };
};

extern union Semaphore_Class * Get_Semaphore_Class(void);

extern void Semaphore_populate(union Semaphore * const semaphore);

extern bool Semaphore_wait(union Semaphore * const semaphore, IPC_Clock_T const ms);

extern bool Semaphore_post(union Semaphore * const semaphore);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*SEMAPHORE_H*/