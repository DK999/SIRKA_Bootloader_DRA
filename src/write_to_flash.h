/*
 * write_to_flash.h
 *
 *  Created on: 27.05.2014
 *      Author: Labor
 */

#ifndef WRITE_TO_FLASH_H_
#define WRITE_TO_FLASH_H_

#include "efm32tg110f32.h"
//#include "em_msc.h"
#include "msc.h"
#include "watchdog.h"
#include "emu.h"
#include "crc.h"
#include "usart.h"

void flash_device();
uint8_t check_firmware();
void get_nr_of_packages();
void send_ok();
void write_to_flash();
void wait_for_package();
void send_crc_fail();
int check_crc();

#endif /* WRITE_TO_FLASH_H_ */
