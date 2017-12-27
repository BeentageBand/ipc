#define COBJECT_IMPLEMENTATION
 
#include "publisher.h"
#include "ipc.h"

#define CSet_Params IPC_TID
#include "cset.h"
#include "cset.c"
#undef CSet_Params

typedef CSet_IPC_TID_T Subcription_List_T;

static void publisher_init(void);

static Subscription_List_T Subscription_List[IPC_PBC_END - IPC_PBC_BEGIN];

static IPC_TID_T Subscription_Buff[IPC_PBC_END - IPC_PBC_BEGIN][IPC_MAX_TIDS];

void publisher_init(void)
{
	uint32_t i;
	for(i = 0; i < Num_Elems(Subscription_List); ++i)
	{
		Populate_CSet_IPC_TID(Subscription_List + i, Subscription_Buff + i, IPC_MAX_TIDS);
	}
}

bool Publisher_Subscribe(IPC_TID_T const tid, IPC_MID_T const mid)
{
	if(IPC_PBC_END <= mid && IPC_PBC_BEGIN > mid) return false;
	uint32_t mid_idx = mid - IPC_PBC_BEGIN;

	Subscriptions_List_T * subscription = Subcription_List + mid_idx;
	subscription->vtbl->insert(subscription, tid);

	return (subscription->vtbl->end(subscription) != subscription->vtbl->find(subscription, tid));
}



bool Publisher_Unsubscribe(IPC_TID_T const tid, IPC_MID_T const mid)
{
	if(IPC_PBC_END <= mid && IPC_PBC_BEGIN > mid) return false;
	uint32_t mid_idx = mid - IPC_PBC_BEGIN;

	Subscriptions_List_T * subscription = Subcription_List + mid_idx;
	subscription->vtbl->erase(subscription, tid);

	return (subscription->vtbl->end(subscription) == subscription->vtbl->find(subscription, tid));
}

void Publisher_Publish(IPC_MID_T const mid, void const * const payload, size_t const pay_size)
{
	if(IPC_PBC_END <= mid && IPC_PBC_BEGIN > mid) return false;
	uint32_t mid_idx = mid - IPC_PBC_BEGIN;

	Subscriptions_List_T * subscription = Subcription_List + mid_idx;
    IPC_TID_T * it = subscription->vtbl->begin(subscription);
	for( ; it != subscription->vtbl->end(subscription); ++it)
	{
		IPC_Send((*it), mid, payload, pay_size);
	}
}
