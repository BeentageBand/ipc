#include "clock-internal.h"

void clock_override(union Clock_Class * const clazz)
{}

void Clock_populate(union Clock * const clock, union IPCFactory * const factory)
{
    Object_populate(&clock->Object, &Get_Clock_Class()->Class);
    clock->factory = factory;
}