#ifndef TIMER_INT_H
#define TIMER_INT_H
#define TIMER_IMPLEMENTATION

#include "timer.h"

static void timer_override(union Timer_Class * const timer);

union Timer_Class * Get_Timer_Class(void)
{
  static union Timer_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  timer_override(&clazz);
  return &clazz;
}
bool Timer_start(union Timer * const timer)
{
  return timer->vtbl->start(timer);
}

bool Timer_reset(union Timer * const timer)
{
  return timer->vtbl->reset(timer);
}

bool Timer_stop(union Timer * const timer)
{
  return timer->vtbl->stop(timer);
}

void Timer_on_timeout(union Timer * const timer)
{
  return timer->vtbl->on_timeout(timer);
}

bool Timer_set_time(union Timer * const timer, IPC_Clock_T const ms)
{
  return timer->vtbl->set_time(timer, ms);
}


#endif /*TIMER_INT_H*/
