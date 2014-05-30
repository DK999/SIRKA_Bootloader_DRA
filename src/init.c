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
   COM_USART->CLKDIV = 0x100;
   COM_USART->CMD = (1 << 11) | (1 << 10) | (1 << 2) | (1 << 0);
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
	TIMER0->TOP = 0x1900;
	TIMER0->IEN = 0x1;
	NVIC_EnableIRQ(TIMER0_IRQn);              // Enable TIMER0 interrupt vector in NVIC

}

void Init_Interrupts(void)
{
	enable_interrupts();
}
