#ifndef THREAD_EXT_H_
#define THREAD_EXT_H_

#include "std_reuse.h"
 
#ifdef __cplusplus
extern "C" {
#endif
 
extern bool_t Thread_Register_To_Process(union Thread * const obj);
extern bool_t Thread_Cancel(union Thread * const obj);
extern bool_t Thread_Create(union Thread * const obj);
extern bool_t Thread_Join(union Thread * const obj, uint32_t wait_ms);

#ifdef __cplusplus
}
#endif
#endif /*THREAD_EXT_H_*/
