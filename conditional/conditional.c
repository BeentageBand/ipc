/*=====================================================================================*/
/**
 * conditional.c
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#define OBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID Dbg_FID_Def(IPC_FID,6)
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "dbg_log.h"
#include "mutex.h"
#include "conditional.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Local X-Macros
 *=====================================================================================*/

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
CLASS_DEF(Conditional)
/*=====================================================================================*
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================*
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================*
 * Local Function Definitions
 *=====================================================================================*/
void Conditional_Init(void)
{
}

void Conditional_Delete(struct Object * const obj)
{}

 /*=====================================================================================*
  * Exported Function Definitions
  *=====================================================================================*/
union Conditional Conditional_Mutex(Mutex_T * const mutex)
{
	union Conditional this = Conditional_Default();
   this.mutex = mutex;
	return this;
}

union Conditional * Conditional_Mutex_New(Mutex_T * const mutex)
{
	Constructor_New_Impl(Conditional, Mutex, mutex);
}

bool_t Conditional_wait(Conditional_T * const this, uint32_t const timeout_ms)
{ return false; }

bool_t Conditional_signal(Conditional_T * const this)
{ return false; }
/*=====================================================================================* 
 * conditional.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
