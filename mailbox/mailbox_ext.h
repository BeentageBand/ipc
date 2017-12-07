#ifndef MAILBOX_EXT_H_
#define MAILBOX_EXT_H_
 
#include "ipc_types.h"
#include "mail.h"
#include "cqueue.h"
#include "mailbox_ext.h"

#ifdef __cplusplus
extern "C" {
#endif
 
extern bool_t Mailbox_lock(union Mailbox * const mbx);
extern bool_t Mailbox_unlock(union Mailbox * const mbx);
extern bool_t Mailbox_wait(union Mailbox * const mbx, uint32_t const wait_ms);
extern bool_t Mailbox_signal(union Mailbox * const mbx);

#ifdef __cplusplus
}
#endif
#endif /*MAILBOX_EXT_H_*/
