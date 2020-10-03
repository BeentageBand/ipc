#define CHASHSET_T_IMPLEMENTATION
#define CHASHMAP_T_IMPLEMENTATION
#include "publishersingleton-internal.h"
#include "publisher.h"
#include "ipc/ipc.h"
#include "logger/logger.h"
#include "ipc/mail/mail.h"

#define CHashSet_Params IPC_TID_T
#include "ctemplate-lib/hashset/chashset-int-template.h"
#undef CHashSet_Params 


#define CHashMap_Params IPC_MID_T, Subscription_T
#include "ctemplate-lib/hashmap/chashmap-int-template.h"
#undef CHashMap_Params 

#define Dbg_Info(...) Logger_info(get_log(), __VA_ARGS__)

static union Logger * get_log(void);
static void publishersingleton_delete(union PublisherSingleton * const publishersingleton);
static bool publishersingleton_subscribe(union PublisherSingleton * const publishersingleton, IPC_TID_T const mbx, IPC_MID_T const mid);
static bool publishersingleton_unsubscribe(union PublisherSingleton * const publishersingleton, IPC_TID_T const mbx, IPC_MID_T const mid);
static void publishersingleton_publish(union PublisherSingleton * const publishersingleton, IPC_MID_T const mid, void * const payload, size_t const pay_size);
static void publisher_dump_subscription(Subscription_List_T * const subscription, IPC_TID_T const tid);

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

void publishersingleton_override(union PublisherSingleton_Class * const clazz)
{
    clazz->Class.destroy = (Class_Delete_T) publishersingleton_delete;
    clazz->publish = publishersingleton_publish;
    clazz->subscribe = publishersingleton_subscribe;
    clazz->unsubscribe = publishersingleton_unsubscribe;
}

void publishersingleton_delete(union PublisherSingleton * const publishersingleton)
{
    _delete(&publishersingleton->subscription_list);
}

bool publishersingleton_subscribe(union PublisherSingleton * const publishersingleton, IPC_TID_T mbx, IPC_MID_T const mid)
{
    HashPair_IPC_MID_T_Subscription_T * found = 
        CHashMap_IPC_MID_T_Subscription_T_find(&publishersingleton->subscription_list, mid);
    if (CHashMap_IPC_MID_T_Subscription_T_end(&publishersingleton->subscription_list) == found)
    {
        Subscription_T sub = _new(union CHashSet_IPC_TID_T);
        CHashSet_IPC_TID_T_populate(sub, NULL, NULL);
        CHashSet_IPC_TID_T_insert(sub, mbx);
        CHashMap_IPC_MID_T_Subscription_T_insert(&publishersingleton->subscription_list, mid, sub);

        HashPair_IPC_MID_T_Subscription_T * found = 
        CHashMap_IPC_MID_T_Subscription_T_find(&publishersingleton->subscription_list, mid);

        return found != CHashMap_IPC_MID_T_Subscription_T_end(&publishersingleton->subscription_list) &&
            NULL != found->value && CHashSet_IPC_TID_T_find(found->value, mbx) != CHashSet_IPC_TID_T_end(found->value);
    }
    else
    {
        CHashSet_IPC_TID_T_insert(found->value, mbx);
        return CHashSet_IPC_TID_T_find(found->value, mbx) != CHashSet_IPC_TID_T_end(found->value);
    }

}

bool publishersingleton_unsubscribe(union PublisherSingleton * const publishersingleton, IPC_TID_T mbx, IPC_MID_T const mid)
{
    HashPair_IPC_MID_T_Subscription_T * found = 
        CHashMap_IPC_MID_T_Subscription_T_find(&publishersingleton->subscription_list, mid);
    if (CHashMap_IPC_MID_T_Subscription_T_end(&publishersingleton->subscription_list) == found)
    {
        return false;
    }
    else
    {
        CHashSet_IPC_TID_T_erase(found->value, mbx);
        if (0 == CHashSet_IPC_TID_T_size(found->value))
        {
            _delete(found->value);
            free(found->value);
            CHashMap_IPC_MID_T_Subscription_T_erase(&publishersingleton->subscription_list, mid);
        }
        return true;
    }
}

void publishersingleton_publish(union PublisherSingleton * const publishersingleton, IPC_MID_T const mid, void * const payload, size_t const pay_size)
{

    HashPair_IPC_MID_T_Subscription_T * found = 
        CHashMap_IPC_MID_T_Subscription_T_find(&publishersingleton->subscription_list, mid);

    if (CHashMap_IPC_MID_T_Subscription_T_end(&publishersingleton->subscription_list) == found) return ;

    Subscription_T sub = found->value;
    if (NULL == sub) return;

    for (struct Bucket_IPC_TID_T * it = CHashSet_IPC_TID_T_begin(sub); it != CHashSet_IPC_TID_T_end(sub); ++it)
    {
        if (!Bucket_IPC_TID_T_is_empty(it))
        {
            IPC_Send(it->value, mid, payload, pay_size);
        }
    }
}

void publisher_dump_subscription(Subscription_List_T * const subscription, IPC_MID_T const mid)
{

    Dbg_Info("Subscription %d:\n", mid);

    HashPair_IPC_MID_T_Subscription_T * found = 
        CHashMap_IPC_MID_T_Subscription_T_find(subscription, mid);

    if (CHashMap_IPC_MID_T_Subscription_T_end(subscription) == found) return ;

    Subscription_T sub = found->value;
    if (NULL == sub) return;

    for (struct Bucket_IPC_TID_T * it = CHashSet_IPC_TID_T_begin(sub); it != CHashSet_IPC_TID_T_end(sub); ++it)
    {
        if (!Bucket_IPC_TID_T_is_empty(it))
        {
            Dbg_Info("%d,", it->value);
        }
    }
}

void PublisherSingleton_populate(union PublisherSingleton * const publishersingleton)
{
    Object_populate(&publishersingleton->Object, &Get_PublisherSingleton_Class()->Class);
    CHashMap_IPC_MID_T_Subscription_T_populate(&publishersingleton->subscription_list, NULL, NULL);
}