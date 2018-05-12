/*
 * ipc_posix.c
 *
 *  Created on: Jan 27, 2018
 *      Author: uids6144
 */

#define COBJECT_IMPLEMENTATION
#define Dbg_FID DBG_FID_DEF(IPC_FID, 4)

#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "dbg_log.h"
#include "ipc_posix.h"
#include "ipc.h"

#define THREAD_INIT(tid, desc) -1,
static void * ipc_posix_routine(void * thread);
static void ipc_posix_delete(struct Object * const obj);

static IPC_Clock_T ipc_posix_time(union IPC_Helper * const helper);
static void ipc_posix_sleep(union IPC_Helper * const helper, IPC_Clock_T const sleep_ms);
static bool ipc_posix_is_time_elapsed(union IPC_Helper * const helper, IPC_Clock_T const time_ms);

static IPC_TID_T ipc_posix_self_thread(union IPC_Helper * const helper);
static bool ipc_posix_alloc_thread(union IPC_Helper * const helper, union Thread * const thread);
static bool ipc_posix_free_thread(union IPC_Helper * const helper, union Thread * const thread);
static bool ipc_posix_run_thread(union IPC_Helper * const helper, union Thread * const thread);
static bool ipc_posix_join_thread(union IPC_Helper * const helper, union Thread * const thread);

static bool ipc_posix_alloc_mutex(union IPC_Helper * const helper, union Mutex * const mutex);
static bool ipc_posix_free_mutex(union IPC_Helper * const helper, union Mutex * const mutex);
static bool ipc_posix_lock_mutex(union IPC_Helper * const helper, union Mutex * const mutex,
             IPC_Clock_T const wait_ms);
static bool ipc_posix_unlock_mutex(union IPC_Helper * const helper, union Mutex * const mutex);

static bool ipc_posix_alloc_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore,
                  uint8_t const value);
static bool ipc_posix_free_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore);
static bool ipc_posix_wait_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore,
                 IPC_Clock_T const wait_ms);
static bool ipc_posix_post_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore);

static bool ipc_posix_alloc_conditional(union IPC_Helper * const helper, union Conditional * const conditional);
static bool ipc_posix_free_conditional(union IPC_Helper * const helper, union Conditional * const conditional);
static bool ipc_posix_wait_conditional(union IPC_Helper * const helper, union Conditional * const conditional,
                   union Mutex * const mutex, IPC_Clock_T const wait_ms);
static bool ipc_posix_post_conditional(union IPC_Helper * const helper, union Conditional * const conditional);

static void ipc_posix_make_timespec(struct timespec * const tm, IPC_Clock_T const clock_ms);

IPC_POSIX_Class_T IPC_POSIX_Class =
    {{
   { ipc_posix_delete, NULL},
   ipc_posix_time,
   ipc_posix_sleep,
   ipc_posix_is_time_elapsed,

   ipc_posix_self_thread,
   ipc_posix_alloc_thread,
   ipc_posix_free_thread,
   ipc_posix_run_thread,
   ipc_posix_join_thread,

   ipc_posix_alloc_mutex,
   ipc_posix_free_mutex,
   ipc_posix_lock_mutex,
   ipc_posix_unlock_mutex,

   ipc_posix_alloc_semaphore,
   ipc_posix_free_semaphore,
   ipc_posix_wait_semaphore,
   ipc_posix_post_semaphore,

   ipc_posix_alloc_conditional,
   ipc_posix_free_conditional,
   ipc_posix_wait_conditional,
   ipc_posix_post_conditional
    }};

static union IPC_POSIX IPC_POSIX = {NULL};
static pthread_condattr_t POSIX_Cond_Attr = PTHREAD_COND_INITIALIZER;
static pthread_attr_t POSIX_Thread_Attr;
static pthread_mutexattr_t POSIX_Mux_Attr;

static pthread_t POSIX_Pool[IPC_MAX_TID] =
    {
   -1,
   IPC_THREAD_LIST(THREAD_INIT)
    };

void * ipc_posix_routine(void * thread)
{
   union Thread * const this = _cast(Thread, (union Thread *)thread);

   Isnt_Nullptr(this, NULL);

   Dbg_Info("%s for thread = %d", __func__, this->tid);

   if(this->vtbl && this->vtbl->runnable)
   {
      this->vtbl->runnable(this);
   }
   else
   {
      Dbg_Fault("%s:Unable to run thread %d", __func__, this->tid);
   }

   union Semaphore * t_sem = &this->sem_ready;
   t_sem->vtbl->post(t_sem);// thread ended wait
   pthread_exit(NULL);
   return NULL;
}

void ipc_posix_delete(struct Object * const obj)
{
  IPC_TID_T i;

  for(i = 0; i < IPC_MAX_MID; ++i)
    {
      if( -1 != POSIX_Pool[i])
   {
     union Thread * thread = IPC_Helper_find_thread(i);
     if(thread)
       {
         _delete(thread);
       }
   }
    }

  pthread_condattr_destroy(&POSIX_Cond_Attr);
  pthread_attr_destroy(&POSIX_Thread_Attr);
  pthread_mutexattr_destroy(&POSIX_Mux_Attr);

}

IPC_Clock_T ipc_posix_time(union IPC_Helper * const helper)
{
  struct timespec timespec;
  int rc = clock_gettime(CLOCK_MONOTONIC, &timespec);
  return (rc)? 0 : (timespec.tv_sec * 1000 + timespec.tv_nsec / 1000000);
}

void ipc_posix_sleep(union IPC_Helper * const helper, IPC_Clock_T const sleep_ms)
{
  usleep(sleep_ms * 1000);
}

bool ipc_posix_is_time_elapsed(union IPC_Helper * const helper, IPC_Clock_T const time_ms)
{
  return (time_ms < helper->vtbl->time(helper));
}

