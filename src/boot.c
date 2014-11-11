/*
 * boot.c
 *
 *  Created on: 27.05.2014
 *      Author: Labor
 *
 */
#include "boot.h"

#define FIRMWARE_START_ADDRESS 0x00004000
#define FIRMWARE_HEADER_SIZE 0x100
#pragma GCC optimize ("O0")
void __attribute ((noinline)) BOOT_jump(uint32_t sp, uint32_t pc)
{
  (void) sp;
  (void) pc;
  /* Set new MSP, PSP based on SP (r0)*/
  __asm("msr msp, r0");
  __asm("msr psp, r0");

  /* Jump to PC (r1)*/
  __asm("mov pc, r1");
}

void BOOT_boot(void)
{

	uint32_t pc, sp;

	uint32_t *bootAddress = (uint32_t *)(FIRMWARE_START_ADDRESS);


	/* Set new vector table */
	SCB->VTOR = (uint32_t)bootAddress;

	/* Read new SP and PC from vector table */
	sp = bootAddress[0];
	pc = bootAddress[1];

	/* Do a jump by loading the PC and SP into the CPU registers */
	BOOT_jump(sp, pc);
}
