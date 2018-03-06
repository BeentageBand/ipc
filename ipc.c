#define COBJECT_IMPLEMENTATION
#undef Dbg_FID
#define Dbg_FID DBG_FID_DEF(IPC_FID,0)

#include "dbg_log.h"
#include "ipc_set.h"
#include "ipc.h"
#include "ipc_helper.h"

IPC_TID_T IPC_Self(void)
{
    union IPC_Helper * ipc_help = IPC_get_instance();
    IPC_TID_T self_id  = ipc_help->vtbl->self_thread(ipc_help);
    union Thread * found = IPC_Helper_find_thread(self_id);
    return (NULL != found)? found->tid : IPC_MAX_TID;
}

void IPC_Ready(void)
{
    union Thread * found = IPC_Helper_find_thread(IPC_Self());
    if(NULL != found)
    {
        //TODO Thread safe support
        //		found->vtbl->post(found);
    }
}

void IPC_Wait(IPC_TID_T const tid, IPC_Clock_T const wait_ms)
{
    union Thread * found = IPC_Helper_find_thread(tid);
    if(NULL != found)
    {
        found->vtbl->wait(found, wait_ms);
        _delete(found);
    }
}

void IPC_Run(IPC_TID_T const tid)
{
    union Thread * found = IPC_Helper_find_thread(tid);
    if(NULL != found)
    {
        found->vtbl->run(found);
    }
}

bool IPC_Register_Mailbox(union Mailbox * const mbx)
{
    union IPC_Helper * ipc_help = IPC_get_instance();
    union Mutex * mux = ipc_help->single_mux;

    if(mux->vtbl->lock(mux, 10000))
    {
        CSet_Mailbox_Ptr_T * const mailbox_stack = ipc_help->rmailboxes;
        mailbox_stack->vtbl->insert(mailbox_stack, mbx);
    }

    mux->vtbl->unlock(mux);
    return NULL != IPC_Helper_find_mailbox(mbx->tid);
}

bool IPC_Unregister_Mailbox(union Mailbox * const mbx)
{
    union IPC_Helper * ipc_help = IPC_get_instance();
    union Mutex * mux = ipc_help->single_mux;

    if(mux->vtbl->lock(mux, 10000))
    {
        CSet_Mailbox_Ptr_T * const mailbox_stack = ipc_help->rmailboxes;
        mailbox_stack->vtbl->erase(mailbox_stack, mbx);
    }

    mux->vtbl->unlock(mux);
    return NULL == IPC_Helper_find_mailbox(mbx->tid);
}

bool IPC_Register_Thread(union Thread * const thread)
{
    union IPC_Helper * ipc_help = IPC_get_instance();
    union Mutex * const mux = ipc_help->single_mux;

    if(mux->vtbl->lock(mux, 10000))
    {
        CSet_Thread_Ptr_T * const thread_stack = ipc_help->rthreads;
        thread_stack->vtbl->insert(thread_stack, thread);
    }

    mux->vtbl->unlock(mux);
    return NULL != IPC_Helper_find_thread(thread->tid);
}

bool IPC_Unregister_Thread(union Thread * const thread)
{
    union IPC_Helper * ipc_help = IPC_get_instance();
    union Mutex * const mux = ipc_help->single_mux;

    if(mux->vtbl->lock(mux, 10000))
    {
        CSet_Thread_Ptr_T * const thread_stack = ipc_help->rthreads;
        thread_stack->vtbl->erase(thread_stack, thread);
    }

    mux->vtbl->unlock(mux);
    return NULL == IPC_Helper_find_thread(thread->tid);
}


bool IPC_Subscribe_Mailist(IPC_MID_T const * const mailist, uint32_t const mailist_size)
{
    uint32_t i;
    bool rc = true;
    for(i = 0; i < mailist_size && rc; ++i)
    {
        rc = Publisher_Subscribe(IPC_Self(), mailist[i]);
    }
    return rc;
}

bool IPC_Unsubscribe_Mailist(IPC_MID_T const * const mailist, uint32_t const mailist_size)
{
    uint32_t i;
    bool rc = true;
    for(i = 0; i < mailist_size && rc; ++i)
    {
        rc = Publisher_Unsubscribe(IPC_Self(), mailist[i]);
    }
    return rc;
}

bool IPC_Retrieve_Mail(union Mail * const mail, IPC_Clock_T const wait_ms)
{
    union Mailbox * const mbx = IPC_Helper_find_mailbox(IPC_Self());
    IPC_Clock_T timestamp = IPC_Clock() + wait_ms;

    bool rc = false;
    do
    {
        rc = mbx->vtbl->retrieve(mbx, mail);
        IPC_Sleep(50);// do something else
    } while( !rc && !IPC_Clock_Elapsed(timestamp));

    return rc;
}

bool IPC_Retrieve_From_Mailist(union Mail * const mail, IPC_Clock_T const wait_ms, IPC_MID_T const * const mailist,
        uint32_t const mailist_size)
{
    union Mailbox * const mbx = IPC_Helper_find_mailbox(IPC_Self());
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
    } while( !rc && !IPC_Clock_Elapsed(timestamp));

    return rc;
}

void IPC_Send(IPC_TID_T const rcv_tid, IPC_MID_T const mid, void const * const payload, size_t const pay_size)
{
    union Mailbox * const mbx = IPC_Helper_find_mailbox(rcv_tid);
    if(NULL != mbx)
    {
        union Mail mail;
        Populate_Mail(&mail, IPC_Self(), rcv_tid, mid, payload, pay_size);
        mbx->vtbl->push_mail(mbx, &mail);
    }
}

void IPC_Publish(IPC_MID_T const mid, void const * const payload, size_t const pay_size)
{
    Publisher_Publish(mid, payload, pay_size);
}

IPC_Clock_T IPC_Clock(void)
{
    union IPC_Helper * ipc_help = IPC_get_instance();
    return ipc_help->vtbl->time(ipc_help);
}


bool IPC_Clock_Elapsed(IPC_Clock_T const clock_ms)
{
    return clock_ms < IPC_Clock();
}

void IPC_Sleep(IPC_Clock_T const ms)
{
    union IPC_Helper * ipc_help = IPC_get_instance();
    ipc_help->vtbl->sleep(ipc_help, ms);
}
