#ifndef MAILBOXCBK_H
#define MAILBOXCBK_H
#include "cobject/cobject.h"

#ifdef MAILBOXCBK_IMPLEMENTATION 
#define cbk_private
#else
#define cbk_private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union MailboxCbk;
union MailboxCbk_Class
{
    
    struct
    {
    struct Class Class;
    bool (* cbk_private register_mbx)(union MailboxCbk * const mailboxcbk, union Mailbox * const mailbox);
bool (* cbk_private unregister_mbx)(union MailboxCbk * const mailboxcbk, union Mailbox * const mailbox);

    };
};

union MailboxCbk
{
    union MailboxCbk_Class * vtbl;
        struct
    {
      union Object Object;
      
    };
};

extern union MailboxCbk_Class * Get_MailboxCbk_Class(void);

extern void MailboxCbk_populate(union MailboxCbk * const mailboxcbk);

extern bool MailboxCbk_register_mbx(union MailboxCbk * const mailboxcbk, union Mailbox * const mailbox);

extern bool MailboxCbk_unregister_mbx(union MailboxCbk * const mailboxcbk, union Mailbox * const mailbox);

#ifdef __cplusplus
}
#endif
#undef cbk_private
#endif /*MAILBOXCBK_H*/