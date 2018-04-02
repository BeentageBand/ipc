#ifndef CONDITIONAL_H_
#define CONDITIONAL_H_
 
#include "mutex.h"
 
#ifdef __cplusplus
extern "C" {
#endif

typedef struct IPC_COND
{
  char _cond[40];
}IPC_COND_T;

typedef union Conditional
{
	struct Conditional_Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		IPC_COND_T _private conditional;
		union Mutex _private * _private mutex;
	};
}Conditional_T;

typedef struct Conditional_Class
{
		struct Class Class;
		bool_t (* _private wait)(union Conditional * const, IPC_Clock_T const);
		bool_t (* _private signal)(union Conditional * const);
}Conditional_Class_T;

extern Conditional_Class_T _private Conditional_Class;

extern void Populate_Conditional(union Conditional * const conditional, union Mutex * const mux);

#ifdef __cplusplus
}
#endif
#endif /*CONDITIONAL_H_*/
