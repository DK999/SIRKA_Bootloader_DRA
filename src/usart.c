/*
 * usart.c
 *
 *  Created on: 27.05.2014
 *      Author: Labor
 */


#include "usart.h"

/******************************************************************************
 * Receives one byte from USART0. This function will stall until
 * a byte is available.
 *****************************************************************************/
 uint8_t USART0_receive(void)
{
  while ( !(USART0->STATUS & USART_STATUS_RXDATAV) );
  return USART0->RXDATA;
}

/******************************************************************************
 * Send one byte over USART0.
 *****************************************************************************/
void USART0_send(uint8_t data)
{

	while( !(USART0->STATUS & (1 << 6)) ); // wait for TX buffer to empty
	USART0->TXDATA = data;                 // send byte over UART
	while (!(USART0->STATUS & USART_STATUS_TXC)) ; // Waiting for transmission of last byte
}

/******************************************************************************
 * Send a string over USART0.
 *****************************************************************************/
void USART0_sendString(char *str)
{
  while (1) {
    if ( *str == 0 )
      break;
    USART0_send((uint8_t)*str++);
  }

//  while ( !(USART0->STATUS & USART_STATUS_TXC) );
}
