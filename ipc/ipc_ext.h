/*=====================================================================================*/
/**
 * ipc_ext.h
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#ifndef IPC_EXT_H_
#define IPC_EXT_H_
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "ipc_types.h"
#include "mailbox.h"
#include "publisher.h"
#include "task.h"
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

#define CLASS_NAME IPC
#define CLASS_INHERITS Object
#define CLASS_MEMBERS(_member) \


#define CLASS_METHODS(_method, _void_method) \
IPC_Task_Id_T _void_method(get_tid) \
int _method(run_task, Task_T * const) \
bool_t _method(register_task, Task_T * const) \
bool_t _method(unregister_task, Task_T * const) \
void _void_method(task_ready) \
int _method(wait_task, Task_T * const) \
void _method(sleep, uint32_t const) \
void _method(set_mailbox, uint32_t const, uint32_t const) \
uint32_t _void_method(timestamp) \
size_t _void_method(get_date_length) \
char const * _void_method(get_date) \

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
extern void IPC_get_instance(IPC_T ** singleton);
/*=====================================================================================* 
 * Exported Function Like Macros
 *=====================================================================================*/
#ifdef __cplusplus
}
#endif
/*=====================================================================================* 
 * ipc_ext.h
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
#endif /*IPC_EXR_H_*/
