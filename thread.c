#define COBJECT_IMPLEMENTATION
 
#include <pthread.h>
#include "thread.h"
#include "thread_ext.h"

#define THREAD_INIT(tid, desc) -1,

static void thread_delete(struct Object * const obj);
static void thread_run(union Thread * const this);
static void thread_wait(union Thread * const this);
static void thread_runnable(union Thread * const this);
static void * thread_routine(void * arg);

union Thread_Class Thread_Class =
{
	{thread_delete, NULL},
	thread_run,
	thread_wait,
	thread_runnable
};

static union Thread Thread = {NULL};

static pthread_t Thread_Pthread_Pool[] =
{
	IPC_THREAD_LIST(THREAD_INIT)
};

static pthread_attr_t Thread_Pthread_Attr = PTHREAD_ATTR_INIT;

void thread_delete(struct Object * const obj)
{
   Thread_T * const this = Object_Cast(&Thread_Class, obj);
   Isnt_Nullptr(this,);

   pthread_cancel(Thread_Pthread_Pool[this->tid]);
}
 
void thread_run(union Thread * const this)
{

	if(0 == pthread_create(Thread_Pthread_Pool[this->tid], thread_routine, (void *)this))
	{
	}
}

void * thread_routine(void * arg)
{
	union Thread * const this = (Thread_T *)Object_Cast(Thread_Class.Class, (struct Object *) arg);
	Isnt_Nullptr(this, NULL);
	
	this->vtbl->runnable(this);
	
	return this;
}

void thread_runnable(union Thread * const this){}//Implements!

void thread_wait(union Thread * const this, uint32_t const wait_ms)
{
	if(Thread_Join(this))
	{

	}
}

void Populate_Thread(union Thread * const this, IPC_TID_T const tid)
{
	if(IPC_TID_MAX > tid) return ;
	
	if(NULL == Thread.vtbl)
	{
		Thread.vtbl = &Thread_Class;
		Thread.tid = 0;
	}

	memcpy(this, &Thread, sizeof(Thread));

	this->tid = tid;

	if(-1 == Thread_Pthread_Pool[this->tid])
	{
		pthread_init(&Thread_Pthread_Pool + this->tid, &Thread_Pthread_Attr);
	}
}
