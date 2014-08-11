/*
 * Copyright 2014, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/** @file
*
* WICED Smart UART Upgrade 
*
* This file provides definitions and function prototypes for the
* WICED Smart Upgrade over the Peripheral UART
*
*/
#ifndef WS_UART_FU_H
#define WS_UART_FU_H

// Command/events for the UART FW upgrade
#define WS_UPGRADE_COMMAND_START                0x33
#define WS_UPGRADE_COMMAND_FINISH               0x55
#define WS_UPGRADE_EVENT_OK                     0xF0
#define WS_UPGRADE_EVENT_FAIL                   0xF1

#define REG32(x)  *((volatile UINT32*)(x))

int  ws_upgrade_uart_init(BLE_PROFILE_PUART_CFG *puart_cfg, void (*puart_rx_callback)(void*));
void ws_upgrade_uart_interrupt_callback(void* unused);

#endif
