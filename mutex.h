#ifndef MUTEX_H_
#define MUTEX_H_

#include "ipc_types.h"
#include "cobject.h"
 
#ifdef __cplusplus
extern "C" {
#endif
 

typedef union Mutex
{
   struct Mutex_Class _private * _private vtbl;
   struct
   {
      struct Object Object;
      void _private * _private mux;
   };
}Mutex_T;

typedef struct Mutex_Class
{
      struct Class Class;
      bool_t (* _private lock)(union Mutex * const, IPC_Clock_T const);
      bool_t (* _private unlock)(union Mutex * const);
}Mutex_Class_T;

extern Mutex_Class_T _private Mutex_Class;

extern void Populate_Mutex(union Mutex * const mux);
 
#ifdef __cplusplus
}
#endif
#endif /*MUTEX_H_*/
