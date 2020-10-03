#ifndef THREADMOCK_H
#define THREADMOCK_H
#include "gmock-thread.h"
#include "ipc/thread/threadcbk.h"

#ifdef THREADMOCK_IMPLEMENTATION 
#define _private
#else
#define _private const
#endif 

#ifdef __cplusplus
extern "C" {
#endif

union ThreadMock;
union ThreadMock_Class
{
    union ThreadCbk_Class ThreadCbk;

    struct
    {
    struct Class Class;
    bool (* _private register_thread)(union ThreadMock * const threadmock, union Thread * const thread);
bool (* _private run_thread)(union ThreadMock * const threadmock, union Thread * const thread);
bool (* _private join_thread)(union ThreadMock * const threadmock, union Thread * const thread);
bool (* _private unregister_thread)(union ThreadMock * const threadmock, union Thread * const thread);

    };
};

union ThreadMock
{
    union ThreadMock_Class * vtbl;
    union ThreadCbk ThreadCbk;
    struct
    {
      union Object Object;
      mock::GThread * _private mock;

    };
};

extern union ThreadMock_Class * Get_ThreadMock_Class(void);

extern void ThreadMock_populate(union ThreadMock * const threadmock, mock::GThread * const mock);

extern bool ThreadMock_register_thread(union ThreadMock * const threadmock, union Thread * const thread);

extern bool ThreadMock_run_thread(union ThreadMock * const threadmock, union Thread * const thread);

extern bool ThreadMock_join_thread(union ThreadMock * const threadmock, union Thread * const thread);

extern bool ThreadMock_unregister_thread(union ThreadMock * const threadmock, union Thread * const thread);

#ifdef __cplusplus
}
#endif
#undef _private
#endif /*THREADMOCK_H*/