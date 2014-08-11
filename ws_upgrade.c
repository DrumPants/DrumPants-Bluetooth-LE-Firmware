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
* WICED Smart Upgrade
*
* This file provides common functions required to support WICED Smart Upgrade
* whether it is being done over the air, UART, or SPI.  Primarily the
* functionality is provided for storing and retrieving information from EEPROM
* or Serial Flash whatever is available on the platform.
*
*/

#include "bleprofile.h"
#include "bleapp.h"
#include "puart.h"
#include "gpiodriver.h"
#include "string.h"
#include "stdio.h"
#include "platform.h"
#include "bleapputils.h"
#include "bleappfwu.h"
#include "limits.h"

/******************************************************
 *                      Constants
 ******************************************************/
#define WS_UPGRADE_NV_IS_EEPROM()  ((Config_and_Firmware_Status & CFA_CONFIG_LOCATION_MASK) == CFA_CONFIG_LOCATION_EEPROM)
#define WS_UPGRADE_SS_LENGTH                    40  // SS is always 40 bytes long.
#define WS_UPGRADE_SERIAL_FLASH_SECTOR_SIZE     (4 * 1024)

/******************************************************
 *                     Structures
 ******************************************************/
// Table of locations and lengths for EEPROM and SF.
typedef struct
{
    UINT32 ss_loc[2];
    UINT32 ds1_loc;
    UINT32 ds1_len;
    UINT32 ds2_loc;
    UINT32 ds2_len;
    UINT32 vs1_loc;
    UINT32 vs1_len;
    UINT32 vs2_loc;
    UINT32 vs2_len;
} WS_UPGRADE_NV_LOC_LEN;

enum
{
    WS_UPGRADE_NV_TYPE_EEPROM = 0,
    WS_UPGRADE_NV_TYPE_SF     = 1
};

// The static sections are assumed to be at these locations.
UINT8   ss_data[WS_UPGRADE_SS_LENGTH];

// Recommended offsets for EEPROM
// --------------------------------------------------------------------------------------------------------------------
// |  SS1 (256b @ 0)  |  SS2 (256b @ 0x100)  |  VS1 (1024b @ 0x140)  | DS1 (30.5Kb @ 0x580)  | DS2 (30.5Kb @ 0x8000)  |
// --------------------------------------------------------------------------------------------------------------------

// Recommended offsets for Serial Flash. All sections have to be on a 4K sector boundary.
// Ensure that the .btp file has the same values as the values below:
// ConfigDSLocation = <ds1_location>
// DLConfigVSLocation = <vs_location1>
// DLConfigVSLength = <vs_length1>
// The following is the layout of the SF.
// -------------------------------------------------------------------------------------------------------------------------------------------
// |  SS1 (4K @ 0)  |  SS2 (4K @ 0x1000)  |  VS1 (4K @ 0x2000)  | VS2 (4K @ 0x3000)  | DS1 (24K @ 0x4000)  | DS2 (24K @ 0xA000)  |  UNUSED
// -------------------------------------------------------------------------------------------------------------------------------------------

// For reference only.
// UINT32 ss_locations[2]       = {0, 0x1000};
// UINT32 vs_location1          = 0x2000;     // VS section occupies 1 sector
// UINT32 vs_length1            = 0x1000;     // 4K = 1 SF sector
// UINT32 vs_location2          = 0x3000;     // Double buffer for VS for failsafe storage.
// UINT32 vs_length2            = 0x1000;     // 4K = 1 SF sector = vs_length1

