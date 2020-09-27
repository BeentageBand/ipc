#include "timer-internal.h"

void timer_override(union Timer_Class * const clazz)
{
}

void Populate_Timer(union Timer * const timer)
{
  Object_populate(&timer->Object, &Get_Timer_Class()->Class);
}