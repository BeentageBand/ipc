#include "ipcgtestworker-internal.h"
#include "ipc/ipc.h"
#include "logger/logger.h"
#include "tst/mailist.h"
 
#include <unistd.h>

#define Dbg_Info(...) Logger_info(get_log(), __VA_ARGS__)
 
static union Logger * get_log(void);
static void ipcgtestworker_delete(union IPCGTestWorker * const ipcgtestworker);
static void ipcgtestworker_on_mail(union IPCGTestWorker * const ipcgtestworker, union Mail * const mail);
static void ipcgtestworker_on_start(union IPCGTestWorker * const ipcgtestworker);
static void ipcgtestworker_on_loop(union IPCGTestWorker * const ipcgtestworker);
static void ipcgtestworker_on_stop(union IPCGTestWorker * const ipcgtestworker);
 
union Logger * get_log(void)
{
	static union Logger log = {NULL};
	if (NULL == log.vtbl)
	{
		Logger_populate(&log, NULL, NULL);
	}
	return &log;
}

void ipcgtestworker_override(union IPCGTestWorker_Class * const clazz)
{
	clazz->Class.destroy = (Class_Delete_T) ipcgtestworker_delete;
	clazz->on_mail = ipcgtestworker_on_mail;
	clazz->on_start = ipcgtestworker_on_start;
	clazz->on_loop = ipcgtestworker_on_loop;
	clazz->on_stop = ipcgtestworker_on_stop;
}


void ipcgtestworker_delete(union IPCGTestWorker * const ipcgtestworker)
{
}
 
void IPCGTestWorker_populate(union IPCGTestWorker * const ipcgtestworker, union ThreadCbk *  const cbk, bool const ready, 
		IPC_TID_T const id, 
		union Mutex * const mux, 
		union Conditional * const cv, 
		union Mailbox * const mailbox, 
		IPC_MID_T const shutdown_mid)
{
	Worker_populate(&ipcgtestworker->Worker, cbk, id, mux, cv, mailbox, shutdown_mid);
}

void ipcgtestworker_on_start(union IPCGTestWorker * const ipcgtestworker)
{
   Dbg_Info("IPC_Gtest_Worker_on_start %d", ipcgtestworker->id);
}

void ipcgtestworker_on_mail(union IPCGTestWorker * const ipcgtestworker, union Mail * const mail)
{
	Dbg_Info("Worker %d: mail not null mid %d", ipcgtestworker->id, mail->mid);

	switch(mail->mid)
	{
	case IPC_GTEST_INT_MID:
	   Dbg_Info("Gtest Worker  Handles mid IPC_GTEST_EV_MID");
	   IPC_Send(GTEST_FWK_WORKER_TID, IPC_GTEST_INT_MID, NULL, 0);
	   break;
	default: break;
	}
}

void ipcgtestworker_on_loop(union IPCGTestWorker * const ipcgtestworker)
{
    IPC_Sleep(1000);
}

void ipcgtestworker_on_stop(union IPCGTestWorker * const ipcgtestworker)
{
   Dbg_Info("IPC_Gtest_Worker_on_stop %d", ipcgtestworker->id);
}
