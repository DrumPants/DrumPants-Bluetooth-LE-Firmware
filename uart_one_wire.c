// fix for puart.h - found in ws_upgrade_uart.h - have no idea if that's right.
#define REG32(x)  *((volatile UINT32*)(x))

#include "blecm.h"
#include "20732mapa0.h"
#include "puart.h"

#include "uart_one_wire.h"

// fix for puart.h - found in ws_upgrade_uart.h - have no idea if that's right.
extern BLECM_FUNC_WITH_PARAM puart_bleRxCb;

// Use P33 for peripheral uart RX.
#define PUART_RX_PIN 33
// Use P32 for peripheral uart TX.
#define PUART_TX_PIN 32

// Application initialization function.
void application_init(void) {
	extern puart_UartConfig puart_config;

	// Do all other app initializations.
	// Set the baud rate we want to use. Default is 115200.
	puart_config.baudrate = 115200;
	// Select the uart pins for RXD, TXD and optionally CTS and RTS.
	// If hardware flow control is not required like here, set these
	// pins to 0x00. See Table 1 and Table 2 for valid options.
	puart_selectUartPads(PUART_RX_PIN, PUART_TX_PIN, 0x00, 0x00);

	// Initialize the peripheral uart driver
	puart_init();

	// Since we are not configuring CTS and RTS here, turn off // hardware flow control. If HW flow control is used, then // puart_flowOff should not be invoked.
	puart_flowOff();

	/* BEGIN - puart interrupt
The following lines enable interrupt when one (or more) bytes
are received over the peripheral uart interface. This is optional.
In the absence of this, the app is expected to poll the peripheral uart to pull out received bytes.
	 */
	// clear interrupt
	P_UART_INT_CLEAR(P_UART_ISR_RX_AFF_MASK);

	// set watermark to 1 byte - will interrupt on every byte received.
	P_UART_WATER_MARK_RX_LEVEL (1);

	// enable UART interrupt in the Main Interrupt Controller and RX Almost Full in the UART // Interrupt Controller
	P_UART_INT_ENABLE |= P_UART_ISR_RX_AFF_MASK;

	// Set callback function to app callback function.
	puart_bleRxCb = application_puart_interrupt_callback;

	// Enable the CPU level interrupt
	puart_enableInterrupt();

	/* END - puart interrupt */


	// print a string message assuming that the device connected
	// to the peripheral uart can handle this string.
	puart_print("Application initialization complete!");
}

// Sends out a stream of bytes to the peer device on the peripheral uart interface. // buffer - The buffer to send to the peer device.
// length - The number of bytes from buffer to send.
// Returns The number of bytes that were sent.
UINT32 application_send_bytes(UINT8* buffer, UINT32 length) {
	UINT32 ok = length;

	// Need to send at least 1 byte.
	if(!buffer || !length)
		return 0;

	// Write out all the given bytes synchronously.
	// If the number of bytes is > P_UART_TX_FIFO_SIZE (16) // puart_write() will block until there is space in the // HW FIFO.
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


// Application thread context uart interrupt handler.
// unused - Unused parameter.
INT32 application_puart_interrupt_callback(void* unused) {
	// There can be at most 16 bytes in the HW FIFO.
	char readbytes[16];

	UINT8 number_of_bytes_read = 0;

	// empty the FIFO
	while(puart_rxFifoNotEmpty() && puart_read(&readbytes[number_of_bytes_read])) {
		number_of_bytes_read++;
	}

	// readbytes should have number_of_bytes_read bytes of data read from puart. Do something with this.
	// clear the interrupt
	P_UART_INT_CLEAR(P_UART_ISR_RX_AFF_MASK);

	// enable UART interrupt in the Main Interrupt Controller and RX Almost Full in the UART Interrupt Controller
	P_UART_INT_ENABLE |= P_UART_ISR_RX_AFF_MASK;

	return 0;
}
