#ifndef MUTEX_H_
#define MUTEX_H_

#include "ipc_types.h"
#include "cobject.h"
 
#ifdef __cplusplus
extern "C" {
#endif
 
typedef struct
{
        char _mux[40];
}IPC_MUX_T;

typedef union Mutex
{
	union Mutex_Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		IPC_MUX_T _private mux;
	};
}Mutex_T;

typedef union Mutex_Class
{
	struct
	{
		struct Class Class;
		bool_t (* _private lock)(union Mutex * const, IPC_Clock_T const);
		bool_t (* _private unlock)(union Mutex * const);
	};
}Mutex_Class_T;

extern Mutex_Class_T _private Mutex_Class;

extern void Populate_Mutex(union Mutex * const mux);
 
#ifdef __cplusplus
}
#endif
#endif /*MUTEX_H_*/
