#define COBJECT_IMPLEMENTATION
#define Dbg_FID DBG_FID_DEF(IPC_FID, 0)

#include "dbg_log.h"
#include "ipc_helper.h"

typedef union Thread * Thread_Ptr_T;

#define CSet_Params Thread_Ptr
#include "cset.c"
#undef CSet_Params

typedef union Mailbox * Mailbox_Ptr_T;

#define CSet_Params Mailbox_Ptr
#include "cset.c"
#undef CSet_Params

static void ipc_helper_delete(struct Object * const obj);

static IPC_Clock_T ipc_helper_time(union IPC_Helper * const helper);
static void ipc_helper_sleep(union IPC_Helper * const helper, IPC_Clock_T const sleep_ms);
static bool ipc_helper_is_time_elapsed(union IPC_Helper * const helper, IPC_Clock_T const time_ms);

static IPC_TID_T ipc_helper_self_thread(union IPC_Helper * const helper);
static bool ipc_helper_alloc_thread(union IPC_Helper * const helper, union Thread * const thread);
static bool ipc_helper_free_thread(union IPC_Helper * const helper, union Thread * const thread);
static bool ipc_helper_run_thread(union IPC_Helper * const helper, union Thread * const thread);
static bool ipc_helper_join_thread(union IPC_Helper * const helper, union Thread * const thread);

static bool ipc_helper_alloc_mutex(union IPC_Helper * const helper, union Mutex * const mutex);
static bool ipc_helper_free_mutex(union IPC_Helper * const helper, union Mutex * const mutex);
static bool ipc_helper_lock_mutex(union IPC_Helper * const helper, union Mutex * const mutex,
        IPC_Clock_T const wait_ms);
static bool ipc_helper_unlock_mutex(union IPC_Helper * const helper, union Mutex * const mutex);

static bool ipc_helper_alloc_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore,
        uint8_t const value);
static bool ipc_helper_free_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore);
static bool ipc_helper_wait_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore, 
        IPC_Clock_T const wait_ms);
static bool ipc_helper_post_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore);

static bool ipc_helper_alloc_conditional(union IPC_Helper * const helper, union Conditional * const conditional);
static bool ipc_helper_free_conditional(union IPC_Helper * const helper, union Conditional * const conditional);
static bool ipc_helper_wait_conditional(union IPC_Helper * const helper, union Conditional * const conditional,
        union Mutex * const mutex, IPC_Clock_T const wait_ms);
static bool ipc_helper_post_conditional(union IPC_Helper * const helper, union Conditional * const conditional);

static int ipc_helper_thread_cmp(Thread_Ptr_T const * a, Thread_Ptr_T const *b);
static int ipc_helper_mailbox_cmp(Mailbox_Ptr_T const * a,Mailbox_Ptr_T const *b);

IPC_Helper_Class_T IPC_Helper_Class = 
{{
        { ipc_helper_delete, NULL},
        ipc_helper_time,
        ipc_helper_sleep,
        ipc_helper_is_time_elapsed,

        ipc_helper_self_thread,
        ipc_helper_alloc_thread,
        ipc_helper_free_thread,
        ipc_helper_run_thread,
        ipc_helper_join_thread,

        ipc_helper_alloc_mutex,
        ipc_helper_free_mutex,
        ipc_helper_lock_mutex,
        ipc_helper_unlock_mutex,

        ipc_helper_alloc_semaphore,
        ipc_helper_free_semaphore,
        ipc_helper_wait_semaphore,
        ipc_helper_post_semaphore,

        ipc_helper_alloc_conditional,
        ipc_helper_free_conditional,
        ipc_helper_wait_conditional,
        ipc_helper_post_conditional
}};

static union IPC_Helper IPC_Helper = {NULL};
static union IPC_Helper * IPC_Helper_Singleton = NULL;
static CSet_Thread_Ptr_T rthreads;
static CSet_Mailbox_Ptr_T rmailboxes;
static Thread_Ptr_T thread_set[IPC_MAX_TID] = {0};
static Mailbox_Ptr_T mailbox_set[IPC_MAX_TID] = {0};


void ipc_helper_delete(struct Object * const obj)
{
    IPC_Helper_T * this = (IPC_Helper_T *) Object_Cast(&IPC_Helper_Class.Class, obj);

    this->rthreads = NULL;
    this->rmailboxes = NULL;
}

