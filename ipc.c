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
        found->vtbl->ready(found);
    }
}

void IPC_Wait(IPC_TID_T const tid, IPC_Clock_T const wait_ms)
{
    union Thread * found = IPC_Helper_find_thread(tid);
    if(NULL != found)
    {
        found->vtbl->wait(found, wait_ms);
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
    IPC_TID_T const self = IPC_Self();
    union Mailbox * const mbx = IPC_Helper_find_mailbox(self);
    IPC_Clock_T timestamp = IPC_Clock() + wait_ms;

    if(!mbx)
    {
        Dbg_Fault("Mailbox %d not found!!", self);
        return false;
    }

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
    IPC_TID_T const self = IPC_Self();
    union Mailbox * const mbx = IPC_Helper_find_mailbox(self);
    IPC_Clock_T timestamp = IPC_Clock() + wait_ms;

    if(!mbx)
    {
        Dbg_Fault("Mailbox %d not found!!", self);
        return false;
    }

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
        Dbg_Info("%s: thread %d send mail %d to thread %d", __func__, IPC_Self(), mid, rcv_tid);
        union Mail mail;
        Populate_Mail(&mail, mid, IPC_Self(), rcv_tid, payload, pay_size);
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
