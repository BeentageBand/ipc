#ifndef MAILIST_H_
#define MAILIST_H_

#include "ipc/worker/worker_mailist.h"
#include "ipcgtestworker/ipcgtestworker_mailist.h"

#define IPC_ENUM(id, description) id,

enum IPC_MID
{
    IPC_MID_INT_BEGIN = 0,
    IPC_GTEST_INT_MAILIST(IPC_ENUM)
    IPC_MID_INT_END,
    IPC_MID_PBC_BEGIN = IPC_MID_INT_END,
    IPC_GTEST_PBC_MAILIST(IPC_ENUM)
    IPC_MID_MAX
};

enum IPC_TID
{
    IPC_MAIN_TID = 0,
    GTEST_FWK_WORKER_TID,
    IPC_GTEST_1_WORKER_TID,
    IPC_TID_MAX
};

#endif /*MAILIST_H_*/