#ifndef TASK_EXT_H_
#define TASK_EXT_H_

#include "std_reuse.h"
 
#ifdef __cplusplus
extern "C" {
#endif
 
extern bool_t Task_Register_To_Process(union Task * const obj);
extern bool_t Task_Cancel(union Task * const obj);
extern bool_t Task_Create(union Task * const obj);
extern bool_t Task_Join(union Task * const obj, uint32_t wait_ms);

#ifdef __cplusplus
}
#endif
#endif /*TASK_EXT_H_*/
