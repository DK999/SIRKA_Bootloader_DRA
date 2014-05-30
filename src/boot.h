/*
 * boot.h
 *
 *  Created on: 27.05.2014
 *      Author: Labor
 */

#ifndef BOOT_H_
#define BOOT_H_

#include "efm32tg110f32.h"
#include "ramfunc.h"

 void BOOT_jump(uint32_t sp, uint32_t pc);
 void BOOT_boot(void);

#endif /* BOOT_H_ */
