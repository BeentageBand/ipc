/*=====================================================================================*/
/**
 * task.c
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#define OBJECT_IMPLEMENTATION Task
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "task.h"
#include "task_ext.h"
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
CLASS_DEF(Task)
/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/
void Task_Init(void)
{
   Task_Obj.tid = 0;
}

void Task_Delete(struct Object * const obj)
{
   Task_T * const this = _dynamic_cast(Task, obj);
   Isnt_Nullptr(this,);
   Task_Cancel(this);
}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
union Task Task_Tid(IPC_Task_Id_T const tid)
{
	union Task this = Task_Default();
   this.tid = tid;

   return this;
}

union Task * Task_Tid_New(IPC_Task_Id_T const tid)
{
	Constructor_New_Impl(Task, Tid, tid);
}

void Task_run(union Task * const this)
{
	Task_Register_To_Process(&this);

	if(Task_Create(this))
	{
	}
}

void Task_runnable(union Task * const this){}//Implements!

void Task_wait(union Task * const this, uint32_t const wait_ms)
{
	if(Task_Join(this))
	{

	}
}
/*=====================================================================================* 
 * task.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
