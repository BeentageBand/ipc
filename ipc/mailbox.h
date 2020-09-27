#ifndef MAILBOX_H_
#define MAILBOX_H_
 
#include "conditional.h"
#include "mail.h"
#include "mutex.h"


#ifdef __cplusplus
extern "C" {
#endif

#define CQueue_Params Mail_T
#include "ctemplate-lib/queue/cqueue-template.h"
#undef CQueue_Params

union Mailbox;

typedef union Mailbox_Cbk
{
      struct Mailbox_Cbk_Class _private * _private vtbl;
      union Object Object;
}Mailbox_Cbk_T;

typedef struct Mailbox_Cbk_Class 
{
      struct Class Class;
      bool (*_private register_mbx)(union Mailbox_Cbk * const, union Mailbox * const);
      bool (*_private unregister_mbx)(union Mailbox_Cbk * const, union Mailbox * const);
}Mailbox_Cbk_Class_T;

typedef union Mailbox
{
   struct Mailbox_Class _private * _private vtbl;
   struct
   {
      union Object Object;
      IPC_TID_T _private tid;
      union CQueue_Mail_T _private mailbox;
      union Mail _private picked_mail;
      union Mutex _private mux;
      union Conditional _private cond;
      union Mailbox_Cbk _private * _private cbk;
   };
}Mailbox_T;

typedef struct Mailbox_Class
{
      struct Class Class;
      void (*_private push_mail)(union Mailbox * const, union Mail * const);
      bool (*_private retrieve)(union Mailbox * const, union Mail * const);
      bool (*_private retrieve_only)(union Mailbox * const, union Mail * const, IPC_MID_T const);
}Mailbox_Class_T;

extern struct Mailbox_Class _private Mailbox_Class;
extern struct Mailbox_Cbk_Class _private Mailbox_Cbk_Class;

extern void Populate_Mailbox(union Mailbox * const mbx, IPC_TID_T const tid, union Mail * const mailbox, size_t const mailbox_size);
      
#ifdef __cplusplus
}
#endif
 
#endif /*MAILBOX_H_*/
