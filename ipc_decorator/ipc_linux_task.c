/*=====================================================================================*/
/**
 * ipc_linux_task.c
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#define CLASS_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID Dbg_FID_Def(IPC_FID,0)
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "dbg_log.h"
#include "ipc_linux_task.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/
#include <pthread.h>
/*=====================================================================================* 
 * Local X-Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Define Macros
 *=====================================================================================*/
#define CLASS_VIRTUAL_METHODS(_ovr_method) \
   _ovr_method(IPC, get_tid) \
   _ovr_method(IPC, run_task) \
   _ovr_method(IPC, wait_task) \
/*=====================================================================================* 
 * Local Type Definitions
 *=====================================================================================*/
struct Linux_Task_Id_Tb
{
   pthread_t ptid;
   char const * const name;
};
/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static void IPC_Linux_Task_Ctor(IPC_Linux_Task_T * const this, IPC_T * const ipc);
static IPC_Task_Id_T IPC_Linux_Task_get_tid(IPC_T * const super);
static int IPC_Linux_Task_run_task(IPC_T * const super, Task_T * const task);
static int IPC_Linux_Task_wait_task(IPC_T * const super, Task_T * const task);

/*=====================================================================================*
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEFINITION

#undef TASK_ID
#define TASK_ID(pid, task, desc) 1,

static pthread_t Pthread_Stack[] =
{
   1,
   IPC_WORKERS_IDS(TASK_ID)
};

static pthread_attr_t PThread_Attr;
/*=====================================================================================*
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================*
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================*
 * Local Function Definitions
 *=====================================================================================*/
void IPC_Linux_Task_init(void)
{
   CHILD_CLASS_INITIALIZATION
   IPC_Linux_Task_Vtbl.ctor = IPC_Linux_Task_Ctor;
   pthread_attr_init(&PThread_Attr);
}
void IPC_Linux_Task_shut(void) {}

void IPC_Linux_Task_Dtor(Object_T * const obj)
{

}

void * IPC_Linux_Task_runnable(void * arg)
{
   Task_T * this_task = (Task_T *)arg;
   Isnt_Nullptr(this_task, NULL);
   Dbg_Warn("IPC_Linux_Task_runnable");

   this_task->vtbl->run(this_task);
   return NULL;
}
/*=====================================================================================*
 * Exported Function Definitions
 *=====================================================================================*/
void IPC_Linux_Task_Ctor(IPC_Linux_Task_T * const this, IPC_T * const ipc)
{
   this->IPC_Decorator.vtbl->ctor(&this->IPC_Decorator, ipc);
}

IPC_Task_Id_T IPC_Linux_Task_get_tid(IPC_T * const super)
{
   IPC_Task_Id_T tid;

   for(tid = 0; tid < IPC_TOTAL_TASK_IDS_ITEMS; ++tid)
   {
      Dbg_Warn("self %lu == stack %lu", pthread_self(), Pthread_Stack[tid]);
      if(pthread_equal(Pthread_Stack[tid], pthread_self()))
      {
         break;
      }
   }
   Dbg_Warn("IPC Self linux_task = %u, pthread = %lu,", tid, Pthread_Stack[tid]);
   return tid;
}
int IPC_Linux_Task_run_task(IPC_T * const super, Task_T * const task)
{
   Isnt_Nullptr(task, -1);

   if(task->tid >= IPC_TOTAL_TASK_IDS_ITEMS) return -1;

   int retval = pthread_create(&Pthread_Stack[task->tid], &PThread_Attr, IPC_Linux_Task_runnable, task);

   Dbg_Warn("Task %d created under pthread %ld", task->tid, Pthread_Stack[task->tid]);
   return retval;
}

int IPC_Linux_Task_wait_task(IPC_T * const super, Task_T * const task)
{
   void * join = NULL;

   pthread_join(Pthread_Stack[task->tid], &join);
   Isnt_Nullptr(join,-1);

   return *((int *)join);
}
/*=====================================================================================* 
 * ipc_linux_task.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