IPC_Clock_T ipc_helper_time(union IPC_Helper * const helper)
{
    union IPC_Helper * this = helper;
    IPC_Clock_T clock = 0;
    while(this)
    {
        if(ipc_helper_time != this->vtbl->time)
        {
            clock = this->vtbl->time(this);
        }

        this = this->next;
    }
    return clock;
}

void ipc_helper_sleep(union IPC_Helper * const helper, IPC_Clock_T const sleep_ms)
{
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_sleep != this->vtbl->sleep)
        {
            this->vtbl->sleep(this, sleep_ms);
        }
        this = this->next;
    }
}

bool ipc_helper_is_time_elapsed(union IPC_Helper * const helper, IPC_Clock_T const time_ms)
{
    union IPC_Helper * this = helper;
    bool  rc = true;
    while(this)
    {
        if(ipc_helper_is_time_elapsed!= this->vtbl->is_time_elapsed)
        {
            rc = this->vtbl->is_time_elapsed(this, time_ms);
        }
    }
    return rc;
}

IPC_TID_T ipc_helper_self_thread(union IPC_Helper * const helper)
{
    IPC_TID_T tid = IPC_MAX_TID;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_self_thread != this->vtbl->self_thread)
        {
            tid = this->vtbl->self_thread(this);
        }

        this = this->next;
    }
    return tid;
}

bool ipc_helper_alloc_thread(union IPC_Helper * const helper, union Thread * const thread)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_alloc_thread != this->vtbl->alloc_thread)
        {
            rc = this->vtbl->alloc_thread(this, thread);
        }

        this = this->next;
    }
    return rc;
}


bool ipc_helper_free_thread(union IPC_Helper * const helper, union Thread * const thread)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_free_thread != this->vtbl->free_thread)
        {
            rc = this->vtbl->free_thread(this, thread);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_run_thread(union IPC_Helper * const helper, union Thread * const thread)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_run_thread != this->vtbl->run_thread)
        {
            rc = this->vtbl->run_thread(this, thread);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_join_thread(union IPC_Helper * const helper, union Thread * const thread)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_join_thread != this->vtbl->join_thread)
        {
            rc = this->vtbl->join_thread(this, thread);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_alloc_mutex(union IPC_Helper * const helper, union Mutex * const mutex)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_alloc_mutex != this->vtbl->alloc_mutex)
        {
            rc = this->vtbl->alloc_mutex(this, mutex);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_free_mutex(union IPC_Helper * const helper, union Mutex * const mutex)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_free_mutex != this->vtbl->free_mutex)
        {
            rc = this->vtbl->free_mutex(this, mutex);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_lock_mutex(union IPC_Helper * const helper, union Mutex * const mutex,
        IPC_Clock_T const wait_ms)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_lock_mutex != this->vtbl->lock_mutex)
        {
            rc = this->vtbl->lock_mutex(this, mutex, wait_ms);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_unlock_mutex(union IPC_Helper * const helper, union Mutex * const mutex)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_unlock_mutex != this->vtbl->unlock_mutex)
        {
            rc = this->vtbl->unlock_mutex(this, mutex);
        }

        this = this->next;
    }
    return rc;
}


bool ipc_helper_alloc_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore,
        uint8_t const value)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_alloc_semaphore!= this->vtbl->alloc_semaphore)
        {
            rc = this->vtbl->alloc_semaphore(this, semaphore, value);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_free_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_free_semaphore != this->vtbl->free_semaphore)
        {
            rc = this->vtbl->free_semaphore(this, semaphore);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_wait_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore, 
        IPC_Clock_T const wait_ms)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_wait_semaphore != this->vtbl->wait_semaphore)
        {
            rc = this->vtbl->wait_semaphore(this, semaphore, wait_ms);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_post_semaphore(union IPC_Helper * const helper, union Semaphore * const semaphore)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_post_semaphore != this->vtbl->post_semaphore)
        {
            rc = this->vtbl->post_semaphore(this, semaphore);
        }

        this = this->next;
    }
    return rc;
}


