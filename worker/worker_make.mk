#=======================================================================================#
# worker_make.mk
#=======================================================================================#
#  Created on: Oct 3, 2015
#      Author: puch
#=======================================================================================#

#=======================================================================================#
# DEFINE PACKAGE RULE
#=======================================================================================#
define $(_flavor_)_$(_feat_)_MAKE
#=======================================================================================#
# OBJECTS DIRECTORY
# e.g: 
#     $(_flavor_)_$(_feat_)_src_dir=pk_module_N_code/_src
#     or
#     $(_flavor_)_$(_feat_)_src_dir=_src
#=======================================================================================#
$(_flavor_)_$(_feat_)_inc=worker.h worker_evs.h

#=======================================================================================#
# LIB REQUISITES
#=======================================================================================#

##
 # Object Requisites
 # e.g: $(_flavor_)_$(_feat_)_lib_objs=$($(_flavor_)_OBJ_DIR)/my_lib_obj$(_obj_ext_) \
 ##
$(_flavor_)_$(_feat_)_lib_objs=worker

$(_flavor_)_$(_feat_)_lib=worker
#=======================================================================================#
# END PACKAGE RULE
#=======================================================================================#
endef
#=======================================================================================#
# LOCAL VARIABLES
#=======================================================================================#

#=======================================================================================#
# LOCAL DEFINES 
#=======================================================================================#

#=======================================================================================#
# LOCAL DEFINE EXPANSIONS
#=======================================================================================#

#=======================================================================================#
# LOCAL RULES EXPANSIONS
#=======================================================================================#

#=======================================================================================#
# INCLUDE PK PROJECT UTILITY
#=======================================================================================#
include $(PROJ_MAK_DIR)/epilog.mk
#=======================================================================================#
# worker_make.mk
#=======================================================================================#
# Changes Log
#
#
#=======================================================================================#
