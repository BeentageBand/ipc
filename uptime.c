#define COBJECT_IMPLEMENTATION

#include "uptime.h"

static void uptime_delete(struct Object * const obj);
static void uptime_sleep(union Union * const this);
static IPC_Clock_T uptime_time(union Union * const this);

Uptime_Class_T _private Uptime_Class =
{
	{uptime_delete, NULL},
	
};

