 ACLOCAL_AMFLAGS = -I m4 --install
# Libraries
lib_LTLIBRARIES = libipc-1.0.0.la
libipc_1_0_0_la_SOURCES = \
ipc/ipc.c                                       \
ipc/ipc.h                                       \
ipc/clock/clock.c                               \
ipc/clock/clock.h                               \
ipc/clock/clock-internal.h                      \
ipc/conditional/conditional.c                   \
ipc/conditional/conditional.h                   \
ipc/conditional/conditional-internal.h          \
ipc/factory/ipcfactory.c                        \
ipc/factory/ipcfactory.h                        \
ipc/factory/ipcfactory-internal.h               \
ipc/helper/ipchelper.c                          \
ipc/helper/ipchelper.h                          \
ipc/helper/ipchelper-internal.h                 \
ipc/mail/mail.c                                 \
ipc/mail/mail.h                                 \
ipc/mail/mail-internal.h                        \
ipc/mailbox/mailbox.c                           \
ipc/mailbox/mailbox.h                           \
ipc/mailbox/mailbox-internal.h                  \
ipc/mailbox/mailboxcbk.c                        \
ipc/mailbox/mailboxcbk.h                        \
ipc/mailbox/mailboxcbk-internal.h               \
ipc/mutex/mutex.c                               \
ipc/mutex/mutex.h                               \
ipc/mutex/mutex-internal.h                      \
ipc/publisher/publisher.c                       \
ipc/publisher/publisher.h                       \
ipc/publisher/publishersingleton.c              \
ipc/publisher/publishersingleton.h              \
ipc/publisher/publishersingleton-internal.h     \
ipc/semaphore/semaphore.c                       \
ipc/semaphore/semaphore.h                       \
ipc/semaphore/semaphore-internal.h              \
ipc/thread/thread.c                             \
ipc/thread/thread.h                             \
ipc/thread/thread-internal.h                    \
ipc/thread/threadcbk.c                          \
ipc/thread/threadcbk.h                          \
ipc/thread/threadcbk-internal.h                 \
ipc/timer/timer.c                               \
ipc/timer/timer.h                               \
ipc/timer/timer-internal.h                      \
ipc/worker/worker.c                             \
ipc/worker/worker.h                             \
ipc/worker/worker-internal.h

libipc_1_0_0_la_LDFLAGS = -version-info 0:0:0
nobase_include_HEADERS = \
ipc/ipc.h                     \
ipc/conditional/conditional.h \
ipc/clock/clock.h             \
ipc/factory/ipcfactory.h      \
ipc/helper/ipchelper.h        \
ipc/mail/mail.h               \
ipc/mailbox/mailbox.h         \
ipc/mailbox/mailboxcbk.h      \
ipc/mutex/mutex.h             \
ipc/publisher/publisher.h     \
ipc/semaphore/semaphore.h     \
ipc/thread/thread.h           \
ipc/thread/threadcbk.h        \
ipc/timer/timer.h             \
ipc/worker/worker.h

pkgconfig_DATA = ipc.pc

# Unit Tests
check_PROGRAMS = unittest
unittest_SOURCES = \
tst/mailist.h                                \
tst/ipcgtestworker/ipcgtestworker.c          \
tst/ipcgtestworker/ipcgtestworker.h          \
tst/ipcgtestworker/ipcgtestworker-internal.h \
tst/ipcgtestworker/ipcgtestworker_mailist.h  \
tst/ipcgtestworker/ipcgtestworker-test.cc    

unittest_CPPFLAGS = $(GMOCK_CFLAGS) $(IPC_CFLAGS)
unittest_LDADD = $(lib_LTLIBRARIES) $(GMOCK_LIBS) $(IPC_LIBS)
unittest_LDFLAGS = -no-install

EXTRA_DIST = m4/NOTES
dist_doc = README.md LICENSE