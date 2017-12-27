#define OBJECT_IMPLEMENTATION Mailbox
 
#include "publisher.h"
#include "mailbox.h"
 
typedef union Mailbox * Mbx_Ptr_T;

#define CSet_Params Mbx_Ptr
#include "cset.h"
#include "cset.c"
#undef CSet_Params

#define CMap_Params, IPC_Mail_Id, CSet_Mbx_Ptr
#include "cmap.h"
#include "cmap.c"
#undef CMap_Params

typedef CMap_IPC_Mail_Id_CSet_Mbx_Ptr_T Publisher_Subscription_T;

static void Publisher_Init(void);
static int Subscription_Compare(void const * a, void const * b);
 
static Publisher_Subscription_T Publisher_Subscription;
 
void Publisher_Init(void)
{
   static bool_t once = false;
   static struct CPair_IPC_Mail_Id_CSet_Mbx_Ptr publisher_subscription_buff[IPC_PBC_MID_END - IPC_PBC_MID_BEGIN];

   if(!once)
   {
	   Populate_CMap_IPC_Mail_Id_CSet_Mbx_Ptr(&Publisher_Subscription, 
			   publisher_subscription_buff, Num_Elems(publisher_subscription_buff));
   }
}

int Subscription_Compare(void const * a, void const * b)
{
   IPC_Task_Id_T const * sa = (IPC_Task_Id_T const *)a;
   IPC_Task_Id_T const * sb = (IPC_Task_Id_T const *)b;
   return (*sa - *sb);
}
 
bool_t Publisher_subscribe(Mailbox_T * const mailbox, IPC_Mail_Id_T const mail_id)
{
   static MailBox_Ptr_T mailbox_buffer[IPC_PBC_END - IPC_PBC_MID_BEGIN][IPC_MAX_TID];

   Publisher_Init();
   union CSet_Mbx_Ptr * vector_mbx = Publisher_Subscription->vtbl->find(&Publisher_Subscription, mail_id);
   
   if(NULL == vector_mbx)
   {
      union CSet_Mbx_Ptr mbx_list;
	  Populate_CSet_Mbx_Ptr(&mbx_list, mailbox_buffer[mail_id - IPC_PBC_MID_BEGIN], IPC_MAX_TID);
      Publisher_Suscription->vtbl->insert(&Publisher_Subscription, mail_id, mbx_list);
	  vector_mbx = Publisher_Subscription->vtbl->find(&Publisher_Subscription, mail_id);
   }

   union Mailbox ** mbx = vector_mbx->vtbl->find(vector_mbx, mailbox);

   if(NULL == mbx)
   {
	   vector_mbx->vtbl->insert(vector_mbx, mailbox);
   }

   return true;
}

bool_t Publisher_unsubscribe(Mailbox_T * const mailbox, IPC_Mail_Id_T const mail_id)
{
   Publisher_init();

   union CSet_Mbx_Ptr * vector_mbx = Publisher_Subscription->vtbl->find(&Publisher_Subscription, mail_id);

   if(NULL != vector_mbx)
   {
	   vector_mbx->vtbl->erase(vector_mbx, mailbox);
   }

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
