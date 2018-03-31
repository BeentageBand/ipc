/*
 * ipc_posix.h
 *
 *  Created on: Jan 27, 2018
 *      Author: uids6144
 */

#ifndef IPC_POSIX_H_
#define IPC_POSIX_H_

#include "ipc_helper.h"

typedef union IPC_Helper_Class IPC_POSIX_Class_T;

typedef union IPC_POSIX
{
  IPC_POSIX_Class_T _private * _private vtbl;
  struct Object Object;
  struct
  {
    union IPC_Helper IPC_Helper;
  };
}IPC_POSIX_T;

extern IPC_POSIX_Class_T IPC_POSIX_Class;

extern void Populate_IPC_POSIX(union IPC_POSIX * const posix);

#endif /* IPC_POSIX_H_ */
