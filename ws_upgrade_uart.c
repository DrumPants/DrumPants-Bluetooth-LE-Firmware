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
* This file provides function required to support WICED Smart Upgrade over the
* Peripheral UART.
*
* To download host sends command to download with length of the patch to be
* transmitted.  After device acks that, host sends fixed chunks of data
* each of each has to be acked.  After all the bytes has been downloaded
* and acknowledged host sends verify command that includes CRC32 of the
* whole patch.  During the download device saves data directly to the EEPROM
* or serial flash.  At the verification stage device reads data back from the
* NVRAM and calculates checksum of the data stored there.  Result of the
* verification is sent back to the host.
*
*/
// #define BLE_TRACE_DISABLE 1

#include "bleprofile.h"
#include "bleapp.h"
#include "puart.h"
#include "gpiodriver.h"
#include "string.h"
#include "stdio.h"
#include "platform.h"
#include "bleapputils.h"
#include "bleappfwu.h"
#include "ws_upgrade_uart.h"
#include "ws_upgrade.h"
#include "devicelpm.h"
#include "spar_utils.h"

/******************************************************
 *                      Constants
 ******************************************************/

// device states during UART FW upgrade
#define WS_UPGRADE_STATE_WAIT_START        0
#define WS_UPGRADE_STATE_WAIT_START_LEN1   1
#define WS_UPGRADE_STATE_WAIT_START_LEN2   2
#define WS_UPGRADE_STATE_READ_DATA         3
#define WS_UPGRADE_STATE_WAIT_FINISH       4
#define WS_UPGRADE_STATE_WAIT_FINISH_LEN1  5
#define WS_UPGRADE_STATE_WAIT_FINISH_LEN2  6
#define WS_UPGRADE_STATE_WAIT_FINISH_LEN3  7
#define WS_UPGRADE_STATE_WAIT_FINISH_LEN4  8

// downloader will use 16 byte chunks
#define WS_UPGRADE_READ_CHUNK                   15

// write to eeprom in 64 byte chunks
#define WS_UPGRADE_CHUNK_SIZE_TO_COMMIT         60

/******************************************************
 *               Function Prototypes
 ******************************************************/
UINT32 ws_upgrade_uart_device_lpm_queriable(LowPowerModePollType type, UINT32 context);

/******************************************************
 *               Variables Definitions
 ******************************************************/

int     ws_upgrade_state                    = WS_UPGRADE_STATE_WAIT_START;
int     ws_upgrade_total_len                = 0;
int     ws_upgrade_current_block_offset;
int     ws_upgrade_total_offset;
UINT32  ws_upgrade_crc32;
char    ws_upgrade_read_buffer[WS_UPGRADE_CHUNK_SIZE_TO_COMMIT];

// Initialize peripheral UART
int ws_upgrade_uart_init(BLE_PROFILE_PUART_CFG *puart_cfg, void (*puart_rx_callback)(void*))
{
    ws_upgrade_init();

    // Find out what type the NV is.
    if (ws_upgrade_active_nv_type == WS_UPGRADE_NV_TYPE_SF)
    {
        // If serial flash flash is used P32 is MISO and P33 is CS to access serial flash
        // typically UART RX should be 25 and UART TX is 24.
        // make sure that application is not trying to use standard settings.
        if ((puart_cfg->rxpin == 33) || (puart_cfg->txpin == 33) ||
            (puart_cfg->rxpin == 32) || (puart_cfg->txpin == 32))
        {
            return FALSE;
        }
    }

	// Select the uart pins for RXD, TXD and optionally CTS and RTS.
	// If hardware flow control is not required like here, set these
	// pins to 0x00. See Table 1 and Table 2 for valid options.
	puart_selectUartPads(puart_cfg->rxpin, puart_cfg->txpin, 0x00, 0x00);

	// Initialize the peripheral uart driver
	puart_init();

	// Since we are not configuring CTS and RTS here, turn off
	// hardware flow control. If HW flow control is used, then
	// puart_flowOff should not be invoked.
	puart_flowOff();

	// Since we are not using any flow control, disable sleep when download starts.
	// If HW flow control is configured or app uses its own flow control mechanism,
	// this is not required.
	devlpm_registerForLowPowerQueries(ws_upgrade_uart_device_lpm_queriable, 0);

	// BEGIN - puart interrupt
	//  The following lines enable interrupt when one (or more) bytes
	//  are received over the peripheral uart interface. This is optional.
	//  In the absense of this, the app is expected to poll the peripheral
	//  uart to pull out received bytes.

	// clear interrupt
	P_UART_INT_CLEAR(P_UART_ISR_RX_AFF_MASK);

	// set watermark to 1 byte - will interrupt on every byte received.
	P_UART_WATER_MARK_RX_LEVEL (1);

	// enable UART interrupt in the Main Interrupt Controller and RX Almost Full in the UART
	// Interrupt Controller
	P_UART_INT_ENABLE |= P_UART_ISR_RX_AFF_MASK;

	// Set callback function to app callback function.
	puart_rxCb = puart_rx_callback;

	// Enable the CPU level interrupt
	puart_enableInterrupt();

	/* END - puart interrupt */
	return (TRUE);
}

