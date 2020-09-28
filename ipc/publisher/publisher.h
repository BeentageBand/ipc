#ifndef PUBLISHER_H_
#define PUBLISHER_H_

#include "ipc/common/ipc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bool Publisher_Subscribe(IPC_TID_T const mbx, IPC_MID_T const mid);

extern bool Publisher_Unsubscribe(IPC_TID_T const mbx, IPC_MID_T const mid);

extern void Publisher_Publish(IPC_MID_T const mid, void const * const payload, size_t const pay_size);
 
#ifdef __cplusplus
}
#endif
 
#endif /*PUBLISHER_H_*/
