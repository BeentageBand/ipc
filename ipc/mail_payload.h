#ifndef MAIL_PAYLOAD_H_
#define MAIL_PAYLOAD_H_

#include "common/ipc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern size_t Mail_Payload_Alloc(void ** const payload, size_t const pay_size);

extern size_t Mail_Payload_Free(void ** const payload);

#ifdef __cplusplus
}
#endif

#endif /*MAIL_PAYLOAD_H_*/
