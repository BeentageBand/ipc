#include "barrier-internal.h"

void barrier_override(union Barrier_Class * const clazz)
{}

void Barrier_populate(union Barrier * const barrier)
{
    Object_populate(&barrier->Object, &Get_Barrier_Class()->Class);
}