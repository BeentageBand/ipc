#ifndef PUBLISHER_H_
#define PUBLISHER_H_

#include "ipc_types.h"

#ifdef __cplusplus
extern "C" {
#endif
extern bool Publisher_Subscribe(union Mailbox * const mbx, IPC_MID_T const mid);

extern bool Publisher_Unsubscribe(union Mailbox * const mbx, IPC_MID_T const mid);

extern void Publisher_Publish(union Mail * const mail);
 
#ifdef __cplusplus
}
#endif
 
#endif /*PUBLISHER_H_*/
