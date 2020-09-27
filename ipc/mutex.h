#ifndef MUTEX_H_
#define MUTEX_H_

#include "common/ipc_types.h"
#include "cobject/cobject.h"
 
#ifdef __cplusplus
extern "C" {
#endif
union Mutex; 

typedef union Mutex_Cbk
{
      struct Mutex_Cbk_Class _private * _private vtbl;
      union Object Object;
}Mutex_Cbk_T;

typedef struct Mutex_Cbk_Class
{
      struct Class Class;
      bool (* _private lock)(union Mutex_Cbk * const, union Mutex * const, IPC_Clock_T const wait_ms);
      bool (* _private unlock)(union Mutex_Cbk * const, union Mutex * const);
}Mutex_Cbk_Class_T;

typedef union Mutex
{
   struct Mutex_Class _private * _private vtbl;
   struct
   {
      union Object Object;
      union Mutex_Cbk _private * _private cbk;
   };
}Mutex_T;

typedef struct Mutex_Class
{
      struct Class Class;
      bool (* _private lock)(union Mutex * const, IPC_Clock_T const);
      bool (* _private unlock)(union Mutex * const);
}Mutex_Class_T;

extern Mutex_Class_T _private Mutex_Class;
extern struct Mutex_Cbk_Class _private Mutex_Cbk_Class;

extern void Populate_Mutex(union Mutex * const mux);
 
#ifdef __cplusplus
}
#endif
#endif /*MUTEX_H_*/
