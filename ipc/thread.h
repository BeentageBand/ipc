#ifndef THREAD_H_
#define THREAD_H_
 
#include "common/ipc_types.h"
#include "conditional.h"
#include "mutex.h"
#include "semaphore/semaphore.h"

#ifdef COBJECT_IMPLEMENTATION
#define _private 
#else
#define _private  const
#endif
#ifdef __cplusplus
extern "C" {
#endif
union Thread;

typedef union Thread_Cbk
{
      struct Thread_Cbk_Class _private * _private vtbl;
      union Object Object;
}Thread_Cbk_T;

typedef struct Thread_Cbk_Class
{
      struct Class Class;
      bool (* _private register_thread)(union Thread_Cbk * const, union Thread * const);
      bool (* _private run_thread)(union Thread_Cbk * const, union Thread * const);
      bool (* _private join_thread)(union Thread_Cbk * const, union Thread * const);
      bool (* _private unregister_thread)(union Thread_Cbk * const, union Thread * const);
}Thread_Cbk_Class_T;

typedef union Thread
{
   struct Thread_Class _private * _private vtbl;
   struct
   {
      union Object Object;
      IPC_TID_T _private tid;
      bool _private ready;
      union Conditional _private cv;
      union Mutex _private mux;
      union Thread_Cbk _private * _private cbk;
   };
}Thread_T;

typedef struct Thread_Class
{
   struct Class Class;
   void (* _private run)(union Thread * const);
   void (* _private wait)(union Thread * const, IPC_Clock_T const);
   void (* _private ready)(union Thread * const);
   void (* _private join)(union Thread * const, IPC_Clock_T const);
   void (* _private runnable)(union Thread * const);
}Thread_Class_T;

extern struct Thread_Class _private Thread_Class;
extern struct Thread_Cbk_Class _private Thread_Cbk_Class;

extern void Populate_Thread(union Thread * const thread, IPC_TID_T const tid);

extern IPC_TID_T Thread_self(void);
 
#ifdef __cplusplus
}
#endif
#endif /*THREAD_H_*/
#undef _private