// NV locations and lengths. Ensure that the BTP file has the same values for the parameters in the
// inline comment on the right.
const WS_UPGRADE_NV_LOC_LEN nv_loc_len[2] =
{
    {
        /* .ss_loc = */     {0, 256},
        /* .ds1_loc = */    0x580,         // ConfigDSLocation
        /* .ds1_len = */    0x7A00,        // ~31KB
        /* .ds2_loc = */    0x8000,
        /* .ds2_len = */    0x7A00,        // ~31KB
        /* .vs1_loc = */    0x140,         // DLConfigVSLocation
        /* .vs1_len = */    0x400,         // DLConfigVSLength
        /* .vs2_loc = */    0,             // No VS2 for EEPROMS
        /* .vs2_len = */    0              // No VS2 for EEPROMS
    },
    // We are assuming that we have a 64KByte Serial Flash. If the SF is larger, you may change ds1_len, ds2_loc, ds2_len
    // accordingly.
    {
        /* .ss_loc = */      {0, 0x1000},
        /* .ds1_loc = */     0x4000,          // ConfigDSLocation
        /* .ds1_len = */     0x6000,
        /* .ds2_loc = */     0xA000,
        /* .ds2_len = */     0x6000,
        /* .vs1_loc = */     0x2000,          // DLConfigVSLocation
        /* .vs1_len = */     0x1000,          // DLConfigVSLength
        /* .vs2_loc = */     0x3000,
        /* .vs2_len = */     0x1000
    },
};

// this is external variable that can be used by other units
UINT32 ws_upgrade_active_nv_type        = WS_UPGRADE_NV_TYPE_EEPROM;   // Assume EEPROM

static UINT32 active_ds_location        = 0;
static UINT32 upgrade_ds_location       = 0;
static UINT32 upgrade_ds_length         = 0;
static UINT32 active_ss_location        = 0;
static UINT32 upgrade_ss_location       = 0;
const UINT8 ss_valid_byte_sequence[]    = {0x01, 0x08, 0x00, 0xf0, 0x00, 0x00, 0x62, 0x08, 0xc0, 0x5d, 0x89};


extern UINT32 Config_and_Firmware_Status;
extern UINT32 Config_DS_Location;
extern UINT32 Config_SS_Location;

/******************************************************
 *               Function Definitions
 ******************************************************/
static UINT32 ws_upgrade_convert_to_nv_virtual_address(UINT32 address);
static UINT32 ws_upgrade_create_upgrade_ss(void);
static void   ws_upgrade_invalidate_active_ss(void);

void ws_upgrade_init(void)
{
    // Find out what type the NV is.
    ble_trace2 ("\rws_upgrade_init: DS:%04x SS:%04x", Config_DS_Location, Config_SS_Location);

    if (!WS_UPGRADE_NV_IS_EEPROM())
    {
        ws_upgrade_active_nv_type = WS_UPGRADE_NV_TYPE_SF;
    }
    else
    {
        ws_upgrade_active_nv_type = WS_UPGRADE_NV_TYPE_EEPROM;
    }

    if ((Config_DS_Location != nv_loc_len[ws_upgrade_active_nv_type].ds1_loc) &&
            (Config_DS_Location != nv_loc_len[ws_upgrade_active_nv_type].ds2_loc))
    {
        // If active DS is neither DS1 nor DS2 we expect to see, fail the download.
        ble_trace0("WARNING: Upgrade will fail - active DS is not one of the expected locations.\n");
        ble_trace0("WARNING: Are ConfigDSLocation and DLConfigVSLocation in the .btp set up as in nv_loc_len[]?\n");
    }
}

// Converts a given address into the NV virtual address.
UINT32 ws_upgrade_convert_to_nv_virtual_address(UINT32 address)
{
    if (WS_UPGRADE_NV_IS_EEPROM())
    {
        return (address |= INDIRECT_MEM_MAP_EEPROM);
    }
    else
    {
        return (address |= INDIRECT_MEM_MAP_SF);
    }
}