bool ipc_helper_alloc_conditional(union IPC_Helper * const helper, union Conditional * const conditional)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_alloc_conditional != this->vtbl->alloc_conditional)
        {
            rc = this->vtbl->alloc_conditional(this, conditional);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_free_conditional(union IPC_Helper * const helper, union Conditional * const conditional)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_free_conditional != this->vtbl->free_conditional)
        {
            rc = this->vtbl->free_conditional(this, conditional);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_wait_conditional(union IPC_Helper * const helper, union Conditional * const conditional,
        union Mutex * const mutex, IPC_Clock_T const wait_ms)
{
    bool rc = false;
    union IPC_Helper* this = helper;
    while(this)
    {
        if(ipc_helper_wait_conditional != this->vtbl->wait_conditional)
        {
            rc = this->vtbl->wait_conditional(this, conditional, mutex, wait_ms);
        }

        this = this->next;
    }
    return rc;
}

bool ipc_helper_post_conditional(union IPC_Helper * const helper, union Conditional * const conditional)
{
    bool rc = false;
    union IPC_Helper * this = helper;
    while(this)
    {
        if(ipc_helper_wait_conditional != this->vtbl->wait_conditional)
        {
            rc = this->vtbl->post_conditional(this, conditional);
        }

        this = this->next;
    }
    return rc;
}

union IPC_Helper * IPC_get_instance(void)
{
        union IPC_Helper this;
        Populate_IPC_Helper(&this);
        IPC_Helper_Singleton = &IPC_Helper;
        return IPC_Helper_Singleton;
}

int ipc_helper_thread_cmp(Thread_Ptr_T const * a, Thread_Ptr_T const * b)
{
    if((*a)->tid < (*b)->tid) return -1;
    if((*a)->tid ==  (*b)->tid) return 0;
    if((*a)->tid >  (*b)->tid) return 1;

    return 0;
}

int ipc_helper_mailbox_cmp(Mailbox_Ptr_T const * a,Mailbox_Ptr_T const *b)
{
    if((*a)->tid <  (*b)->tid) return -1;
    if((*a)->tid == (*b)->tid) return 0;
    if((*a)->tid >  (*b)->tid) return 1;

    return 0;
}

void Populate_IPC_Helper(union IPC_Helper * const this)
{
    if(NULL == IPC_Helper.vtbl)
    {
        memset(thread_set, 0, sizeof(thread_set));
        memset(mailbox_set, 0, sizeof(mailbox_set));

        IPC_Helper.vtbl = &IPC_Helper_Class;
        Populate_CSet_Cmp_Thread_Ptr(&rthreads, thread_set, Num_Elems(thread_set),
                (CSet_Cmp_T) ipc_helper_thread_cmp);
        Populate_CSet_Cmp_Mailbox_Ptr(&rmailboxes, mailbox_set, Num_Elems(mailbox_set),
                (CSet_Cmp_T) ipc_helper_mailbox_cmp);
        IPC_Helper.rthreads = &rthreads;
        IPC_Helper.rmailboxes = &rmailboxes;
    }

    memcpy(this, &IPC_Helper, sizeof(IPC_Helper));
}

union Thread * IPC_Helper_find_thread(IPC_TID_T const thread)
{
        (void)IPC_get_instance();
        Isnt_Nullptr(IPC_Helper_Singleton->rthreads, NULL);

        union Thread t = {NULL};
        t.tid = thread;

        CSet_Thread_Ptr_T * const thread_stack = IPC_Helper_Singleton->rthreads;
        Thread_Ptr_T * found = thread_stack->vtbl->find(thread_stack, &t);

        bool is_found = (found != thread_stack->vtbl->end(thread_stack));
        Dbg_Verb("%s tid %d %s found!!\n", __func__, thread, (is_found)? "is": "is not");
        return (is_found)? *found : NULL;
}

union Mailbox * IPC_Helper_find_mailbox(IPC_TID_T const mailbox)
{
        (void)IPC_get_instance();
        Isnt_Nullptr(IPC_Helper_Singleton->rmailboxes, NULL);

        union Mailbox m = {NULL};
        Populate_Mailbox(&m, mailbox, NULL, 0);

        CSet_Mailbox_Ptr_T * const mailbox_stack = IPC_Helper_Singleton->rmailboxes;
        Mailbox_Ptr_T * found = mailbox_stack->vtbl->find(mailbox_stack, &m);

        bool is_found = (found != mailbox_stack->vtbl->end(mailbox_stack));
        Dbg_Verb("%s tid %d %s found!!\n", __func__, mailbox, (is_found)? "is": "is not");
        return (is_found)? *found : NULL;
}

void IPC_Helper_Append(union IPC_Helper * appendable)
{
    union IPC_Helper * this = IPC_get_instance();

    if(this == appendable) return;
    while(this->next)
    {
        if(this == appendable)
        {
            return;
        }
        this = this->next;
    }

    this->next = appendable;
}
