#include "ipc.h"
#include "logger/logger.h"
#include "helper/ipchelper.h"
#include "publisher/publisher.h"

#define Dbg_Fault(...) Logger_error(get_logger(), __VA_ARGS__)
#define Dbg_Info(...)  Logger_info(get_logger(), __VA_ARGS__)

static union Logger * get_logger(void);
static union IPCHelper * get_helper(void);
static union IPCClock * get_clock(void);

union Logger * get_logger(void)
{
    static union Logger log = {NULL};
    if (NULL == log.vtbl) {
        Logger_populate(&log, NULL, NULL);
    }
    return &log;
}

union IPCHelper * get_helper(void)
{
    static union IPCHelper helper = {NULL};

    if (NULL == helper.vtbl)
    {
        IPCHelper_populate(&helper, NULL);
    }
    return &helper;
}

IPC_TID_T IPC_Self(void)
{
    union IPCHelper * helper = get_helper();
    IPC_TID_T self_id  = 0; // Implement self id
    union Thread * found = IPCHelper_find_thread(helper, self_id);
    return (NULL != found)? found->id : -1;
}

void IPC_Ready(void)
{
    union IPCHelper * helper = get_helper();
    union Thread * found = IPCHelper_find_thread(get_helper(), IPC_Self());
    if(NULL != found)
    {
        Thread_ready(found);
    }
}

void IPC_Wait(IPC_TID_T const id, IPC_Clock_T const wait_ms)
{
    union IPCHelper * helper = get_helper();
    union Thread * found = IPCHelper_find_thread(helper, id);
    if(NULL != found)
    {
        Thread_wait(found, wait_ms);
    }
}

void IPC_Run(IPC_TID_T const id)
{
    union IPCHelper * helper = get_helper();
    union Thread * found = IPCHelper_find_thread(helper, id);
    if(NULL != found)
    {
        Thread_run(found);
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
    union IPCHelper * helper = get_helper();
    IPC_TID_T const self = IPC_Self();
    union Mailbox * const mbx = IPCHelper_find_mailbox(helper, self);
    IPC_Clock_T timestamp = IPC_Clock() + wait_ms;

    if(!mbx)
    {
        Dbg_Fault("Mailbox %d not found!!", self);
        return false;
    }

    bool rc = false;
    do
    {
        rc = Mailbox_retrieve(mbx, mail);
        IPC_Sleep(50);// do something else
    } while( !rc && !IPC_Clock_Elapsed(timestamp));

    return rc;
}

bool IPC_Retrieve_From_Mailist(union Mail * const mail, IPC_Clock_T const wait_ms, IPC_MID_T const * const mailist,
        uint32_t const mailist_size)
{
    IPC_TID_T const self = IPC_Self();
    union IPCHelper * helper = get_helper();
    union Mailbox * const mbx = IPCHelper_find_mailbox(helper, self);
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
            rc = Mailbox_retrieve_only(mbx, mail, mailist[i]);
        }
        IPC_Sleep(50);// do something else
    } while( !rc && !IPC_Clock_Elapsed(timestamp));

    return rc;
}

void IPC_Send(IPC_TID_T const rcv_tid, IPC_MID_T const mid, void const * const payload, size_t const pay_size)
{
    union IPCHelper * helper = get_helper();
    union Mailbox * const mbx = IPCHelper_find_mailbox(helper, rcv_tid);
    if(NULL != mbx)
    {
        Dbg_Info("%s: thread %d send mail %d to thread %d", __func__, IPC_Self(), mid, rcv_tid);
        union Mail mail;
        Mail_populate(&mail, mid, IPC_Self(), rcv_tid, payload, pay_size);
        Mailbox_push_mail(mbx, &mail);
    }
}

void IPC_Publish(IPC_MID_T const mid, void const * const payload, size_t const pay_size)
{
    Publisher_Publish(mid, payload, pay_size);
}

IPC_Clock_T IPC_Clock(void)
{
    union IPCHelper * helper= get_helper();
    return Clock_clock(helper->clock);
}


bool IPC_Clock_Elapsed(IPC_Clock_T const clock_ms)
{
    return clock_ms < IPC_Clock();
}

void IPC_Sleep(IPC_Clock_T const ms)
{
    union IPCHelper * helper = get_helper();
    Clock_sleep(helper->clock, ms);
}
