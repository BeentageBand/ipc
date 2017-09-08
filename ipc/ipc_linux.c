/*=====================================================================================*/
/**
 * ipc_linux.c
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#define CLASS_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID Dbg_FID_Def(IPC_FID,3)
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "dbg_log.h"
#include "task_ext.h"
#include "mailbox_ext.h"
#include "task.h"
#include "mailbox.h"
#include "ipc_ext.h"
#include "ipc.h"

/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/
#include <pthread.h>
#include <semaphore.h>
/*=====================================================================================* 
 * Local X-Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Define Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Type Definitions
 *=====================================================================================*/
struct PThread_Support
{
	IPC_Task_Id_T tid;
	pthread_t pthread;
	pthread_attr_t attributes;
	sem_t semaphore;
	pthread_mutex_t mutex;
	pthread_mutexattr_t mutex_attr;
	pthread_cond_t cond;
	pthread_condattr_t cond_attr;
};
/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static void * PThread_Routine(void * args)
 /*=====================================================================================*
  * Local Object Definitions
  *=====================================================================================*/
#undef TASK_ID
#define TASK_ID(pid, task, desc) \
	{task##_WORKER, -1, PTHREAD_CREATE_JOINABLE,\
	SEM_FAILED, {0}, \
	PTHREAD_MUTEX_INITIALIZER, \
	{0}, PTHREAD_COND_INITIALIZER},

static struct PThread_Support PThread_Support[IPC_TOTAL_TASK_IDS_ITEMS] =
{
		IPC_WORKERS_IDS(TASK_ID)
};
 /*=====================================================================================*
  * Exported Object Definitions
  *=====================================================================================*/

 /*=====================================================================================*
  * Local Inline-Function Like Macros
  *=====================================================================================*/

 /*=====================================================================================*
  * Local Function Definitions
  *=====================================================================================*/
void * PThread_Routine(void * args)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	union Task * task = (union Task *)args;
	Isnt_Nullptr(task, NULL);

    sem_wait(&PThread_Support[task->tid].semaphore);

	task->runnable(task);

	return NULL;
}
 /*=====================================================================================*
  * Exported Function Definitions
  *=====================================================================================*/
IPC_Task_Id_T IPC_Self_Task_Id(void)
{
	IPC_Task_Id_T tid;
	for(tid = IPC_BEGIN_TASK_ID; tid < IPC_TOTAL_TASK_IDS_ITEMS; ++tid)
	{
		if(0 == pthread_equal(pthread_self(), PThread_Support[tid].pthread))
		{
			return tid;
		}
	}
	return tid;
}

void IPC_Task_Ready(void)
{
	union IPC * ipc = IPC_get_instance();
	Isnt_Nullptr(ipc,);

}

bool_t Task_Cancel(union Task * const task)
{
	Isnt_Nullptr(task, false);
	if(-1 == PThread_Support[task->tid].pthread)
	{
		Dbg_Warn("%s Task %d is already created", __func__, task->tid);
		return false;
	}

	return 0 == pthread_cancel(PThread_Support[task->tid].pthread);
}
bool_t Task_Create(union Task * const task)
{
	Isnt_Nullptr(task, false);
	if(-1 != PThread_Support[task->tid].pthread)
	{
		Dbg_Warn("%s Task %d is already created", __func__, task->tid);
		return false;
	}

	pthread_attr_init(&PThread_Support[task->tid].attributes);
	sem_init(&PThread_Support[task->tid].attributes, task->dependencies->vtbl->size(&task->dependencies));

	pthread_mutex_init(&PThread_Support[task->tid].mutex, &PThread_Support[task->tid].mutex_attr);
	pthread_cond_init(&PThread_Support[task->tid].cond, &PThread_Support[task->tid].cond_attr);

	int rc = pthread_create(&PThread_Support[task->tid].pthread,&PThread_Support[task->tid].attributes,
			PThread_Routine, task);

	return (0 == rc);
}
bool_t Task_Join(union Task * const task, uint32_t wait_ms)
{
	Isnt_Nullptr(task, false);

	if(-1 == PThread_Support[task->tid].pthread)
	{
		Dbg_Warn("%s Task %d is already created", __func__, task->tid);
		return false;
	}
	return 0 == pthread_join(&PThread_Support[task->tid].pthread, NULL);
}

bool_t Mailbox_lock(union Mailbox * const mbx)
{
	Isnt_Nullptr(mbx, false);
	return 0 == pthread_mutex_lock(&PThread_Support[mbx->owner].mutex);
}
bool_t Mailbox_unlock(union Mailbox * const mbx)
{
	Isnt_Nullptr(mbx, false);
	return 0 == pthread_mutex_unlock(&PThread_Support[mbx->owner].mutex);
}

bool_t Mailbox_wait(union Mailbox * const mbx, uint32_t const wait_ms)
{
	Isnt_Nullptr(mbx, false);
	return 0 == pthread_cond_timedwait(&PThread_Support[mbx->owner].cond, &PThread_Support[mbx->owner].mutex, wait_ms);
}
bool_t Mailbox_signal(union Mailbox * const mbx)
{
	Isnt_Nullptr(mbx, false);
	return 0 == pthread_cond_signal(&PThread_Support[mbx->owner].cond);
}
/*=====================================================================================* 
 * ipc_linux.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
