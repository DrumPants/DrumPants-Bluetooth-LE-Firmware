#
# Copyright 2013, Broadcom Corporation
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

APP_SRC = hello_sensor.c

########################################################################
# Add subdirectories to the includes
########################################################################

INCS += $(DIR)

# Include application cgs if the app wants to override some config here
# by uncommenting the following line and adding application.cgs to this directory.
# CGS_LIST += $(DIR)/application.cgs

# Include the chip common make file
include spar_$(CHIP)$(REVNUM).inc
