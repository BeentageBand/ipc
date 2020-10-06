#define CQUEUE_T_IMPLEMENTATION
#include "logger/logger.h"
#include "mailbox-internal.h"

#define IPC_MAILBOX_LOCK_MS (200)

#define CQueue_Params Mail_T
#include "ctemplate-lib/queue/cqueue-int-template.h"
#undef CQueue_Params


#define Dbg_Verb(...) Logger_debug(get_log(), __VA_ARGS__)
#define Dbg_Info(...) Logger_info(get_log(), __VA_ARGS__)

static union Logger * get_log(void);
static void mailbox_delete(union Mailbox * const mailbox);
static void mailbox_push_mail(union Mailbox * const mailbox, union Mail * mail);
static bool mailbox_retrieve(union Mailbox * const mailbox, union Mail * mail);
static bool mailbox_retrieve_only(union Mailbox * const mailbox, union Mail * mail, IPC_MID_T const mid);
static void mailbox_dump_ipc(union CQueue_Mail_T * const mailbox, enum LogLevel const dbg_lvl);

void mailbox_override(union Mailbox_Class * const clazz)
{
    clazz->Class.destroy = (Class_Delete_T) mailbox_delete;
    clazz->push_mail = mailbox_push_mail;
    clazz->retrieve = mailbox_retrieve;
    clazz->retrieve_only = mailbox_retrieve_only;

}
union Logger * get_log(void)
{
    static union Logger log = {NULL};
    static union Formatter fmt = {NULL};
    static union LoggerHandler handler = {NULL};
    if (NULL == log.vtbl) {
        LoggerHandler_populate(&handler);
        Formatter_populate(&fmt, __FILE__, LOG_DEBUG_LEVEL, " ");
        Logger_populate(&log, &fmt, &handler);
    }
    return &log;
}

void mailbox_delete(union Mailbox * const mailbox)
{
    _delete(&mailbox->mailbox);
}

void mailbox_push_mail(union Mailbox * const mailbox, union Mail * mail)
{
    union CQueue_Mail_T * const mbx = &mailbox->mailbox;
    union Conditional * const cond = mailbox->conditional;
    union Mutex * const mutex = mailbox->mutex;

    Dbg_Info("%s:mid %d", __func__, mail->mid);

    if(Mutex_lock(mutex, IPC_MAILBOX_LOCK_MS))
    {
        CQueue_Mail_T_push_front(mbx, *mail);
        mailbox_dump_ipc(mbx , LOG_INFO_LEVEL);
        Conditional_signal(cond);
        Mutex_unlock(mutex);
    }
}

bool mailbox_retrieve(union Mailbox * const mailbox, union Mail * mail)
{
    union CQueue_Mail_T * const mbx = &mailbox->mailbox;
    union Conditional * const cond = mailbox->conditional;
    union Mutex * const mutex = mailbox->mutex;

    bool rc = false;
    if(Mutex_lock(mutex, IPC_MAILBOX_LOCK_MS))
    {
        while(0 == CQueue_Mail_T_size(mbx) &&
                Conditional_wait(cond, IPC_MAILBOX_LOCK_MS))
        {}//Wait until mailbox has a mail

        if(0 != CQueue_Mail_T_size(mbx))
        {
            //Copy from previous mail
            union Mail rcv = CQueue_Mail_T_back(mbx);
            Dbg_Verb("Mail %d found\n", rcv.mid);
            _copy(mail, &rcv);

            CQueue_Mail_T_pop_back(mbx);
            rc = true;
        }
        mailbox_dump_ipc(mbx, LOG_DEBUG_LEVEL);
        Mutex_unlock(mutex);
    }

    return rc;
}

bool mailbox_retrieve_only(union Mailbox * const mailbox, union Mail * mail, IPC_MID_T const mid)
{
    union CQueue_Mail_T * const mbx = &mailbox->mailbox;
    union Conditional * const cond = mailbox->conditional;
    union Mutex * const mutex = mailbox->mutex;
    bool rc = false;

    if(Mutex_lock(mutex, IPC_MAILBOX_LOCK_MS))
    {
        while(!CQueue_Mail_T_size(mbx) &&
                Conditional_wait(cond, IPC_MAILBOX_LOCK_MS))
        {}//Wait until mailbox has a mail

        if(CQueue_Mail_T_size(mbx))
        {

            union Mail * found = CQueue_Mail_T_begin(mbx);
            for(int i = 0; CQueue_Mail_T_at(mbx, i) != CQueue_Mail_T_end(mbx); ++i)
            {
                found = CQueue_Mail_T_at(mbx, i);
                if(mid == found->mid)
                {
                    break;
                }
            }

            if(found != CQueue_Mail_T_end(mbx))
            {
                _copy(mail, found);
                Dbg_Verb("Mail %d found\n", found->mid);
                CQueue_Mail_T_pop_back(mbx);
                mailbox_dump_ipc(mbx, LOG_INFO_LEVEL);
                rc = true;
            }
        }
        Mutex_unlock(mutex);
    }

    return rc;
}

void mailbox_dump_ipc(union CQueue_Mail_T * const mailbox, enum LogLevel const dbg_lvl)
{
    Dbg_Verb("[");
    for(int i = 0; CQueue_Mail_T_at(mailbox, i) != CQueue_Mail_T_end(mailbox);
            ++i)
    {
        union Mail * it = CQueue_Mail_T_at(mailbox, i);
        Dbg_Verb("%d, ", it->mid);
    }
    Dbg_Verb("]");
}

void Mailbox_populate(union Mailbox * const mailbox, IPC_TID_T const tid, union Mutex * const mutex, union Conditional * const conditional,
                      union MailboxCbk * const cbk)
{
    Object_populate(&mailbox->Object, &Get_Mailbox_Class()->Class);
    CQueue_Mail_T_populate(&mailbox->mailbox);
    mailbox->tid = tid;
    mailbox->mutex = mutex;
    mailbox->conditional = conditional;
    mailbox->cbk = cbk;
}
