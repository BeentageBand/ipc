#ifndef CONDITIONAL_H_
#define CONDITIONAL_H_
 
#include "mutex.h"
 
#ifdef __cplusplus
extern "C" {
#endif

typedef union Conditional_Cbk
{
      struct Conditional_Cbk_Class _private * _private vtbl;
      struct Object Object;
}Conditional_Cbk_T;

typedef struct Conditional_Cbk_Class
{
      struct Class Class;
      bool (* _private wait)(union Conditional_Cbk * const, union Conditional * const,
            union Conditional * const, IPC_Clock_T const wait_ms);
      bool (* _private post)(union Conditional_Cbk * const, union Conditional * const);
}Conditional_Cbk_Class;

typedef union Conditional
{
   struct Conditional_Class _private * _private vtbl;
   struct
   {
      struct Object Object;
      union Mutex _private * _private mutex;
      void _private * _private conditional;
   };
}Conditional_T;

typedef struct Conditional_Class
{
      struct Class Class;
      bool_t (* _private wait)(union Conditional * const, IPC_Clock_T const);
      bool_t (* _private signal)(union Conditional * const);
}Conditional_Class_T;

extern Conditional_Class_T _private Conditional_Class;
extern struct Conditional_Cbk_Class _private Conditional_Cbk_Class;

extern void Populate_Conditional(union Conditional * const conditional, union Mutex * const mux);

#ifdef __cplusplus
}
#endif
#endif /*CONDITIONAL_H_*/
