#ifndef MUTEX_H_
#define MUTEX_H_

#include "cobject.h"
 
#ifdef __cplusplus
extern "C" {
#endif
 
typedef uitn32_t IPC_MUX_T;

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
		bool_t (* _private lock)(union Mutex * const);
		bool_t (* _private unlock)(union Mutex * const);
	};
}Mutex_Class_T;

extern Mutex_Class_T _private Mutex_Class;

extern void Populate_Mutex(union Mutex * const mux);
 
#ifdef __cplusplus
}
#endif
#endif /*MUTEX_H_*/
