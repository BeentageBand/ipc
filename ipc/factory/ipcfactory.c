#include "ipcfactory-internal.h"

void ipcfactory_override(union IPCFactory_Class * const clazz)
{}

void IPCFactory_populate(union IPCFactory * const ipcfactory)
{
    Object_populate(&ipcfactory->Object, &Get_IPCFactory_Class()->Class);
}