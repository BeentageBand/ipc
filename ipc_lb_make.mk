define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_lib_libs=conditional ipc ipc_decorator mail mailbox mutex task worker

$(_flavor_)_$(_feat_)_lib=ipc_lb

endef

include $(PROJ_MAK_DIR)/epilog.mk
