#ifndef MUTEX_H
#define MUTEX_H
#include "cobject/cobject.h"
#include "ipc/common/ipc_types.h"

#ifdef MUTEX_IMPLEMENTATION 
#define mutex_private
#else
#define mutex_private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union Mutex;
union Mutex_Class
{
    
    struct
    {
    struct Class Class;
    bool (* mutex_private unlock)(union Mutex * const mutex);
bool (* mutex_private lock)(union Mutex * const mutex, IPC_Clock_T const ms);

    };
};

union Mutex
{
    union Mutex_Class * vtbl;
        struct
    {
      union Object Object;
      
    };
};

extern union Mutex_Class * Get_Mutex_Class(void);

extern void Mutex_populate(union Mutex * const mutex);

extern bool Mutex_unlock(union Mutex * const mutex);

extern bool Mutex_lock(union Mutex * const mutex, IPC_Clock_T const ms);

#ifdef __cplusplus
}
#endif
#undef mutex_private
#endif /*MUTEX_H*/