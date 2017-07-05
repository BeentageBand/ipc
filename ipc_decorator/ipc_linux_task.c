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
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
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
   IPC_Task_Id_T tid;
   char const * const name;
};
/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static void IPC_Linux_Task_Ctor(IPC_Linux_Task_T * const this, IPC_T * const ipc);
static IPC_Task_Id_T IPC_Linux_Task_get_tid(IPC_T * const super);
static int IPC_Linux_Task_run_task(IPC_T * const super, Task_T * const task);
static int IPC_Linux_Task_wait_task(IPC_T * const super, Task_T * const task);
static int IPC_Linux_Task_is_this_task(void const * a, void const * b);

/*=====================================================================================*
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEFINITION

#undef TASK_ID
#define TASK_ID(pid, task, desc) {-1, task##_WORKER, #task},

struct Linux_Task_Id_Tb Linux_Task_Lookup[] =
{
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

   qsort(Linux_Task_Lookup, Num_Elems(Linux_Task_Lookup), sizeof(*Linux_Task_Lookup),
         IPC_Linux_Task_is_this_task);
   pthread_attr_init(&PThread_Attr);
}
void IPC_Linux_Task_shut(void) {}

void IPC_Linux_Task_Dtor(Object_T * const obj)
{

}

int IPC_Linux_Task_is_this_task(void const * a, void const * b)
{
   pthread_t const * ta = (pthread_t const *)a;
   pthread_t const * tb = (pthread_t const *)b;
   return (*ta - *tb);
}

void * IPC_Linux_Task_runnable(void * arg)
{
   Task_T * this_task = (Task_T *)arg;
   Isnt_Nullptr(this_task, NULL);
   printf("IPC_Linux_Task_runnable\n\n");

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
   pthread_t ptid = pthread_self();
   struct Linux_Task_Id_Tb * linux_task = bsearch(&ptid, Linux_Task_Lookup, Num_Elems(Linux_Task_Lookup),
         sizeof(*Linux_Task_Lookup), IPC_Linux_Task_is_this_task);
   printf("IPC Self linux_task = %s\n", (linux_task)? "found": "NULL");
   Isnt_Nullptr(linux_task, IPC_TOTAL_TASK_IDS_ITEMS);

   return linux_task->tid;
}
int IPC_Linux_Task_run_task(IPC_T * const super, Task_T * const task)
{
   Isnt_Nullptr(task, -1);

   if(task->tid >= IPC_TOTAL_TASK_IDS_ITEMS) return -1;

   pthread_t t;
   int retval = pthread_create(&t, &PThread_Attr, IPC_Linux_Task_runnable, task);

   for(uint32_t i = 0; i < Num_Elems(Linux_Task_Lookup); ++i)
   {
      if(task->tid == Linux_Task_Lookup[i].tid)
      {
         Linux_Task_Lookup[i].ptid = t;
         break;
      }
   }
   return retval;
}

int IPC_Linux_Task_wait_task(IPC_T * const super, Task_T * const task)
{
   void * join = NULL;
   pthread_t t = -1;

   for(uint32_t i = 0; i < Num_Elems(Linux_Task_Lookup); ++i)
   {
      if(task->tid == Linux_Task_Lookup[i].tid)
      {
         t = Linux_Task_Lookup[i].ptid;
         break;
      }
   }

   pthread_join(t, &join);
   Isnt_Nullptr(join,-1);

   return *((int *)join);
}
/*=====================================================================================* 
 * ipc_linux_task.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
