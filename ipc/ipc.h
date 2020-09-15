#ifndef IPC_H_
#define IPC_H_

#include "ipc_types.h"
#include "mailbox.h"
#include "thread.h"

#define IPC_Send_Self(mid, payload, pay_size) IPC_Send(IPC_Self(), mid, payload, pay_size)

#ifdef __cplusplus
extern "C" {
#endif
/**
 * IPC Task Handling
 * This apis should be used in task running
 */
extern IPC_TID_T IPC_Self(void);

extern void IPC_Ready(void);

extern void IPC_Wait(IPC_TID_T const tid, IPC_Clock_T const wait_ms);

extern void IPC_Run(IPC_TID_T const tid);

/**
 * IPC Mail Comm
 */
extern bool IPC_Subscribe_Mailist(IPC_MID_T const * const mailist, uint32_t const mailist_size);

extern bool IPC_Unsubscribe_Mailist(IPC_MID_T const * const mailist, uint32_t const mailist_size);

extern bool IPC_Retrieve_Mail(union Mail * const mail, IPC_Clock_T const wait_ms);

extern bool IPC_Retrieve_From_Mailist(union Mail * const mail, IPC_Clock_T const wait_ms, IPC_MID_T const * const mailist,
      uint32_t const mailist_size);

extern void IPC_Send(IPC_TID_T const rcv_tid, IPC_MID_T const mid, void const * const payload, size_t const pay_size);

/**
 * IPC Miscellaneous
 */
extern void IPC_Publish(IPC_MID_T const mid, void const * const payload, size_t const pay_size);

extern IPC_Clock_T IPC_Clock(void);

extern bool IPC_Clock_Elapsed(IPC_Clock_T const clock_ms);

extern void IPC_Sleep(IPC_Clock_T const ms);

#ifdef __cplusplus
}
#endif

#endif /*IPC_H_*/
