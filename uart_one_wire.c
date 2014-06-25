// fix for puart.h - found in ws_upgrade_uart.h - have no idea if that's right.
#ifndef REG32
	#define REG32(x)  *((volatile UINT32*)(x))
#endif

#include "bleapp.h"
//#include "blecm.h"


#include "puart.h"
#include "platform.h"
#include "devicelpm.h"

// should i really include this file? seems like something that should be done via build script depending on the platform?
#ifndef P_UART_INT_CLEAR
	#include "20732mapa0.h"
#endif

#include "uart_one_wire.h"

#if SDK_VERSION < 2
	// fix for puart.h - found in ws_upgrade_uart.h - have no idea if that's right.
	extern BLECM_FUNC_WITH_PARAM puart_bleRxCb;

	// make SDK 1.2 compatible with SDK 2
	#define puart_rxCb puart_bleRxCb
#endif

// holds the callback function when a packet is read.
FUNC_ON_UART_RECEIVE onReceiveCB;


// Callback called by the FW when ready to sleep/deep-sleep. Disable both by returning 0
// so the UART will always receive bytes.
UINT32 ws_upgrade_uart_device_lpm_queriable(LowPowerModePollType type, UINT32 context)
{
	// Disable sleep.
	return 0;
}



// Application thread context uart interrupt handler.
// unused - Unused parameter.
// changing return types based on a define? that's not messed up at all!
// really though, this is the fastest way to handle differences between the callback function signature between SDK 1.2 and 2.0
#if SDK_VERSION < 2
INT32
#else
void
#endif
  application_puart_interrupt_callback(void* unused) {
	// There can be at most 16 bytes in the HW FIFO.
	char readbytes[16];

	UINT8 number_of_bytes_read = 0;

	// empty the FIFO
	while(puart_rxFifoNotEmpty() && puart_read(&readbytes[number_of_bytes_read])) {
		number_of_bytes_read++;
	}

	// readbytes should have number_of_bytes_read bytes of data read from puart. Do something with this.

	// TODO: should this be after clearing the interrupt????
	if (number_of_bytes_read > 0) {
		onReceiveCB(readbytes, number_of_bytes_read);
	}

	// clear the interrupt
	P_UART_INT_CLEAR(P_UART_ISR_RX_AFF_MASK);

	// enable UART interrupt in the Main Interrupt Controller and RX Almost Full in the UART Interrupt Controller
	P_UART_INT_ENABLE |= P_UART_ISR_RX_AFF_MASK;

#if SDK_VERSION < 2
	return 0;
#endif
}


/***
 * Inits the PUART with the given RX callback.
 *
 * Returns TRUE if successful.
 */
