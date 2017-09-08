/*=====================================================================================*/
/**
 * mutex.h
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#ifndef MUTEX_H_
#define MUTEX_H_
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "object.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Define Macros
 *=====================================================================================*/
#define Mutex_INHERITS BASE_CLASS
#define Mutex_MEMBERS(_member, _class) \

#define Mutex_METHODS(_method, _class) \
 _method(bool_t, _class, lock, uint32_t const) \
 _method(bool_t, _class, unlock, void) \

#ifdef __cplusplus
extern "C" {
#endif
/*=====================================================================================* 
 * Exported Type Declarations
 *=====================================================================================*/
CLASS_DECL(Mutex)
/*=====================================================================================* 
 * Exported Object Declarations
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Prototypes
 *=====================================================================================*/
extern union Mutex Mutex(void);
extern union Mutex * Mutex_New(void);
/*=====================================================================================* 
 * Exported Function Like Macros
 *=====================================================================================*/
#ifdef __cplusplus
}
#endif
/*=====================================================================================* 
 * mutex.h
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
#endif /*MUTEX_H_*/
