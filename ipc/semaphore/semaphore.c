#include "semaphore-internal.h"

void semaphore_override(union Semaphore_Class * const clazz)
{}

void Populate_Semaphore(union Semaphore * const semaphore)
{
    Object_populate(&semaphore->Object, &Get_Semaphore_Class()->Class);
}


