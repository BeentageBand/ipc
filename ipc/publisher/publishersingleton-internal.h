#ifndef PUBLISHERSINGLETON_INT_H
#define PUBLISHERSINGLETON_INT_H
#define PUBLISHERSINGLETON_IMPLEMENTATION

#include "publishersingleton.h"

static void publishersingleton_override(union PublisherSingleton_Class * const publishersingleton);

union PublisherSingleton_Class * Get_PublisherSingleton_Class(void)
{
  static union PublisherSingleton_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  publishersingleton_override(&clazz);
  return &clazz;
}
bool PublisherSingleton_subscribe(union PublisherSingleton * const publishersingleton, IPC_TID_T const mbx, IPC_MID_T const mid)
{
  return publishersingleton->vtbl->subscribe(publishersingleton, mbx, mid);
}

bool PublisherSingleton_unsubscribe(union PublisherSingleton * const publishersingleton, IPC_TID_T const mbx, IPC_MID_T const mid)
{
  return publishersingleton->vtbl->unsubscribe(publishersingleton, mbx, mid);
}

void PublisherSingleton_publish(union PublisherSingleton * const publishersingleton, IPC_MID_T const mid, void * const payload, size_t const pay_size)
{
  return publishersingleton->vtbl->publish(publishersingleton, mid, payload, pay_size);
}


#endif /*PUBLISHERSINGLETON_INT_H*/