// Checks if the second static section exists or not and creates it if required.
UINT32 ws_upgrade_create_upgrade_ss(void)
{
    // SS is always 40 bytes long.
    UINT8 upgrade_ss_data[WS_UPGRADE_SS_LENGTH];
    UINT8 upgrade_ss_current[WS_UPGRADE_SS_LENGTH];
    UINT32 active_ss_virtual_address, upgrade_ss_virtual_address, i;

    if (!WS_UPGRADE_NV_IS_EEPROM())
    {
        for (i = 0; i < 2; i++)
        {
            active_ss_virtual_address = ws_upgrade_convert_to_nv_virtual_address(nv_loc_len[ws_upgrade_active_nv_type].ss_loc[i]);
            // ble_trace2("Reading from 0x%08X, v: 0x%08X", ss_locations[i], virt_addr);

            bleappfwu_readMem(ss_data, active_ss_virtual_address, sizeof(ss_data));

            // Compare the first few bytes to what should be the right SS.
            if (!memcmp(ss_data, ss_valid_byte_sequence, sizeof(ss_valid_byte_sequence)))
            {
                active_ss_location = nv_loc_len[ws_upgrade_active_nv_type].ss_loc[i];
                Config_SS_Location = active_ss_location;
                break;
            }
        }
    }
    else
    {
        active_ss_location = Config_SS_Location;
        active_ss_virtual_address = ws_upgrade_convert_to_nv_virtual_address(active_ss_location);
        bleappfwu_readMem(ss_data, active_ss_virtual_address, sizeof(ss_data));
    }

    //ble_trace1("Read these bytes from current SS:0x%08X", active_ss_location);
    //ble_tracen(ss_data, sizeof(ss_data));

    // Make a local copy so we can modify the target DS in the SS.
    memcpy(upgrade_ss_data, ss_data, sizeof(upgrade_ss_data));

    // upgrade ss location is the 'other' ss location in the list.
    upgrade_ss_location = (active_ss_location == nv_loc_len[ws_upgrade_active_nv_type].ss_loc[0]) ?
        nv_loc_len[ws_upgrade_active_nv_type].ss_loc[1] : nv_loc_len[ws_upgrade_active_nv_type].ss_loc[0];

    // Convert the upgrade location into the equivalent virtual address.
    upgrade_ss_virtual_address = ws_upgrade_convert_to_nv_virtual_address(upgrade_ss_location);

    // Then update the DS with the upgrade DS.
    upgrade_ss_data[0x1E] = (upgrade_ds_location & 0xFF);
    upgrade_ss_data[0x1F] = (upgrade_ds_location & 0xFF00) >> 8;
    upgrade_ss_data[0x20] = (upgrade_ds_location & 0xFF0000) >> 16;
    upgrade_ss_data[0x21] = (upgrade_ds_location & 0xFF000000) >> 24;

    if (!WS_UPGRADE_NV_IS_EEPROM())
    {
        // Sector must be erased before writing to it.
        bleappfwu_eraseMem(upgrade_ss_virtual_address, WS_UPGRADE_SERIAL_FLASH_SECTOR_SIZE);
    }

    //ble_trace2("Writing new SS: 0x%08X = 0x%08X", upgrade_ss_location, upgrade_ss_virtual_address);
    //ble_tracen(upgrade_ss_data, sizeof(upgrade_ss_data));

    // Write this to the upgrade SS.
    if (bleappfwu_writeMem(upgrade_ss_data, upgrade_ss_virtual_address, sizeof(upgrade_ss_data)))
    {
    	ble_trace0("Could not write new SS!");
        return 0;
    }

    memset(upgrade_ss_current, 0x00, sizeof(upgrade_ss_current));

    // Read back the SS we just wrote to make sure that it was indeed committed to the NV
    if (bleappfwu_readMem(upgrade_ss_current, upgrade_ss_virtual_address, sizeof(upgrade_ss_current)))
        return 0;

    //ble_trace0("Read Back new SS:");
    //ble_tracen(upgrade_ss_current, sizeof(upgrade_ss_current));

    // Return true iff what we wrote is whats now in the NV. Else we may brick the device
    return !memcmp(upgrade_ss_current, upgrade_ss_data, sizeof(upgrade_ss_current));
}

// WARNING: Invalidates the current SS. Should be called only when upgrade succeeded and the upgrade
// SS was also created successfully before by ws_upgrade_create_upgrade_ss.
// Failure to do this correctly will result in bricking the device.
void ws_upgrade_invalidate_active_ss(void)
{
    UINT32 i, j, found = 0, virt;
    // Flip one bit in the active SS so that the checksum of the SS fails on the
    // subsequent boot.
    // ble_tracen(ss_data, sizeof(ss_data));

    ss_data[0] ^= 1;

    // ble_trace0("Invalidated Old SS.");
    // ble_tracen(ss_data, sizeof(ss_data));

    virt = ws_upgrade_convert_to_nv_virtual_address(active_ss_location);

    if (!WS_UPGRADE_NV_IS_EEPROM())
    {
        // Erase sector if SF before writing.
        bleappfwu_eraseMem(virt, WS_UPGRADE_SERIAL_FLASH_SECTOR_SIZE);
    }

    // Write this to the active SS.
    bleappfwu_writeMem(ss_data, virt, sizeof(ss_data));

    // Active SS has now been clobbered. The upgrade SS will take over now.
}

