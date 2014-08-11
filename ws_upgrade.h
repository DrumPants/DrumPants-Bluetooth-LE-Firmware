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
* WICED Smart Upgrade
*
*/
#ifndef WS_UPGRADE_H
#define WS_UPGRADE_H

/*****************************************************************************/
/** @defgroup ws_upgrade   WICED Smart Upgrade
 *
 *  WICED Smart Upgrade functionality is used to store and retrieve data from
 *  the memory being upgraded.  The 2073x device maybe connected to an
 *  EEPROM or Serial Flash.  The functionality in this module allows application
 *  to access the memory without knowing what is used and the connection method.
 */
/*****************************************************************************/

enum
{
    WS_UPGRADE_NV_TYPE_EEPROM = 0,
    WS_UPGRADE_NV_TYPE_SF     = 1
};

extern UINT32   ws_upgrade_active_nv_type;

/**
* \brief Initialize WICED Smart Upgrade module
* \ingroup ws_upgrade
*
* \details This function is typically called by the application during initialization
* to initialize upgrade module.  Specifically this function finds out which
* medium (serial flash or EEPROM) is installed and saves in the ws_upgrade_active_nv_type
* variable.
*
*/
void     ws_upgrade_init(void);

/**
* \brief Initialize NV locations
* \ingroup ws_upgrade
*
* \details Application calls this function during the start of the firmware download
* to setup memory locations depending on which partition is being used and which
* medium (serial flash or EEPROM) is installed.
*
*/
UINT32     ws_upgrade_init_nv_locations(void);

/**
* \brief Store memory chunk to memory
* \ingroup ws_upgrade
*
* \details Application can call this function to store the next memory chunk in the
* none volatile memory.  Application does not need to know which type of memory is
* used or which partition is being upgraded.
*
* \param offset Offset in the memory where data need to be stored
* \param data   Pointer to the chunk of data to be stored
* \param len    Size of the memory chunk that need to be stored
*
*/
UINT32   ws_upgrade_store_to_nv(UINT32 offset, UINT8 *data, UINT32 len);

/**
* \brief Retrieve memory chunk from memory
* \ingroup ws_upgrade
*
* \details Application typically calls this function when the upgrade process has
* been completed to verify that the data has been successfully stored.  Application
* does not need to know which type of memory is used or which partition is being upgraded.
*
* \param offset Offset in the memory from where data need to be retrieved
* \param data   Pointer to the location to retrieve the data
* \param len    Size of the memory chunk to be retrieved
*
*/
UINT32   ws_upgrade_retrieve_from_nv(UINT32 offset, UINT8 *data, UINT32 len);

/**
* \brief Retrieve memory chunk from memory
* \ingroup ws_upgrade
*
* \details After download is completed and verified this function is called to
* switch active partitions with the one that has been receiving the new image.
*
*/
void     ws_upgrade_finish(void);

#endif
