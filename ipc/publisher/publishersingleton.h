#ifndef PUBLISHERSINGLETON_H
#define PUBLISHERSINGLETON_H
#include "ipc/common/ipc_types.h"

#define CHashSet_Params IPC_TID_T 
#include "ctemplate-lib/hashset/chashset-template.h"
#undef CHashSet_Params

typedef union CHashSet_IPC_TID_T * Subscription_T;

#define CHashMap_Params IPC_MID_T, Subscription_T
#include "ctemplate-lib/hashmap/chashmap-template.h"
#define CHashMap_Params

typedef union CHashMap_IPC_MID_T_Subscription_T Subscription_List_T;

#ifdef PUBLISHERSINGLETON_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union PublisherSingleton;
union PublisherSingleton_Class
{
    
    struct
    {
    struct Class Class;
    bool (* _private subscribe)(union PublisherSingleton * const publishersingleton, IPC_TID_T const mbx, IPC_MID_T const mid);
bool (* _private unsubscribe)(union PublisherSingleton * const publishersingleton, IPC_TID_T const mbx, IPC_MID_T const mid);
void (* _private publish)(union PublisherSingleton * const publishersingleton, IPC_MID_T const mid, void * const payload, size_t const pay_size);

    };
};

union PublisherSingleton
{
    union PublisherSingleton_Class * vtbl;
        struct
    {
      union Object Object;
      Subscription_List_T _private subscription_list;
    };
};

extern union PublisherSingleton_Class * Get_PublisherSingleton_Class(void);

extern void PublisherSingleton_populate(union PublisherSingleton * const publishersingleton);

extern bool PublisherSingleton_subscribe(union PublisherSingleton * const publishersingleton, IPC_TID_T const mbx, IPC_MID_T const mid);

extern bool PublisherSingleton_unsubscribe(union PublisherSingleton * const publishersingleton, IPC_TID_T const mbx, IPC_MID_T const mid);

extern void PublisherSingleton_publish(union PublisherSingleton * const publishersingleton, IPC_MID_T const mid, void * const payload, size_t const pay_size);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*PUBLISHERSINGLETON_H*/