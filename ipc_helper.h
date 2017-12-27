#ifndef IPC_HELPER_H_
#define IPC_HELPER_H_
 
#include "ipc_types.h"
#include "mailbox.h"
#include "publisher.h"
#include "thread.h"
 
#ifdef __cplusplus
extern "C" {
#endif
 
typedef union IPC_Helper
{
	struct Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		union CMap_Task_Ptr _private tasks;
		union Timestamp _private timestamp;
	};
}IPC_Helper_T;

typedef struct Class IPC_Helper_Class_T;

extern IPC_Helper_Class_T _private IPC_Class;

extern union IPC_Helper * IPC_get_instance(void);
 
#ifdef __cplusplus
}
#endif
#endif /*IPC_HELPER_H_*/
