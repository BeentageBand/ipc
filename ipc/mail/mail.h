#ifndef MAIL_H
#define MAIL_H
#include "cobject/cobject.h"
#include "ipc/common/ipc_types.h"

#ifdef MAIL_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union Mail;
union Mail_Class
{
    
    struct
    {
    struct Class Class;
    void (* _private dump)(union Mail * const mail);
void (* _private set_payload)(union Mail * const mail, void * const data, size_t const data_size);
void (* _private set_mid)(union Mail * const mail, IPC_MID_T const mid);
void (* _private set_sender)(union Mail * const mail, IPC_TID_T const sender);
void (* _private set_receiver)(union Mail * const mail, IPC_TID_T const receiver);

    };
};

union Mail
{
    union Mail_Class * vtbl;
        struct
    {
      union Object Object;
      IPC_MID_T _private mid;
IPC_TID_T _private sender;
IPC_TID_T _private receiver;
void * _private payload;
size_t _private pay_size;

    };
};

extern union Mail_Class * Get_Mail_Class(void);

extern void Mail_populate(union Mail * const mail, IPC_MID_T const mid, IPC_TID_T const sender, IPC_TID_T const receiver, void * const payload, size_t const pay_size);

extern void Mail_dump(union Mail * const mail);

extern void Mail_set_payload(union Mail * const mail, void * const data, size_t const data_size);

extern void Mail_set_mid(union Mail * const mail, IPC_MID_T const mid);

extern void Mail_set_sender(union Mail * const mail, IPC_TID_T const sender);

extern void Mail_set_receiver(union Mail * const mail, IPC_TID_T const receiver);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*MAIL_H*/