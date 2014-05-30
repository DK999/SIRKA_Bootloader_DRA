/*
 * usart.h
 *
 *  Created on: 27.05.2014
 *      Author: Labor
 */

#ifndef USART_H_
#define USART_H_

#include "efm32tg110f32.h"

uint8_t USART0_receive(void);
void USART0_send(uint8_t data);
void USART0_sendString(char *str);

#endif /* USART_H_ */
