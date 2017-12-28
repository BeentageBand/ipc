#ifndef MAIL_PAYLOAD_H_
#define MAIL_PAYLOAD_H_


#include "ipc_types.h"

extern size_t Mail_Payload_Alloc(void ** const payload, size_t const pay_size);

extern size_t Mail_Payload_Free(void ** const payload);

#endif /*MAIL_PAYLOAD_H_*/
