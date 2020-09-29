#ifndef CLOCK_INT_H
#define CLOCK_INT_H
#define CLOCK_IMPLEMENTATION

#include "clock.h"

static void clock_override(union Clock_Class * const clock);

union Clock_Class * Get_Clock_Class(void)
{
  static union Clock_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  clock_override(&clazz);
  return &clazz;
}
IPC_Clock_T Clock_clock(union Clock * const clock)
{
  return clock->vtbl->clock(clock);
}

bool Clock_is_clock_elapsed(union Clock * const clock, IPC_Clock_T const clock_ms)
{
  return clock->vtbl->is_clock_elapsed(clock, clock_ms);
}

void Clock_sleep(union Clock * const clock, IPC_Clock_T const ms)
{
  return clock->vtbl->sleep(clock, ms);
}


#endif /*CLOCK_INT_H*/
