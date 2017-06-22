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
#define CLASS_IMPLEMENTATION
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
static void Task_Ctor(Task_T * const this, IPC_Task_Id_T const tid);
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
void Task_init(void)
{
   printf("%s \n", __FUNCTION__);
   Task_Obj.tid = 0;

   Task_Vtbl.ctor = Task_Ctor;
   Task_Vtbl.run = NULL;

}
void Task_shut(void) {}

void Task_Dtor(Object_T * const obj)
{
}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
void Task_Ctor(Task_T * const this, IPC_Task_Id_T const tid)
{
   this->tid = tid;
}
/*=====================================================================================* 
 * task.c
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
