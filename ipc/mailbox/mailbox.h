#ifndef MAILBOX_H
#define MAILBOX_H
#include "ipc/common/ipc_types.h"
#include "ipc/conditional/conditional.h"
#include "ipc/mail/mail.h"
#include "ipc/mutex/mutex.h"

#ifdef MAILBOX_IMPLEMENTATION 
#define mailbox_private
#else
#define mailbox_private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

typedef union Mail Mail_T;

#define CQueue_Params Mail_T
#include "ctemplate-lib/queue/cqueue-template.h"
#undef CQueue_Params


union Mailbox;
union Mailbox_Class
{
    
    struct
    {
    struct Class Class;
    void (* mailbox_private push_mail)(union Mailbox * const mailbox, union Mail * const mail);
bool (* mailbox_private retrieve)(union Mailbox * const mailbox, union Mail * const mail);
bool (* mailbox_private retrieve_only)(union Mailbox * const mailbox, union Mail * const mail, IPC_MID_T const mid);

    };
};

union Mailbox
{
    union Mailbox_Class * vtbl;
        struct
    {
      union Object Object;
      IPC_TID_T mailbox_private tid;
union CQueue_Mail_T mailbox_private mailbox;
union Mutex * mailbox_private mutex;
union Conditional * mailbox_private conditional;
union MailboxCbk * mailbox_private cbk;

    };
};

extern union Mailbox_Class * Get_Mailbox_Class(void);

extern void Mailbox_populate(union Mailbox * const mailbox, IPC_TID_T const tid, union Mutex * const mutex, union Conditional * const conditional, union MailboxCbk * const cbk);

extern void Mailbox_push_mail(union Mailbox * const mailbox, union Mail * const mail);

extern bool Mailbox_retrieve(union Mailbox * const mailbox, union Mail * const mail);

extern bool Mailbox_retrieve_only(union Mailbox * const mailbox, union Mail * const mail, IPC_MID_T const mid);

#ifdef __cplusplus
}
#endif
#undef mailbox_private
#endif /*MAILBOX_H*/