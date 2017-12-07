define $(_flavor_)_$(_feat_)_MAKE

$(_flavor_)_$(_feat_)_inc=mailbox.h publisher.h
 
$(_flavor_)_$(_feat_)_lib_objs=mailbox publisher

$(_flavor_)_$(_feat_)_lib=mailbox

endef

include $(PROJ_MAK_DIR)/epilog.mk
