#define COBJECT_IMPLEMENTATION
 
#include "thread.h"
#include "thread_ext.h"

static void thread_delete(struct Object * const obj);
static void thread_run(union Thread * const this);
static void thread_wait(union Thread * const this);
static void thread_runnable(union Thread * const this);

union Thread_Class Thread_Class =
{
	{thread_delete, NULL},
	thread_run,
	thread_wait,
	thread_runnable
};

static union Thread Thread = {NULL};

void thread_delete(struct Object * const obj)
{
   Thread_T * const this = Object_Cast(&Thread_Class, obj);
   Isnt_Nullptr(this,);
   Thread_Cancel(this);
}
 
void Populate_Thread(union Thread * const this, IPC_TID_T const tid)
{
	if(NULL == Thread.vtbl)
	{
		Thread.vtbl = &Thread_Class;
		Thread.tid = 0;
	}

	memcpy(this, &Thread, sizeof(Thread));
	this.tid = tid;

	return this;
}

void thread_run(union Thread * const this)
{
	Thread_Register_To_Process(&this);

	if(Thread_Create(this))
	{
	}
}

void thread_runnable(union Thread * const this){}//Implements!

void thread_wait(union Thread * const this, uint32_t const wait_ms)
{
	if(Thread_Join(this))
	{

	}
}