// Send a status byte to the host.
void ws_upgrade_uart_send_status(int status)
{
	// ble_trace1("Ack:%d", status);
    puart_write(status);
}

#define POLYNOMIAL              0x04C11DB7
#define WIDTH                   (8 * sizeof(unsigned long))
#define TOPBIT                  (1 << (WIDTH - 1))
#define INITIAL_REMAINDER       0xFFFFFFFF
#define FINAL_XOR_VALUE         0xFFFFFFFF
#define REFLECT_DATA(X)         ((unsigned char) reflect((X), 8))
#define REFLECT_REMAINDER(X)    ((unsigned long) reflect((X), WIDTH))
#define CHECK_VALUE             0xCBF43926

/*********************************************************************
 *
 * Function:    reflect()
 *
 * Description: Reorder the bits of a binary sequence, by reflecting
 *              them about the middle position.
 *
 * Notes:       No checking is done that nBits <= 32.
 *
 * Returns:     The reflection of the original data.
 *
 *********************************************************************/
static unsigned long reflect(unsigned long data, unsigned char nBits)
{
    unsigned long  reflection = 0x00000000;
    unsigned char  bit;

    // Reflect the data about the center bit.
    for (bit = 0; bit < nBits; ++bit)
    {
        // If the LSB bit is set, set the reflection of it.
        if (data & 0x01)
        {
            reflection |= (1 << ((nBits - 1) - bit));
        }
        data = (data >> 1);
    }
    return (reflection);

}   /* reflect() */



unsigned long crcSlow(unsigned long  crc32, unsigned char const message[], int nBytes)
{
    int            byte;
    unsigned char  bit;

    // Perform modulo-2 division, a byte at a time.
    for (byte = 0; byte < nBytes; ++byte)
    {
        // Bring the next byte into the crc32.
        crc32 ^= (REFLECT_DATA(message[byte]) << (WIDTH - 8));

        // Perform modulo-2 division, a bit at a time.
        for (bit = 8; bit > 0; --bit)
        {
            // Try to divide the current data bit.
            if (crc32 & TOPBIT)
            {
                crc32 = (crc32 << 1) ^ POLYNOMIAL;
            }
            else
            {
                crc32 = (crc32 << 1);
            }
        }
    }
    return crc32;

}   /* crcSlow() */

unsigned long crcComplete(unsigned long crc32)
{
    // The final crc32 is the CRC result.
    return (REFLECT_REMAINDER(crc32) ^ FINAL_XOR_VALUE);
}

// verify function is called after all the data has been received and stored
// in the NV.  The function reads back data from the NV and calculates the checksum.
// Function returns TRUE if calculated CRC matches the one calculated by the host
int ws_upgrade_verify(void)
{
    unsigned int i;
    unsigned long  crc32 = INITIAL_REMAINDER;

    for (i = 0; i < ws_upgrade_total_len; i += WS_UPGRADE_READ_CHUNK)
    {
        char memory_chunk[WS_UPGRADE_READ_CHUNK];
        int bytesToRead = i + WS_UPGRADE_READ_CHUNK < ws_upgrade_total_len ? WS_UPGRADE_READ_CHUNK : ws_upgrade_total_len - i;

        ws_upgrade_retrieve_from_nv (i, memory_chunk, bytesToRead);
        crc32 = crcSlow(crc32, memory_chunk, bytesToRead);
    }
    crc32 = crcComplete(crc32);

    ble_trace2 ("\nws_verify rcvd:%x calculated:%x", ws_upgrade_crc32, crc32);

    return (crc32 == ws_upgrade_crc32);
}

