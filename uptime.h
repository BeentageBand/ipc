#ifndef UPTIME_H_
#define UPTIME_H_

#include "ipc_types.h"
#include "cobject.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef union Uptime
{
	union Uptime_Class _private * _private vtbl;
	struct
	{
		struct Object Object;
	};
}Uptime_T;

typedef union Uptime_Class
{
	struct
	{
		struct Class Class;
		void (* _private sleep)(union Uptime * const);
		IPC_Clock_T (* _private time)(union Uptime * const);
	};
}Uptime_Class_T;

extern Uptime_Class_T _private Uptime_Class;

extern void Populate_Uptime(union Uptime * const uptime);

#ifdef __cplusplus
}
#endif
#endif /*UPTIME_H_*/
