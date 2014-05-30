/*
 * status.h
 *
 *  Created on: 30.05.2014
 *      Author: Labor
 */

#ifndef STATUS_H_
#define STATUS_H_
#include "efm32tg110f32.h"
#include "crc.h"
#include "usart.h"

void send_hello(void);
void send_ok();
void send_crc_fail();


#endif /* STATUS_H_ */
