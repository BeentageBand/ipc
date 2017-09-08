/*=====================================================================================*/
/**
 * linux_mutex.c
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#define CLASS_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID Dbg_FID_Def(IPC_FID,5)
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "dbg_log.h"
#include "linux_mutex.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/
#include <time.h>
/*=====================================================================================* 
 * Local X-Macros
 *=====================================================================================*/
#undef CLASS_VIRTUAL_METHODS
#define CLASS_VIRTUAL_METHODS(_ovr_method)  \
  _ovr_method(Mutex,lock) \
  _ovr_method(Mutex,unlock) \

/*=====================================================================================* 
 * Local Define Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Type Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static void Linux_Mutex_ctor(Linux_Mutex_T * const this);
static bool_t Linux_Mutex_lock(Mutex_T * const this, uint32_t const tout_ms);
static bool_t Linux_Mutex_unlock(Mutex_T * const this);
/*=====================================================================================*
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEFINITION
/*=====================================================================================*
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================*
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================*
 * Local Function Definitions
 *=====================================================================================*/
void Linux_Mutex_Init(void)
{
   Linux_Mutex_Class.lock = Linux_Mutex_lock;
   Linux_Mutex_Class.unlock = Linux_Mutex_unlock;
}


void Linux_Mutex_Delete(struct Object * const obj)
{
   Linux_Mutex_T * const this = _dynamic_cast(Linux_Mutex, obj);
   Isnt_Nullptr(this, );
   pthread_mutex_destroy(&this->pmutex);
}
 /*=====================================================================================*
  * Exported Function Definitions
  *=====================================================================================*/
union Linux_Mutex Linux_Mutex(void)
{
	union Linux_Mutex this = Linux_Mutex_Default();
   pthread_mutex_init(&this.pmutex, NULL);
   return this;
}
union Linux_Mutex * Linux_Mutex_New(void);

bool_t Linux_Mutex_lock(Mutex_T * const super, uint32_t const tout_ms)
{
   Dbg_Info("%s", __FUNCTION__);
   Linux_Mutex_T * const this = _dynamic_cast(Linux_Mutex, super);
   Isnt_Nullptr(this, false);
   struct timespec  tm_out = {0, tout_ms*1000000U};
   return 0 == pthread_mutex_timedlock(&this->pmutex, &tm_out);
}

bool_t Linux_Mutex_unlock(Mutex_T * const super)
{
   Dbg_Info("%s", __FUNCTION__);
   Linux_Mutex_T * const this = _dynamic_cast(Linux_Mutex, super);
   Isnt_Nullptr(this, false);
   return 0 == pthread_mutex_unlock(&this->pmutex);
}
/*=====================================================================================* 
 * mutex.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
