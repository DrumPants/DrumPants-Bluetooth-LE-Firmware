#
# Copyright 2014, Broadcom Corporation
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

########################################################################
# Add Application sources here.
########################################################################
APP_SRC = hello_sensor.c uart_one_wire.c ws_upgrade_uart.c ws_upgrade.c


# This application requires a special patch to notify before a notification is noticed
# see http://community.broadcom.com/message/6375#6375
APP_PATCHES_AND_LIBS += application_poll_notification.a

########################################################################
################ DO NOT MODIFY FILE BELOW THIS LINE ####################
########################################################################
