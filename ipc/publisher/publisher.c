#include "publishersingleton.h"
#include "publisher.h"
#include "logger/logger.h"
#include "ipc/ipc.h"

#define Dbg_Warn(...) Logger_warn(get_log(), __VA_ARGS__)

static union Logger * get_log(void);
static union PublisherSingleton * get_publisher(void);

union Logger * get_log(void)
{
    static union Logger log = {NULL};
    static union Formatter fmt = {NULL};
    static union LoggerHandler handler = {NULL};
    if (NULL == log.vtbl) {
        LoggerHandler_populate(&handler);
        Formatter_populate(&fmt, __FILE__, LOG_DEBUG_LEVEL, " ");
        Logger_populate(&log, &fmt, &handler);
    }
    return &log;
}

union PublisherSingleton * get_publisher(void)
{
    static union PublisherSingleton singleton = {NULL};
    if (NULL == singleton.vtbl)
    {
        PublisherSingleton_populate(&singleton);
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
