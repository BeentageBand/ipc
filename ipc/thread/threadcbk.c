#include "thread.h"
#include "threadcbk-internal.h"

void threadcbk_override(union ThreadCbk_Class * const clazz)
{}

void ThreadCbk_populate(union ThreadCbk * const threadcbk)
{
    Object_populate(&threadcbk->Object, &Get_ThreadCbk_Class()->Class);
}