// Application thread context uart interrupt handler.
// unused - Unused parameter.
void ws_upgrade_uart_interrupt_callback(void* unused)
{
    // There can be at most 16 bytes in the HW FIFO.
    char  readbyte;
    int   send_status = FALSE;

    // empty the FIFO
    while(puart_rxFifoNotEmpty() && puart_read(&readbyte))
    {
    	// ble_trace1("\rurs:%d", ws_upgrade_state);
        switch (ws_upgrade_state)
        {
        case WS_UPGRADE_STATE_WAIT_START:
            if (readbyte == WS_UPGRADE_COMMAND_START)
            {
                ws_upgrade_state = WS_UPGRADE_STATE_WAIT_START_LEN1;
            }
            else
            {
                ble_trace2("\ruart read state:%d byte:%02x", ws_upgrade_state, readbyte);
            }
            break;

        case WS_UPGRADE_STATE_WAIT_START_LEN1:
            ws_upgrade_total_len            = readbyte;
            ws_upgrade_state                = WS_UPGRADE_STATE_WAIT_START_LEN2;
            break;

        case WS_UPGRADE_STATE_WAIT_START_LEN2:
			if (!ws_upgrade_init_nv_locations())
			{
				// If we are not able to init the NV locations.
				ws_upgrade_uart_send_status(WS_UPGRADE_EVENT_FAIL);
                ws_upgrade_state = WS_UPGRADE_STATE_WAIT_START;
				break;
			}
			
            ws_upgrade_total_len           += (readbyte << 8);
            ws_upgrade_current_block_offset = 0;
            ws_upgrade_total_offset         = 0;
            ws_upgrade_state                = WS_UPGRADE_STATE_READ_DATA;

            ble_trace2("\ruart read state:%d total_len:%d", ws_upgrade_state, ws_upgrade_total_len);

            send_status = TRUE;
            break;

        case WS_UPGRADE_STATE_READ_DATA:
            ws_upgrade_read_buffer[ws_upgrade_current_block_offset++] = readbyte;
            if ((ws_upgrade_current_block_offset == WS_UPGRADE_CHUNK_SIZE_TO_COMMIT) ||
                (ws_upgrade_total_offset + ws_upgrade_current_block_offset == ws_upgrade_total_len))
            {
                ws_upgrade_store_to_nv(ws_upgrade_total_offset, ws_upgrade_read_buffer, ws_upgrade_current_block_offset);
                if (ws_upgrade_total_offset == 0)
                {
                    ble_tracen (ws_upgrade_read_buffer, 30);
                }
                ws_upgrade_total_offset        += ws_upgrade_current_block_offset;
                ws_upgrade_current_block_offset = 0;

                if (ws_upgrade_total_offset == ws_upgrade_total_len)
                {
                    ws_upgrade_state = WS_UPGRADE_STATE_WAIT_FINISH;
                    send_status = TRUE;
                    break;
                }
            }
            if (ws_upgrade_current_block_offset % WS_UPGRADE_READ_CHUNK == 0)
            {
                send_status = TRUE;
            }
            break;

        case WS_UPGRADE_STATE_WAIT_FINISH:
            if (readbyte == WS_UPGRADE_COMMAND_FINISH)
            {
                ws_upgrade_state      = WS_UPGRADE_STATE_WAIT_FINISH_LEN1;
                ws_upgrade_crc32      = 0;
            }
            else
            {
                ble_trace2("\ruart read state:%d byte:%02x", ws_upgrade_state, readbyte);
            }
            break;

        case WS_UPGRADE_STATE_WAIT_FINISH_LEN1:
            ws_upgrade_crc32    = readbyte;
            ws_upgrade_state    = WS_UPGRADE_STATE_WAIT_FINISH_LEN2;
            break;

        case WS_UPGRADE_STATE_WAIT_FINISH_LEN2:
            ws_upgrade_crc32   += (readbyte << 8);
            ws_upgrade_state    = WS_UPGRADE_STATE_WAIT_FINISH_LEN3;
            break;

        case WS_UPGRADE_STATE_WAIT_FINISH_LEN3:
            ws_upgrade_crc32   += (readbyte << 16);
            ws_upgrade_state    = WS_UPGRADE_STATE_WAIT_FINISH_LEN4;
            break;

        case WS_UPGRADE_STATE_WAIT_FINISH_LEN4:
            ws_upgrade_crc32   += (readbyte << 24);
            ws_upgrade_state    = WS_UPGRADE_STATE_WAIT_FINISH_LEN4;

            if (ws_upgrade_verify())
            {
                ws_upgrade_uart_send_status(WS_UPGRADE_EVENT_OK);
                ws_upgrade_finish();
            }
            else
            {
                ws_upgrade_uart_send_status(WS_UPGRADE_EVENT_FAIL);
                ws_upgrade_state = WS_UPGRADE_STATE_WAIT_START;
            }
            break;
        }
    }
    // ble_trace3("\ruart read state:%d offset:%d total:%d", ws_upgrade_state, ws_upgrade_current_block_offset, ws_upgrade_total_offset);

    // clear the interrupt
    P_UART_INT_CLEAR(P_UART_ISR_RX_AFF_MASK);

    // enable UART interrupt in the Main Interrupt Controller and RX Almost Full in the UART Interrupt Controller
    P_UART_INT_ENABLE |= P_UART_ISR_RX_AFF_MASK;

    if (send_status)
    {
        ws_upgrade_uart_send_status(WS_UPGRADE_EVENT_OK);
    }
}

// Callback called by the FW when ready to sleep/deep-sleep. Disable both by returning 0
// when there is an active download ongoing.
UINT32 ws_upgrade_uart_device_lpm_queriable(LowPowerModePollType type, UINT32 context)
{
	// Disable sleep.
	return 0;
}
