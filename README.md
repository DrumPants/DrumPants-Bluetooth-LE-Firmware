broadcom-wiced-ble-uart
=======================

An example of how to use the peripheral UART on a Broadcom BCM20732 Bluetooth Low Energy module using the Wiced Smart SDK. 

Note this is a very simple implementation to prove that it works. You'll probably want to buffer the UART according to your application's needs.

Derived from sample code in the "Wiced Smart Interfaces" PDF, page 15, by Tyler Freeman.


Soldering
-------------

The BCM920732TAG dev board doesn't come with jumpers to the PUART, so you'll need to do some ninja soldering:

There are some breakout pads near the BLE module on the board: 

* P32 - TX
* P33 - RX

![Photo](soldering-example.jpg)

This of course can be configured differently, but those are the defaults of GPIO_PIN_UART_TX and GPIO_PIN_UART_RX.

Connected to an Arduino DUE:

![Photo](soldering-Arduino.jpg)