define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=ipc_decorator.h ipc_linux_task.h ipc_linux_timestamp.h
 
$(_flavor_)_$(_feat_)_lib_objs=ipc_decorator ipc_linux_task ipc_linux_timestamp

$(_flavor_)_$(_feat_)_lib=ipc_decorator

endef

include $(PROJ_MAK_DIR)/epilog.mk
