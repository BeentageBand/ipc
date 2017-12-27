#ifndef IPC_TYPES_H_
#define IPC_TYPES_H_

#include "ipc_mailist_ids.h"
#include "ipc_thread_ids.h"

#define IPC_ENUM(id, desc) id,

#ifdef __cplusplus
extern "C" {
#endif
enum IPC_MID
{
	IPC_BCT_BEGIN = 0,
	IPC_BROADCAST_MAILIST(IPC_ENUM)
	IPC_BCT_END = 0,
	IPC_PBC_BEGIN = 0,
	IPC_PUBLIC_MAILIST(IPC_ENUM)
	IPC_PBC_END = 0,
	IPC_INT_BEGIN = 0,
	IPC_INTERNAL_MAILIST(IPC_ENUM)
	IPC_INT_END,
	IPC_MAX_MIDS = IPC_INT_END
};
typedef uint32_t IPC_MID_T;

enum IPC_TID
{
	IPC_TID_MAIN = 0,
	IPC_THREAD_LIST(IPC_ENUM)
	IPC_MAX_TIDS
};

typedef uint32_t IPC_TID_T;

typedef uint32_t IPC_Clock_T;

#ifdef __cplusplus
}
#endif
#endif /*IPC_TYPES_H_*/
