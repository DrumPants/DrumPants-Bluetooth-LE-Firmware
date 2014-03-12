broadcom-wiced-ble-uart
=======================

An example of how to use the peripheral UART on a Broadcom BCM20732 Bluetooth Low Energy module using the Wiced Smart SDK. 

The function of note is `onUARTReceive()` in hello_sensor.c: that's your callback whenever the UART receives data.

Note this is a very simple implementation to prove that it works. You'll probably want to buffer the UART according to your application's needs.

Derived by Tyler Freeman from sample code in the "Wiced Smart Interfaces" PDF, page 15.


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



Building
-----------------

You can build it without using Eclipse. Just run

C:\Users\tyler\Documents\WICED\WICED-Smart-SDK-1.1.0\WICED-Smart-SDK>make.exe RAM.hello_sensor-BCM920732TAG_Q32 download UART=COM4