#ifndef THREADCBK_H
#define THREADCBK_H
#include <stdbool.h>
#include "cobject/cobject.h"

#ifdef THREADCBK_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union Thread;
union ThreadCbk;
union ThreadCbk_Class
{
    
    struct
    {
    struct Class Class;
    bool (* _private register_thread)(union ThreadCbk * const threadcbk, union Thread * const thread);
bool (* _private run_thread)(union ThreadCbk * const threadcbk, union Thread * const thread);
bool (* _private join_thread)(union ThreadCbk * const threadcbk, union Thread * const thread);
bool (* _private unregister_thread)(union ThreadCbk * const threadcbk, union Thread * const thread);

    };
};

union ThreadCbk
{
    union ThreadCbk_Class * vtbl;
        struct
    {
      union Object Object;
      
    };
};

extern union ThreadCbk_Class * Get_ThreadCbk_Class(void);

extern void ThreadCbk_populate(union ThreadCbk * const threadcbk);

extern bool ThreadCbk_register_thread(union ThreadCbk * const threadcbk, union Thread * const thread);

extern bool ThreadCbk_run_thread(union ThreadCbk * const threadcbk, union Thread * const thread);

extern bool ThreadCbk_join_thread(union ThreadCbk * const threadcbk, union Thread * const thread);

extern bool ThreadCbk_unregister_thread(union ThreadCbk * const threadcbk, union Thread * const thread);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*THREADCBK_H*/