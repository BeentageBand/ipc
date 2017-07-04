/*=====================================================================================*/
/**
 * mail_ringbuffer.h
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#ifndef MAIL_RINGBUFFER_H_
#define MAIL_RINGBUFFER_H_
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Define Macros
 *=====================================================================================*/
#define OBJECT_TEMPLATE
#define _template_t1 Mail_Ptr
/*=====================================================================================* 
 * Exported Type Declarations
 *=====================================================================================*/
typedef Mail_T * Mail_Ptr_T;
/*=====================================================================================* 
 * Exported Object Declarations
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Prototypes
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Like Macros
 *=====================================================================================*/
#include "cvector.h"
#undef _template_t1
#undef OBJECT_TEMPLATE

/*=====================================================================================* 
 * mail_ringbuffer.h
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
#endif /*MAIL_RINGBUFFER_H_*/