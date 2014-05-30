/*
 * init.h
 *
 *  Created on: 27.05.2014
 *      Author: Labor
 */

#ifndef INIT_H_
#define INIT_H_

#include "efm32tg110f32.h"
#include "irq.h"

void Init_USART0(void);
void Init_Clocks(void);
void Init_GPIO(void);
void Init_WD(void);
void Init_TIMER0(void);
void Init_Interrupts(void);

#endif /* INIT_H_ */