PUART_ERROR peripheral_uart_init(FUNC_ON_UART_RECEIVE callback, void (*trace0) (const char *p_str) ) {
	extern puart_UartConfig puart_config;

	// Do all other app initializations.
	// Set the baud rate we want to use. Default is 115200.
	puart_config.baudrate = PUART_BAUD_RATE;

	// these are reversed for some reason? returns true if invalid, false if valid. WTF??
	if (!puart_checkRxdPortPin(PUART_RX_PIN)) {
		ble_trace1("\nUsing PUART RX pin %d", PUART_RX_PIN);
	}
	else {
		ble_trace1("\nFailed to set PUART RX pin %d", PUART_RX_PIN);
		return PUART_ERROR_PIN_INVALID;
	}

	if (!puart_checkTxdPortPin(PUART_TX_PIN)) {
		ble_trace1("\nUsing PUART TX pin %d", PUART_TX_PIN);
	}
	else {
		ble_trace1("\nFailed to set PUART TX pin %d", PUART_TX_PIN);
		return PUART_ERROR_PIN_INVALID;
	}

	// Select the uart pins for RXD, TXD and optionally CTS and RTS.
	// If hardware flow control is not required like here, set these
	// pins to 0x00. See Table 1 and Table 2 for valid options.
	if (puart_selectUartPads(PUART_RX_PIN, PUART_TX_PIN, 0x00, 0x00)) {
		ble_trace0("\nFailed to set PUART RX/TX pins!");

		return PUART_ERROR_PIN_ASSIGNMENT_FAILED;
	}
	else {
		ble_trace0("\nSet PUART RX/TX pins correctly!");
	}

	// Initialize the peripheral uart driver
	puart_init();

#if SDK_VERSION >= 2
	puart_setBaudrate(0x0, 0x0, PUART_BAUD_RATE);
#endif

	// Since we are not configuring CTS and RTS here, turn off // hardware flow control. If HW flow control is used, then // puart_flowOff should not be invoked.
	puart_flowOff();

#if SDK_VERSION < 2
	bleprofile_PUARTRxOn();
	bleprofile_PUARTTxOn();
#else
	puart_enableTx();
#endif


	// disable sleep mode: the PUART does not work if the device is asleep
//#if SDK_VERSION < 2
	devlpm_init();
//#endif

	// Since we are not using any flow control, disable sleep when download starts.
	// If HW flow control is configured or app uses its own flow control mechanism,
	// this is not required.
	if (!devlpm_registerForLowPowerQueries(ws_upgrade_uart_device_lpm_queriable, 0)) {
		ble_trace0("\nFailed to disable low power mode for PUART!");

		return PUART_ERROR_LPM;
	}



	/* BEGIN - puart interrupt
The following lines enable interrupt when one (or more) bytes
are received over the peripheral uart interface. This is optional.
In the absence of this, the app is expected to poll the peripheral uart to pull out received bytes.
	 */
#if ENABLE_PUART_INTERRUPT_CALLBACK
	if (callback != NULL) {
	    //trace0("\nENABLING_PUART_INTERRUPT_CALLBACK");

		// clear interrupt
		P_UART_INT_CLEAR(P_UART_ISR_RX_AFF_MASK);

		// set watermark to 1 byte - will interrupt on every byte received.
		P_UART_WATER_MARK_RX_LEVEL (1);

		// enable UART interrupt in the Main Interrupt Controller and RX Almost Full in the UART // Interrupt Controller
		P_UART_INT_ENABLE |= P_UART_ISR_RX_AFF_MASK;

		// also register a listener from the caller
		onReceiveCB = callback;

		// Set callback function to app callback function.
		puart_rxCb = application_puart_interrupt_callback;

		// Enable the CPU level interrupt
		puart_enableInterrupt();

		/* END - puart interrupt */
	}
#endif

	// print a string message assuming that the device connected
	// to the peripheral uart can handle this string.
	//puart_print("Application initialization complete!");

    //trace0("\nuart_init() DONE\n");

	return PUART_SUCCESS;
}

// Sends out a stream of bytes to the peer device on the peripheral uart interface.
// buffer - The buffer to send to the peer device.
// length - The number of bytes from buffer to send.
// Returns The number of bytes that were sent.
UINT32 application_send_bytes(UINT8* buffer, UINT32 length) {
	UINT32 ok = length;

	// Need to send at least 1 byte.
	if(!buffer || !length)
		return 0;

	// Write out all the given bytes synchronously.
	// If the number of bytes is > P_UART_TX_FIFO_SIZE (16)
	// puart_write() will block until there is space in the // HW FIFO.
	while(length--)
	{
		puart_write(*buffer++);
	}

	return ok;
}

// Attempts to receive data from the peripheral uart.
// buffer - The buffer into which to read bytes.
// length - The number of bytes to read.
// Return The actual number of bytes read.
UINT32 application_receive_bytes(UINT8* buffer, UINT32 length) {

	UINT32 number_of_received_bytes = 0;
	// Need to receive at least 1 byte.
	if(!buffer || !length)
		return 0;

	// Try to receive length bytes
	while(length--)
	{

		if(!puart_read(buffer++)) {
			// If the FIFO is empty, break out, no // more bytes are available.
			break;
		}

		number_of_received_bytes++;
	}

	// Return the actual number of bytes read.
	return number_of_received_bytes;
}



