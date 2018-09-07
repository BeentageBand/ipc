# pks_ipc
interprocess communication wrapper with mailboxing architecture

## create thread
1. On ipc_threads_ids.h, edit the following macro:
  ````
  IPC_THREAD_LIST(X) \
  X(THIS_THREAD_TID)
  ````
**Style Standard**: Enum naming convention is _\<THREAD_NAME\>\_\<WORKER\>\_TID_.

  Please specify if thread type is a worker.
2. Construct Thread/Worker Object. This Thread/Worker Object should be implemented (inherited)\*\*\*.
  On code, populate thread:
  ````
  Populate_<Thread_Name>(&this);
  ````
  \*\*\*_Please refer to cobject feature : how to inherit a class.
  _Inheriting Thread Object, please check how to implement overrides.
  _Inheriting Worker Object, please check how to implement overrides.

## run thread
3. On code run thread:
````
  IPC_Run(THIS_THREAD_ID);
````
