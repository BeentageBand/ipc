#include "logger/logger.h"
#include "conditional-internal.h"

static void conditional_delete(union Conditional* const obj);


void conditional_override(union Conditional_Class * const clazz)
{
  clazz->Class.destroy = (Class_Delete_T) conditional_delete;
}

void conditional_delete(union Conditional * const conditional)
{
  _delete(conditional->mutex);
  conditional->mutex = NULL;
}

void Conditional_populate(union Conditional * const conditional, union Mutex * const mutex)
{

  Object_populate(&conditional->Object, &Get_Conditional_Class()->Class);
  conditional->mutex = mutex;
}
