#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "ipc_types.h"
#include "cobject.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
        char _mux[40];
}IPC_SEM_T;

typedef union Semaphore
{
   struct Semaphore_Class _private * _private vtbl;
   struct
   {
      struct Object Object;
      IPC_SEM_T sem;
   };
}Semaphore_T;

typedef struct Semaphore_Class
{
        struct Class Class;
        bool (* _private wait)(union Semaphore * const, IPC_Clock_T const);
        bool (* _private post)(union Semaphore * const);
}Semaphore_Class_T;

extern struct Semaphore_Class _private Semaphore_Class;

extern void Populate_Semaphore(union Semaphore * const sem, uint32_t value);

#ifdef __cplusplus
}
#endif
#endif /*SEMAPHORE_H_*/
