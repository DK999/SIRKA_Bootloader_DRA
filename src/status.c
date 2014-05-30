/*
 * status.c
 *
 *  Created on: 30.05.2014
 *      Author: Labor
 */

#include "status.h"

#define USART_CS_PORT 2
#define USART_CS_PIN 14

uint8_t finished[5]={0xAA,0xAA,0x05,0x00,0x01};	// Sends if succeeds
uint8_t failed[5]={0xAA,0xAA,0x05,0x00,0x00};	// Sends if CRC fails

void send_hello(void)
{	volatile uint16_t crc = 0x0000;
	uint8_t byte[7];

	byte[0] = 0xAA;							// Setting Preamble
	byte[1] = 0xAA;
	byte[2] = 0x07;							// Setting Frame length (7 Byte)
	byte[3] = 0x00;							// Setting Host Address
	byte[4] = 0x7F;

	GPIO->P[USART_CS_PORT].DOUTSET = (1 << USART_CS_PIN);			// Set RS485 for Write

	for ( int i = 0; i < 5 ; i++)
	{
		crc = CRC16(crc,byte[i]);			// create CRC
		USART0_send(byte[i]);				// Send bytes
	}
	byte[5] = (uint8_t)crc;					// split CRC in two bytes
	byte[6] = (uint8_t)(crc >> 8 );

	USART0_send(byte[5]);					// send CRC LSB
	USART0_send(byte[6]);					// send CRC MSB

	GPIO->P[USART_CS_PORT].DOUTCLR = (1 << USART_CS_PIN);			// Clear RS485 for Read
}

/* Sends success message */
void send_ok()
{
	GPIO->P[USART_CS_PORT].DOUTSET = (1 << USART_CS_PIN);
	for ( int i = 0; i < 5 ; i++)
		USART0_send(finished[i]);
	GPIO->P[USART_CS_PORT].DOUTCLR = (1 << USART_CS_PIN);
}
/* Sends failure message */
void send_crc_fail()
{
	GPIO->P[USART_CS_PORT].DOUTSET = (1 << USART_CS_PIN);
	for ( int i = 0; i < 5 ; i++)
		USART0_send(failed[i]);
	GPIO->P[USART_CS_PORT].DOUTCLR = (1 << USART_CS_PIN);
}
