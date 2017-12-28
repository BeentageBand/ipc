#define COBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID DBG_FID_DEF(IPC_FID,0)

#include "dbg_log.h"
#include "ipc_set.h"
#include "ipc.h"
#include "ipc_helper.h"

static union Thread * ipc_find_thread(IPC_TID_T const thread);
static union Mailbox * ipc_find_mailbox(IPC_TID_T const mailbox);

union Thread * ipc_find_thread(IPC_TID_T const thread)
{
	union IPC_Helper * ipc_help;
	Allocate_IPC_Helper(ipc_help);
	CMap_IPC_TID_Thread_T * const thread_stack = ipc_help->thread_stack;
	CPair_IPC_TID_Thread_T * found = thread_stack->vtbl->find(thread_stack, thread);
	return (found != thread_stack->vtbl->end(thread_stack))? found->object : NULL;
}

union Thread * ipc_find_thread(IPC_TID_T const thread)
{
	union IPC_Helper * ipc_help;
	Allocate_IPC_Helper(ipc_help);
	CMap_IPC_TID_Mailbox_T * const mailbox_stack = ipc_help->mailbox_stack;
	CPair_IPC_TID_Mailbox_T * found = mailbox_stack->vtbl->find(mailbox_stack, thread);
	return (found != mailbox_stack->vtbl->end(thread_stack))? found->object : NULL;
}

IPC_TID_T IPC_Self(void)
{
	union IPC_Helper * ipc_help;
	Allocate_IPC_Helper(ipc_help);
	IPC_TID_T self_id  = ipc_help->vtbl->self(icp_help);
	union Thread * found = ipc_find_thread(self_id);
	return (NULL != found)? found->tid : MAX_TIDS;
}

void IPC_Ready(void)
{
	union Thread * found = ipc_find_thread(IPC_Self());
	if(NULL != found)
	{
		found->vtbl->post(found);
	}
}

void IPC_Wait(IPC_TID_T const tid, IPC_Clock_T const wait_ms)
{
	union Thread * found = ipc_find_thread(tid);
	if(NULL != found)
	{
		(void)found->vtbl->wait(found, wait_ms);
	}
}
	
void IPC_Run(IPC_TID_T const tid)
{
	union Thread * found = ipc_find_thread(tid);
	if(NULL != found)
	{
		found->vtbl->post(found);
	}
}

bool IPC_Register_Mailbox(union Mailbox * const mbx)
{
	union IPC_Helper * ipc_help;

	Allocate_IPC_Helper(ipc_help);

	CMap_IPC_TID_Mailbox_T * const mailbox_stack = ipc_help->mailbox_stack;
	mailbox_stack->vtbl->insert(mailbox_stack, IPC_Self(), &mbx);
	return NULL != ipc_find_mailbox(IPC_Self());
}

bool IPC_Unregister_Mailbox(union Mailbox * const mbx)
{
	union IPC_Helper * ipc_help;
	Allocate_IPC_Helper(ipc_help);

	CMap_IPC_TID_Mailbox_T * const mailbox_stack = ipc_help->mailbox_stack;
	mailbox_stack->vtbl->erase(mailbox_stack, IPC_Self());
	
	return NULL == ipc_find_mailbox(IPC_Self());
}


bool IPC_Subscribe_Mailist(IPC_MID_T const * const mailist, uint32_t const mailist_size)
{
	union IPC_Helper * ipc_help;
	Allocate_IPC_Helper(ipc_help);
	union Publisher * const publisher = ipc_help->publisher;
	union Mailbox * mailbox = ipc_find_mailbox(IPC_Self());
	uint32_t i;
	bool rc = true;
	for( i; i < mailist_size && rc; ++i)
	{
		rc = Publisher_Subscribe(publisher, mailbox, mailist[i]);
	}
	return rc;
}

bool IPC_Unsubscribe_Mailist(IPC_MID_T const * const mailist, uint32_t const mailist_size)
{
	union IPC_Helper * ipc_help;
	Allocate_IPC_Helper(ipc_help);
	union Publisher * const publisher = ipc_help->publisher;
	union Mailbox * mailbox = ipc_find_mailbox(IPC_Self());
	uint32_t i;
	bool rc = true;
	for( i; i < mailist_size && rc; ++i)
	{
		rc = Publisher_Unsubscribe(publisher, mailbox, mailist[i]);
	}
	return rc;
}

bool IPC_Retrieve_Mail(union Mail * const mail, IPC_Clock_T const wait_ms)
{
	union Mailbox * const mbx = ipc_find_mailbox(IPC_Self());
	IPC_Clock_T timestamp = IPC_Clock() + wait_ms;

	bool rc = false;
	do
	{
		rc = mbx->vtbl->retrieve(mbx, mail);
		IPC_Sleep(50);// do something else
	} while( !rc && !IPC_Time_Elapsed(timestamp));

	return rc;
}

bool IPC_Retrieve_From_Mailist(union Mail * const mail, IPC_Clock_T const wait_ms, IPC_MID_T const * const mailist,
 uint32_t const mailist_size)
{
	union Mailbox * const mbx = ipc_find_mailbox(IPC_Self());
	IPC_Clock_T timestamp = IPC_Clock() + wait_ms;

	bool rc = false;
	uint8_t i = 0;
	do
	{
		for(; i < mailist_size && !rc; ++i)
		{
			rc = mbx->vtbl->retrieve_only(mbx, mail, mailist[i]);
		}
		IPC_Sleep(50);// do something else
	} while( !rc && !IPC_Time_Elapsed(timestamp));
	
	return rc;
}

void IPC_Send(IPC_TID_T const rcv_tid, IPC_MID_T const mid, void const * const payload, size_t const pay_size)
{
	union Mailbox * const mbx = ipc_find_mailbox(rcv_tid);
	if(NULL != mbx)
	{
		union Mail mail;
		Populate_Mail(&mail, IPC_Self(), rcv_tid, mid, mbx->payload_alloc, payload, pay_size);
		mbx->vtbl->push_mail(mbx, &mail);
	}
}

void IPC_Publish(IPC_MID_T const mid, void const * const payload, size_t const pay_size)
{
	union IPC_Helper * ipc_help;
	Allocate_IPC_Helper(ipc_help);
	
	union Publisher * const publisher = ipc_help->publisher;
	
	Publisher_Publish(mid, payload, pay_size);
}

IPC_Clock_T IPC_Clock(void)
{
	union IPC_Helper * ipc_help;
	Allocate_IPC_Helper(ipc_help);
	union Uptime * const uptime = ipc_help->uptime;
	return uptime->millis(uptime);
}


bool IPC_Clock_Elapsed(IPC_Clock_T const clock_ms)
{
	return clock_ms < IPC_Clock();
}

void IPC_Sleep(IPC_Clock_T const ms)
{
	union IPC_Helper * ipc_help;
	Allocate_IPC_Helper(ipc_help);
	ipc_help->vtbl->sleep(ipc_help);
}
