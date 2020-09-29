#ifndef CONDITIONAL_INT_H
#define CONDITIONAL_INT_H
#define CONDITIONAL_IMPLEMENTATION

#include "conditional.h"

static void conditional_override(union Conditional_Class * const conditional);

union Conditional_Class * Get_Conditional_Class(void)
{
  static union Conditional_Class clazz;
  if (0 != clazz.Class.offset) return &clazz;
  Class_populate(&clazz.Class, sizeof(clazz), NULL);
  conditional_override(&clazz);
  return &clazz;
}
bool Conditional_signal(union Conditional * const conditional)
{
  return conditional->vtbl->signal(conditional);
}

bool Conditional_wait(union Conditional * const conditional, IPC_Clock_T const ms)
{
  return conditional->vtbl->wait(conditional, ms);
}


#endif /*CONDITIONAL_INT_H*/
