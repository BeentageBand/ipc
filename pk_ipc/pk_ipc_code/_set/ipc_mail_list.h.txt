/*=====================================================================================*/
/**
 * ipc_mail_list.h
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#ifndef IPC_MAIL_LIST_H_
#define IPC_MAIL_LIST_H_
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "dread_stdin_hdr_evs.h"
/*=====================================================================================* 
 * Standard Includes
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Define Macros
 *=====================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#define IPC_PRIVATE_MAIL_LIST \
   DREAD_STDIN_PRIVATE_MAIL_LIST  \

#define IPC_SUBSCRIBABLE_MAIL_LIST \
   DREAD_STDIN_SUBSCRIBABLE_MAIL_LIST  \

#define IPC_RETRIEVE_TOUT_MS (500U)
/*=====================================================================================* 
 * Exported Type Declarations
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Object Declarations
 *=====================================================================================*/

/*=====================================================================================* 
 * Exported Function Prototypes
 *=====================================================================================*/
#undef IPC_MAIL
#define IPC_MAIL(mail, description) mail,

#define PRIVATE_MAIL(mail, desc)      IPC_MAIL(mail, desc)
#define SUBSCRIBABLE_MAIL(mail, desc) IPC_MAIL(mail, desc)


enum
{
   IPC_BEGIN_PRIVATE_MAIL_LIST_ID = 0,
   IPC_PRIVATE_MAIL_LIST
   IPC_END_PRIVATE_MAIL_LIST_ID,
   IPC_BEGIN_SUBSCRIBABLE_MAIL_LIST_ID = IPC_END_PRIVATE_MAIL_LIST_ID,
   IPC_SUBSCRIBABLE_MAIL_LIST
   IPC_END_SUBSCRIBABLE_MAIL_LIST_ID,
   IPC_TOTAL_MAIL_LIST_ITEMS
};
/*=====================================================================================* 
 * Exported Function Like Macros
 *=====================================================================================*/
#ifdef __cplusplus
}
#endif
/*=====================================================================================* 
 * ipc_mail_list.h
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
#endif /*IPC_MAIL_LIST_H_*/
