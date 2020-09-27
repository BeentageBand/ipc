#ifndef IPC_HELPER_H_
#define IPC_HELPER_H_
 
#include "conditional/conditional.h"
#include "mailbox/mailbox.h"
#include "mutex/mutex.h"
#include "publisher.h"
#include "semaphore/semaphore.h"
#include "thread.h"
#include "timer/timer.h"
 
#ifdef COBJECT_IMPLEMENTATION
#define _private 
#else
#define _private  const
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef union Thread * Thread_Ptr_T;

#define CSet_Params Thread_Ptr_T
#include "ctemplate-lib/set/cset-template.h"
#undef CSet_Params
 
typedef union Mailbox * Mailbox_Ptr_T;

#define CSet_Params Mailbox_Ptr_T
#include "ctemplate-lib/set/cset-template.h"
#undef CSet_Params

typedef union IPC_Helper
{
   union IPC_Helper_Class _private * _private vtbl;
   struct
   {
      union Object Object;
      union IPC_Helper _private * _private next;
      union CSet_Thread_Ptr_T _private *  _private rthreads;
      union CSet_Mailbox_Ptr_T _private * _private rmailboxes;
      union Mutex _private * _private single_mux;
   };
}IPC_Helper_T;


typedef union IPC_Helper_Class
{
   struct
   {
      struct Class Class;

      IPC_Clock_T (* _private time)(union IPC_Helper * const);
      void (* _private sleep)(union IPC_Helper * const, IPC_Clock_T const);
      
      IPC_TID_T (* _private self_thread)(union IPC_Helper * const);

      bool (* _private alloc_thread)(union IPC_Helper * const, union Thread * const);
      bool (* _private alloc_mailbox)(union IPC_Helper * const, union Mailbox * const);
      bool (* _private alloc_mutex)(union IPC_Helper * const, union Mutex * const);
      bool (* _private alloc_semaphore)(union IPC_Helper * const, union Semaphore * const, uint8_t const);
      bool (* _private alloc_conditional)(union IPC_Helper * const, union Conditional * const);
      bool (* _private alloc_timer)(union IPC_Helper * const, union Timer * const);
   };
}IPC_Helper_Class_T;

extern IPC_Helper_Class_T _private IPC_Helper_Class;

extern union IPC_Helper * IPC_get_instance(void);

extern void Populate_IPC_Helper(union IPC_Helper * const ipc_helper);

extern void IPC_Helper_Append(union IPC_Helper * appendable);
 
extern union Thread * IPC_Helper_find_thread(IPC_TID_T const thread);

extern union Mailbox * IPC_Helper_find_mailbox(IPC_TID_T const mailbox);

#ifdef __cplusplus
}
#endif
#endif /*IPC_HELPER_H_*/
#undef _private