// setup nvram locations to be used during upgrade
UINT32 ws_upgrade_init_nv_locations(void)
{
    if ((Config_DS_Location != nv_loc_len[ws_upgrade_active_nv_type].ds1_loc) &&
            (Config_DS_Location != nv_loc_len[ws_upgrade_active_nv_type].ds2_loc))
    {
        // If active DS is neither DS1 nor DS2 we expect to see, fail the download.
        ble_trace0("Active DS is not DS1 and not DS2. Cannot upgrade.\n");
        return 0;
    }
    active_ds_location = Config_DS_Location;

    upgrade_ds_location = (active_ds_location == nv_loc_len[ws_upgrade_active_nv_type].ds1_loc) ?
        nv_loc_len[ws_upgrade_active_nv_type].ds2_loc : nv_loc_len[ws_upgrade_active_nv_type].ds1_loc;
    upgrade_ds_length   = (active_ds_location == nv_loc_len[ws_upgrade_active_nv_type].ds1_loc) ?
        nv_loc_len[ws_upgrade_active_nv_type].ds2_len   : nv_loc_len[ws_upgrade_active_nv_type].ds1_len;

    ble_trace3("Active: 0x%08X, Upgrade: 0x%08X, UG length: 0x%08X", active_ds_location, upgrade_ds_location, upgrade_ds_length);
    // ble_tracen(ss_data, sizeof(ss_data));

    return 1;
}

// Stores to the physical NV storage medium.
UINT32 ws_upgrade_store_to_nv(UINT32 offset, UINT8 *data, UINT32 len)
{
    UINT32 _offset = offset;

    // The real offset into the NV is the current offset + the upgrade DS location.
    offset += upgrade_ds_location;

    // Now add the NV virtual address to this offset based on the physical part.
    offset = ws_upgrade_convert_to_nv_virtual_address(offset);

    // Try to erase DS if running from serial flash and this is the first write
    if (!WS_UPGRADE_NV_IS_EEPROM() && !_offset)
    {
        // Use the virtual address
        UINT32 addr = offset;
        // We will erase sector-by-sector.
        // Note that we are rounding up to sector boundary here because
        // the smallest unit we can erase is a sector (block erase and
        // chip erase are not desirable for obvious reasons).
        // Only the lower 24 bits of addr will go to the SF. See SF datasheet.
        while(addr < (offset + upgrade_ds_length))
        {
            // erase in chunks of 4KB
            bleappfwu_eraseMem(addr, WS_UPGRADE_SERIAL_FLASH_SECTOR_SIZE);

            addr += WS_UPGRADE_SERIAL_FLASH_SECTOR_SIZE;
        }
    }

    // Write to NV.
    return bleappfwu_writeMem(data, offset, len);
}

// Retrieve chunk of data from the physical NV storage medium
UINT32 ws_upgrade_retrieve_from_nv(UINT32 offset, UINT8 *data, UINT32 len)
{
    // The real offset into the NV is the current offset + the upgrade DS location.
    offset += upgrade_ds_location;

    // Now add the NV virtual address to this offset based on the physical part.
    offset = ws_upgrade_convert_to_nv_virtual_address(offset);

    // Write to NV.
    bleappfwu_readMem(data, offset, len);
    return 0;
}

// After download is completed and verified this function is
// called to switch active partitions with the one that has been
// receiving the new image.
void ws_upgrade_finish(void)
{
    // If successful, create the upgrade SS too.
    if (ws_upgrade_create_upgrade_ss())
    {
        // Then invalidate the active SS only if the upgrade SS was
        // created successfully.
        ws_upgrade_invalidate_active_ss();
    }
    else
    {
    	ble_trace0("Could not create upgrade SS\n");
    }
    // Now force a reset.
    // First disable all interrupts.
    bleapputil_cpuIntDisable();

    // trip watch dog now.
    bleappfwu_watchdogExpired(0);

    // End of the world - will not return.
}
