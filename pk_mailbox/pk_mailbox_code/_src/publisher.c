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
#define CLASS_IMPLEMENTATION
/*=====================================================================================*
 * Project Includes
 *=====================================================================================*/
#include "publisher.h"
#include "mailbox.h"
#include "mailbox_ringbuffer.h"
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
 typedef struct
 {
    IPC_Mail_Id_T mid;
    Vector_Mailbox_T subscription;
 }Subscription_T;
/*=====================================================================================* 
 * Local Function Prototypes
 *=====================================================================================*/
static void Publisher_init(void);
static int Subscription_Compare(void const * a, void const * b);
/*=====================================================================================* 
 * Local Object Definitions
 *=====================================================================================*/
#undef IPC_MAIL
#define IPC_MAIL(mail, description) {mail, Vector_Mailbox()},

static Subscription_T Subscription_List[] =
{
      IPC_SUBSCRIBABLE_MAIL_LIST
};
/*=====================================================================================* 
 * Exported Object Definitions
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Inline-Function Like Macros
 *=====================================================================================*/

/*=====================================================================================* 
 * Local Function Definitions
 *=====================================================================================*/
void Publisher_init(void)
{
   static bool_t once = false;

   if(!once)
   {
      qsort(Subscription_List, Num_Elems(Subscription_List), sizeof(*Subscription_List),
            Subscription_Compare);
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
   Publisher_init();
   Subscription_T * const found_sub = (Subscription_T * const) bsearch(&mail_id,
         Subscription_List, Num_Elems(Subscription_List), sizeof(*Subscription_List),
                     Subscription_Compare);
   Isnt_Nullptr(found_sub, false);
   // TODO find if exits, change vector to set
   found_sub->subscription.vtbl->push_back(&found_sub->subscription, mailbox);
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
      Mailbox_T * const mbx = &found_sub->subscription.vtbl->at(&found_sub->subscription, i);
      mbx->vtbl->push_mail(mbx, mail);
   }
}
/*=====================================================================================* 
 * publisher.cpp
 *=====================================================================================*
 * Log History
 *
 *=====================================================================================*/
