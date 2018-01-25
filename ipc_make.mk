define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc= \
conditional.h \
ipc.h \
ipc_types.h \
ipc_helper.h \
mail.h \
mail_payload.h \
mailbox.h \
mutex.h \
publisher.h \
semaphore.h \
thread.h \
uptime.h \
worker.h \
worker_evs.h \

$(_flavor_)_$(_feat_)_lib_objs= \
conditional \
ipc \
ipc_helper \
mail \
mail_payload \
mailbox \
mutex \
publisher \
semaphore \
thread \
uptime \
worker \

$(_flavor_)_$(_feat_)_lib=ipc

endef

include $(PROJ_MAK_DIR)/epilog.mk
