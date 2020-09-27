 ACLOCAL_AMFLAGS = -I m4 --install
# Libraries
lib_LTLIBRARIES = libipc-1.0.la
libipc_1_0_la_SOURCES = \
ipc/ipc.c                              \
ipc/ipc.h                              \
ipc/conditional/conditional.c          \
ipc/conditional/conditional.h          \
ipc/conditional/conditional-internal.h \
ipc/mail/mail.c                        \
ipc/mail/mail.h                        \
ipc/mail/mail-internal.h               \
ipc/mailbox/mailbox.c                  \
ipc/mailbox/mailbox.h                  \
ipc/mailbox/mailbox-internal.h         \
ipc/mailbox/mailboxcbk.c               \
ipc/mailbox/mailboxcbk.h               \
ipc/mailbox/mailboxcbk-internal.h      \
ipc/mutex/mutex.c                      \
ipc/mutex/mutex.h                      \
ipc/mutex/mutex-internal.h             \
ipc/semaphore/semaphore.c              \
ipc/semaphore/semaphore.h              \
ipc/semaphore/semaphore-internal.h     \
ipc/timer/timer.c                      \
ipc/timer/timer.h                      \
ipc/timer/timer-internal.h

libipc_1_0_la_LDFLAGS = -version-info 0:0:0
nobase_include_HEADERS = \
ipc/ipc.h                     \
ipc/conditional/conditional.h \
ipc/mutex/mutex.h             \
ipc/mail/mail.h               \
ipc/semaphore/semaphore.h     \
ipc/timer/timer.h

# Unit Tests
check_PROGRAMS = unittest
unittest_SOURCES = 

unittest_CPPFLAGS = $(libgmock_main_CFLAGS)
unittest_LDADD = $(lib_LTLIBRARIES) $(libgmock_main_LIBS)
unittest_LDFLAGS = -no-install

EXTRA_DIST = m4/NOTES
dist_doc = README.md LICENSE
