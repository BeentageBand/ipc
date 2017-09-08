/*=====================================================================================*/
/**
 * publisher.cpp
 * author : puch
 * date : Oct 22 2015
 *
 * description : Any comments
 *
 */
/*=====================================================================================*/
#define OBJECT_IMPLEMENTATION Mailbox
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "publisher.h"
#include "mailbox.h"
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
typedef union Mailbox * Mbx_Ptr;
CLASS_DECL(Vector, Mbx_Ptr)
CLASS_DECL(Map, IPC_Mail_Id_T, Vector_Mbx_Ptr_T)

typedef union Map_IPC_Mail_Id_T_Vector_Mbx_Ptr_T Publisher_Subscription_T;


#define Vector_Params Mbx_Ptr
#include "cvector.c"
#undef Vector_Params

#define Map_Params, IPC_Mail_Id_T, Vector_Mbx_Ptr_T
#include "cmap.c"
#undef Map_Params

/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static void Publisher_Init(void);
static int Subscription_Compare(void const * a, void const * b);
/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
static Publisher_Subscription_T Publisher_Subscription;
/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/
void Publisher_Init(void)
{
   static bool_t once = false;

   if(!once)
   {
	   Publisher_Subscription = Map_IPC_Mail_Id_T_Vector_Mbx_Ptr();
   }
}

int Subscription_Compare(void const * a, void const * b)
{
   IPC_Task_Id_T const * sa = (IPC_Task_Id_T const *)a;
   IPC_Task_Id_T const * sb = (IPC_Task_Id_T const *)b;
   return (*sa - *sb);
}
/*=====================================================================================* 
 * Exported Function Definitions
 *=====================================================================================*/
bool_t Publisher_subscribe(Mailbox_T * const mailbox, IPC_Mail_Id_T const mail_id)
{
   Publisher_Init();
   union Vector_Mbx_Ptr * vector_mbx = Publisher_Subscription->vtbl->find(&Publisher_Subscription, mail_id);
   if(NULL == vector_mbx)
   {

   }
   return true;
}
bool_t Publisher_unsubscribe(Mailbox_T * const mailbox, IPC_Mail_Id_T const mail_id)
{
   Publisher_init();
   Subscription_T * const found_sub = (Subscription_T * const) bsearch(&mail_id,
         Subscription_List, Num_Elems(Subscription_List), sizeof(*Subscription_List),
                     Subscription_Compare);
   Isnt_Nullptr(found_sub, false);
   // TODO find if exits, change vector to set
   // TODO erase found_sub->subscription.vtbl->erase(&found_sub->subscription, mailbox);
   return true;
}
void Publisher_publish_mail(Mail_T * const mail)
{
   Publisher_init();
   Isnt_Nullptr(mail, );
   Subscription_T * const found_sub = (Subscription_T * const) bsearch(&mail->mail_id,
            Subscription_List, Num_Elems(Subscription_List), sizeof(*Subscription_List),
                        Subscription_Compare);
   Isnt_Nullptr(found_sub, );

   for(uint8_t i = 0; i < found_sub->subscription.vtbl->size(&found_sub->subscription);
         ++i)
   {
      Mailbox_T * const mbx = found_sub->subscription.vtbl->at(&found_sub->subscription, i);
      mbx->vtbl->push_mail(mbx, mail);
   }
}
/*=====================================================================================* 
 * publisher.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