IPC_TID_T ipc_posix_self_thread(union IPC_Helper * const helper)
{
  IPC_TID_T i;
  pthread_t self = pthread_self();

  for(i = 0; i < IPC_MAX_TID; ++i)
    {
      if( pthread_equal(POSIX_Pool[i], self))
   {
     break;
   }
    }

  return i;
}

bool ipc_posix_alloc_thread(union IPC_Helper * const helper, union Thread * const thread)
{
  bool rc = true;

  if(-1 == POSIX_Pool[thread->tid])
    {
      rc = true;
      thread->attr = &POSIX_Thread_Attr;
      IPC_Register_Thread(thread);
    }
  return rc;
}

bool ipc_posix_free_thread(union IPC_Helper * const helper, union Thread * const thread)
{
  bool rc = false;

  if(0 == pthread_cancel(POSIX_Pool[thread->tid]))
    {
      POSIX_Pool[thread->tid] = -1;
    }
  return rc;
}

bool ipc_posix_run_thread(union IPC_Helper * const helper, union Thread * const thread)
{
  bool rc = false;
  if(-1 == POSIX_Pool[thread->tid])
    {
      rc = 0 == pthread_create(POSIX_Pool + thread->tid,
                (pthread_attr_t *)thread->attr,
                ipc_posix_routine, (void *)thread);
    }
  return rc;
}

bool ipc_posix_join_thread(union IPC_Helper * const helper, union Thread * const thread)
{
  bool rc = false;

  if(-1 == POSIX_Pool[thread->tid])
    {
      union Thread * t = NULL;
      rc = 0 == pthread_join(POSIX_Pool[thread->tid],
              (void **) &t);
    }

  return rc;
}

bool ipc_posix_alloc_mutex(union IPC_Helper * const helper, union Mutex * const mutex)
{
  return 0 == pthread_mutex_init((pthread_mutex_t *)&mutex->mux, &POSIX_Mux_Attr);
}

bool ipc_posix_free_mutex(union IPC_Helper * const helper, union Mutex * const mutex)
{
  return 0 == pthread_mutex_destroy((pthread_mutex_t *)&mutex->mux);
}

bool ipc_posix_lock_mutex(union IPC_Helper * const helper, union Mutex * const mutex,
           IPC_Clock_T const wait_ms)
{
  struct timespec timespec;
  ipc_posix_make_timespec(&timespec, wait_ms);

  return 0 == pthread_mutex_timedlock((pthread_mutex_t *)&mutex->mux,
                  &timespec);
}

bool ipc_posix_unlock_mutex(union IPC_Helper * const helper, union Mutex * const mutex)
{
  return 0 == pthread_mutex_unlock((pthread_mutex_t *)&mutex->mux);
}

bool ipc_posix_alloc_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore,
                uint8_t const value)
{
  return 0 == sem_init((sem_t *)&semaphore->sem, 0, value);
}

bool ipc_posix_free_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore)
{
  return 0 == sem_destroy((sem_t *)&semaphore->sem);
}

bool ipc_posix_wait_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore,
               IPC_Clock_T const wait_ms)
{
  struct timespec timespec;
  ipc_posix_make_timespec(&timespec, wait_ms);
  return 0 == sem_timedwait((sem_t *)&semaphore->sem, &timespec);
}

bool ipc_posix_post_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore)
{
  return 0 == sem_post((sem_t *)&semaphore->sem);
}

bool ipc_posix_alloc_conditional(union IPC_Helper * const helper, union Conditional * const conditional)
{
  return 0 == pthread_cond_init((pthread_cond_t *)&conditional->conditional,
            &POSIX_Cond_Attr);
}

bool ipc_posix_free_conditional(union IPC_Helper * const helper, union Conditional * const conditional)
{
  return 0 == pthread_cond_destroy((pthread_cond_t *)&conditional->conditional);
}

bool ipc_posix_wait_conditional(union IPC_Helper * const helper, union Conditional * const conditional,
            union Mutex * const mutex, IPC_Clock_T const wait_ms)
{
  struct timespec timespec;
  ipc_posix_make_timespec(&timespec, wait_ms);
  return 0 == pthread_cond_timedwait((pthread_cond_t *)&conditional->conditional,
                 (pthread_mutex_t *)&mutex->mux, &timespec);
}

bool ipc_posix_post_conditional(union IPC_Helper * const helper, union Conditional * const conditional)
{
  union IPC_POSIX * const this = _cast(IPC_POSIX, helper);
  Isnt_Nullptr(this, false);
  return 0 == pthread_cond_signal((pthread_cond_t *)&conditional->conditional);
}

void ipc_posix_make_timespec(struct timespec * const tm, IPC_Clock_T const clock_ms)
{
  tm->tv_sec = clock_ms / 1000;
  tm->tv_nsec = clock_ms - (tm->tv_sec * 1000);
  tm->tv_nsec *= 1000000;
}

void Populate_IPC_POSIX(union IPC_POSIX * const this)
{

  if(NULL == IPC_POSIX.vtbl)
    {
      POSIX_Pool[IPC_MAIN_TID] = pthread_self();
      Dbg_Warn("Start IPC POSIX: starter thread %d is IPC_MAIN_TID", POSIX_Pool[IPC_MAIN_TID]);
      Populate_IPC_Helper(&IPC_POSIX.IPC_Helper);
      Object_Init(&IPC_POSIX.Object, &IPC_POSIX_Class.Class, 0);
      pthread_condattr_init(&POSIX_Cond_Attr);
      pthread_attr_init(&POSIX_Thread_Attr);
      pthread_mutexattr_init(&POSIX_Mux_Attr);
    }
  memcpy(this, &IPC_POSIX, sizeof(IPC_POSIX));
}
