/*=====================================================================================*/
/**
 * linux_conditional.c
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#define CLASS_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID Dbg_FID_Def(IPC_FID,7)
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "dbg_log.h"
#include "linux_mutex.h"
#include "linux_conditional.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/
#include <time.h>
/*=====================================================================================* 
 * Local X-Macros
 *=====================================================================================*/
#undef CLASS_VIRTUAL_METHODS
#define CLASS_VIRTUAL_METHODS(_ovr_method) \
  _ovr_method(Conditional,wait) \
  _ovr_method(Conditional,signal) \

/*=====================================================================================* 
 * Local Define Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Type Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/

/*=====================================================================================*
 * Local Object Definitions
 *=====================================================================================*/
CLASS_DEF(Linux_Conditional)
/*=====================================================================================*
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================*
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================*
 * Local Function Definitions
 *=====================================================================================*/
void Linux_Conditional_init(void)
{
   Linux_Conditional_Class.wait = Linux_Conditional_wait;
   Linux_Conditional_Class.signal = Linux_Conditional_signal;
}

void Linux_Conditional_shut(void) {}

void Linux_Conditional_Dtor(Object_T * const obj)
{
   Linux_Conditional_T * const this = _dynamic_cast(Linux_Conditional, obj);
   Isnt_Nullptr(this, );
   pthread_cond_destroy(&this->pcond);
}
 /*=====================================================================================*
  * Exported Function Definitions
  *=====================================================================================*/
union Linux_Conditional Linux_Conditional_Mutex(union Mutex * const mutex)
{
	union Linux_Conditional this = Linux_Conditional_Default();
	Object_Update_Info(this.Object, &Conditional_Mutex(mutex).Object,
		sizeof(this.Conditional, sizeof(this.Conditional)));
	pthread_cond_init(&this.pcond, NULL);

	Linux_Mutex_T * const linux_mutex = _dynamic_cast(Linux_Mutex, mutex);
	Isnt_Nullptr(linux_mutex, this);
	return this;
}
union Linux_Conditional * Linux_Conditional_Mutex_New(union Mutex * const mutex)
{
	Constructor_New_Impl(Linux_Conditional, Mutex, mutex);
}


bool_t Linux_Conditional_wait(union Conditional * const super, uint32_t const tout_ms)
{
   Dbg_Info("%s", __FUNCTION__);
   Linux_Conditional_T * const this = _dynamic_cast(Linux_Conditional, super);
   Isnt_Nullptr(this, false);

   Linux_Mutex_T * const linux_mutex = _dynamic_cast(Linux_Mutex, super->mutex);
   Isnt_Nullptr(linux_mutex, false);

   struct timespec  tm_out = {0, tout_ms*1000000U};
   return 0 == pthread_cond_timedwait(&this->pcond, &linux_mutex->pmutex, &tm_out);
}

bool_t Linux_Conditional_signal(union Conditional * const super)
{
   Dbg_Info("%s", __FUNCTION__);
   Linux_Conditional_T * const this = _dynamic_cast(Linux_Conditional, super);
   Isnt_Nullptr(this, false);

   return 0 == pthread_cond_signal(&this->pcond);
}
/*=====================================================================================* 
 * conditional.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
