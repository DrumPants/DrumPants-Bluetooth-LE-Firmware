/***
 * Handles UART communication through the PUART.
 */

#ifndef UART_ONE_WIRE_H
#define UART_ONE_WIRE_H


// Use P33 for peripheral uart RX. (default GPIO_PIN_UART_RX)
#define PUART_RX_PIN GPIO_PIN_UART_RX
// Use P32 for peripheral uart TX. (default GPIO_PIN_UART_TX)
#define PUART_TX_PIN GPIO_PIN_UART_TX

#define PUART_BAUD_RATE 115200


// the callback function when a packet is read.
// - char* buffer : the buffer (no bigger than 16 bytes)
// - int bufferLength : length of buffer (min: 1, max: 16)
typedef void (*FUNC_ON_UART_RECEIVE)(char*, int);


/***
 * Inits the PUART with the given RX callback.
 */
void uart_init(FUNC_ON_UART_RECEIVE callback);

#endif
