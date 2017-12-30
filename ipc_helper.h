#ifndef IPC_HELPER_H_
#define IPC_HELPER_H_
 
#include "ipc_types.h"
#include "mailbox.h"
#include "publisher.h"
#include "thread.h"
#include "uptime.h"
 
#ifdef __cplusplus
extern "C" {
#endif

typedef union Thread * Thread_Ptr_T;

#define CSet_Params Thread_Ptr
#include "cset.h"
#undef CSet_Params 

typedef union Mailbox * Mailbox_Ptr_T;

#define CSet_Params Mailbox_Ptr
#include "cset.h"
#undef CSet_Params 
 
typedef union IPC_Helper
{
	struct Class _private * _private vtbl;
	struct
	{
		struct Object Object;
		CSet_Thread_Ptr_T _private rthreads;
		CSet_Mailbox_Ptr_T _private rmailboxes;
		union Uptime _private uptime;
	};
}IPC_Helper_T;

typedef struct Class IPC_Helper_Class_T;

extern IPC_Helper_Class_T _private IPC_Class;

extern union IPC_Helper * IPC_get_instance(void);
 
#ifdef __cplusplus
}
#endif
#endif /*IPC_HELPER_H_*/
