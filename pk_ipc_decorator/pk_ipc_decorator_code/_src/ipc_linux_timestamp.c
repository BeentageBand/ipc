/*=====================================================================================*/
/**
 * ipc_linux_timestamp.c
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
#include "ipc_linux_timestamp.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/
#include <unistd.h>
#include <time.h>
/*=====================================================================================* 
 * Local X-Macros
 *=====================================================================================*/
#define CLASS_VIRTUAL_METHODS(_ovr) \
   _ovr(IPC,get_date_length) \
   _ovr(IPC,get_date) \
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

/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static void IPC_Linux_Timestamp_Ctor(IPC_Linux_Timestamp_T * const this, IPC_T * const ipc);
static size_t IPC_Linux_Timestamp_get_date_length(IPC_T * const super);
static char const * IPC_Linux_Timestamp_get_date(IPC_T * const super);
static char const * Date_Fmt = "%4d-%2d-%2d %2d:%2d";
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
void IPC_Linux_Timestamp_init(void)
{
   printf("%s \n", __FUNCTION__);
   CHILD_CLASS_INITIALIZATION
   IPC_Linux_Timestamp_Vtbl.ctor = IPC_Linux_Timestamp_Ctor;
   IPC_Linux_Timestamp_Obj.date = (char const *)malloc(sizeof(Date_Fmt));
}
void IPC_Linux_Timestamp_shut(void) {}

void IPC_Linux_Timestamp_Dtor(Object_T * const obj)
{
}
/*=====================================================================================*
 * Exported Function Definitions
 *=====================================================================================*/
void IPC_Linux_Timestamp_Ctor(IPC_Linux_Timestamp_T * const this, IPC_T * const ipc)
{
   this->IPC_Decorator.vtbl->ctor(&this->IPC_Decorator, ipc);
}


size_t IPC_Linux_Timestamp_get_date_length(IPC_T * const super)
{
   return sizeof(Date_Fmt);
}

char const * IPC_Linux_Timestamp_get_date(IPC_T * const super)
{
   IPC_Linux_Timestamp_T * const this = _dynamic_cast(IPC_Linux_Timestamp, super);
   Isnt_Nullptr(this, NULL);
   time_t linux_time = time(NULL);
   struct tm * tm = localtime(&linux_time);
   Isnt_Nullptr(tm, 0);

   sprintf(this->date, Date_Fmt, tm->tm_year, tm->tm_mon, tm->tm_mday,
         tm->tm_hour, tm->tm_min);
   return this->date;
}
/*=====================================================================================* 
 * ipc_linux_timestamp.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
