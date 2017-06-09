/*=====================================================================================*/
/**
 * task.h
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#if !defined(TASK_H_) || defined(CLASS_IMPLEMENTATION)
#define TASK_H_
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "ipc_types.h"
#include "object.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Define Macros
 *=====================================================================================*/
#undef CLASS_NAME
#undef CLASS_INHERITS
#undef CLASS_MEMBERS
#undef CLASS_METHODS

#define CLASS_NAME Task
#define CLASS_INHERITS Object

#define CLASS_MEMBERS(_member) \
_member(IPC_Task_Id_T _private, tid) \

#define CLASS_METHODS(_method, _void_method) \
void _method(ctor, IPC_Task_Id_T const) \
void _void_method(run) \

#ifdef __cplusplus
extern "C" {
#endif
/*=====================================================================================* 
 * Exported Type Declarations
 *=====================================================================================*/
CLASS_DECLARATION
/*=====================================================================================* 
 * Exported Object Declarations
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Prototypes
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Like Macros
 *=====================================================================================*/
#ifdef __cplusplus
}
#endif
/*=====================================================================================* 
 * task.h
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
#endif /*TASK_H_*/
