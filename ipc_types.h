#ifndef IPC_TYPES_H_
#define IPC_TYPES_H_

#include "ipc_mailist_ids.h"
#include "ipc_set.h"
#include "ipc_thread_ids.h"
#include "std_reuse.h"

#define IPC_ENUM(id, desc) id,
#define IPC_CLOCK_MS_SEC (1000UL)
#define IPC_CLOCK_NS_MS (1000000UL)

#ifdef __cplusplus
extern "C" {
#endif

enum IPC_MID
{
   IPC_PBC_BEGIN_MID = 0,
   IPC_PUBLIC_MAILIST(IPC_ENUM)
   IPC_PBC_END_MID,
   IPC_INT_BEGIN_MID = IPC_PBC_END_MID,
   IPC_INTERNAL_MAILIST(IPC_ENUM)
   IPC_INT_END_MID,
   IPC_MAX_MID = IPC_INT_END_MID
};
typedef uint32_t IPC_MID_T;

enum IPC_TID
{
   IPC_MAIN_TID = 0,
   IPC_THREAD_LIST(IPC_ENUM)
   IPC_MAX_TID
};

typedef uint32_t IPC_TID_T;

typedef uint32_t IPC_Clock_T;

#ifdef __cplusplus
}
#endif
#endif /*IPC_TYPES_H_*/
