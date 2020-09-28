#include "publishersingleton.h"
#include "publisher.h"
#include "logger/logger.h"
#include "ipc/ipc.h"

#define Dbg_Warn(...) Log_warn(get_log(), __VA_ARGS__)

static union Logger * get_log(void);
static union PublisherSingleton * get_publisher(void);

union Logger * get_log(void)
{
    static union Logger log = {NULL};
    if (NULL == log.vtbl)
    {
        Logger_populate(&log, NULL, NULL);
    }
    return &log;
}

union PublisherSingleton * get_publisher(void)
{
    static union PublisherSingleton singleton = {NULL};
    if (NULL == singleton.vtbl)
    {
        Publisher_populate(&singleton);
    }
    return &singleton;
}

bool Publisher_Subscribe(IPC_TID_T const tid, IPC_MID_T const mid)
{
    return PublisherSingleton_subscribe(get_publisher(), tid, mid);
}

bool Publisher_Unsubscribe(IPC_TID_T const tid, IPC_MID_T const mid)
{
    return PublisherSingleton_unsubscribe(get_publisher(), tid, mid);
}

void Publisher_Publish(IPC_MID_T const mid, void const * const payload, size_t const pay_size)
{
    PublisherSingleton_publish(get_publisher(), mid, payload, pay_size);
}
