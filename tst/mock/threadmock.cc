#include "threadmock-internal.h"

static bool threadmock_register_thread(union ThreadMock * const threadmock, union Thread * const thread);
static bool threadmock_run_thread(union ThreadMock * const threadmock, union Thread * const thread);
static bool threadmock_join_thread(union ThreadMock * const threadmock, union Thread * const thread);
static bool threadmock_unregister_thread(union ThreadMock * const threadmock, union Thread * const thread);

void threadmock_override(union ThreadMock_Class * const clazz)
{
    clazz->join_thread = threadmock_join_thread;
    clazz->run_thread = threadmock_run_thread;
    clazz->unregister_thread = threadmock_unregister_thread;
    clazz->register_thread = threadmock_register_thread;
}

bool threadmock_register_thread(union ThreadMock * const threadmock, union Thread * const thread)
{
    return 0 == threadmock->mock->register_thread(*thread);
}

bool threadmock_run_thread(union ThreadMock * const threadmock, union Thread * const thread)
{
    return 0 == threadmock->mock->create_thread(*thread);
}

bool threadmock_join_thread(union ThreadMock * const threadmock, union Thread * const thread)
{
    return 0 == threadmock->mock->join_thread();
}

bool threadmock_unregister_thread(union ThreadMock * const threadmock, union Thread * const thread)
{
    void * exit_code = nullptr;
    return 0 == threadmock->mock->cancel_thread(exit_code);
}

void ThreadMock_populate(union ThreadMock * const threadmock, mock::GThread * const mock)
{
    ThreadCbk_populate(&threadmock->ThreadCbk);
    Object_populate(&threadmock->Object, &Get_ThreadMock_Class()->Class);
    threadmock->mock = mock;
}