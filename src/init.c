/*
 * init.c
 *
 *  Created on: 27.05.2014
 *      Author: Labor
 */


#include "init.h"

#define COM_USART USART0
#define USART_PORT 4
#define USART_CS_PORT 2
#define USART_RX_PIN 12
#define USART_TX_PIN 13
#define USART_CS_PIN 14


void Init_USART0()
{
	/*
	 * Geschwindigkeiten für USART
	 * CLKDIV -> 0x100 = 1MBit/s	16x OVS
	 * CLKDIV -> 0x40 = 1,5MBit/s	16x OVS
	 * CLKDIV -> 0x0 = 2MBit/s		16x OVS
	 *
	 * CLKDIV -> 0x100 = 2MBit/s 	8x OVS
	 * CLKDIV -> 0x0 = 4MBit/s 		8xOVS
	 *
	 * CLKDIV -> 0x300 = 2MBit/s 	4xOVS
	 * CLKDIV -> 0x0 = 8MBit/s		4xOVS
	 */
	COM_USART->CLKDIV = 0x300;
	COM_USART->CMD = (1 << 11) | (1 << 10) | (1 << 2) | (1 << 0);	// CSMA | MSBF | CCEN | ASYNC
	COM_USART->CTRL |= ( 3 << 5 );	// Oversampling 0 = 16x; 1 = 8x; 2 = 6x; 3 = 4x
	COM_USART->IFC = 0x1FF9;
	COM_USART->ROUTE = 0x303;
}

void Init_Clocks()
{
	CMU->CTRL = 0xC062C;            // Set HF clock divider to 0 to keep core frequency 32MHz
	CMU->OSCENCMD |= 0x4;           // Enable XTAL Oscillator
	while(! (CMU->STATUS & 0x8) );  // Wait for XTAL osc to stabilize
	CMU->HFPERCLKDIV = 0x100;
	CMU->CMD = 0x2;                 // Select HF XTAL osc as system clock source. 32MHz XTAL
	CMU->HFPERCLKEN0 = 0x54; 		// Enable GPIO, USART0, and USART1 peripheral clocks
	CMU->LFCLKSEL = 0x5;
	CMU->HFCORECLKEN0 = 0x4;
	CMU->AUXHFRCOCTRL = 0xB3;

	/* Write MSC unlock code to enable interface */
	  MSC->LOCK = MSC_UNLOCK_CODE;

}

void Init_GPIO()
{
	GPIO->P[USART_CS_PORT].MODEH = ( 4 << 24 ) ; // Configure PD0 (MOSI), PD2 (SCK), PD3 (CS), PD4 (nHLD), and PD5 (nWP) as digital outputs and PD1 as input
	GPIO->P[USART_CS_PORT].DOUTCLR = (1 << USART_CS_PIN); // Initialize CS and nHLD high, nWP low
	GPIO->P[USART_PORT].MODEH = ( 4 << 20 ) | (1 << 16);              // Configure PC0 as digital output
	GPIO->P[USART_PORT].DOUTSET = (1 << USART_TX_PIN);   // Initialize PC0 high since UART TX idles high
	GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;						// Enable signal SWO
}

void Init_WD()
{
	WDOG->CTRL = 0xA0C;
	WDOG->CMD = 0x0;
}

void Init_TIMER0()
{
	TIMER0->CTRL = 0x40;
	TIMER0->TOP = 0x140;		// 60µs , former 0x1900
}

void Init_Interrupts(void)
{
	enable_interrupts();
}
