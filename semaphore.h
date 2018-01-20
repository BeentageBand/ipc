#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "ipc_types.h"
#include "cobject.h"

typedef union Semaphore
{
	union Semaphore_Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		uint32_t value;
	};
}Semaphore_T;

typedef union Semaphore_Class
{
	struct
	{
		struct Class Class;
		bool (* _private wait)(union Semaphore * const, IPC_Clock_T const);
		bool (* _private post)(union Semaphore * const);
	};
}Semaphore_Class_T;


extern void Populate_Semaphore(union Semaphore * const sem, uint32_t value);

#endif /*SEMAPHORE_H_*/
