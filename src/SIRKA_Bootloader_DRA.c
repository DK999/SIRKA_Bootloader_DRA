/**************************************************************************//**
 * @file
 * @brief SIRKA_Bootloader_DRA
 * @author Budelmann Elektronik
 * @version 1.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 *
 *****************************************************************************/
#include "efm32tg110f32.h"
#include "em_chip.h"
#include "em_msc.h"
#include "write_to_flash.h"
#include "boot.h"
#include "init.h"
#include "usart.h"

#define USART_CS_PORT 2
#define USART_CS_PIN 14

#define PREAMBLE_ONE 0
#define PREAMBLE_TWO 1
#define LENGTH 2
#define ADDRESS 3
#define COMMAND 4
#define PARAMETER 5
#define CRCL 6
#define CRCH 7

uint8_t received_frame[14];
short frame_position;

void sendWelcomeMessage(void)
{ GPIO->P[USART_CS_PORT].DOUTSET = (1 << USART_CS_PIN);
  if (RMU->RSTCAUSE & (0x01 << 4))
	USART0_sendString("WATCHDOG triggered! Error on transmit\r\n\r\n");

  USART0_sendString("SIRKA Bootloader 1.0\r\n");

  USART0_sendString("Please enter a command: \r\n"
                    " 0x00 - Upload Firmware\r\n"
		  	  	  	" 0x01 - Verify Firmware\r\n"
		  	  	  	" 0x02 - Boot Firmware\r\n");
  GPIO->P[USART_CS_PORT].DOUTCLR = (1 << USART_CS_PIN);
  EMU->AUXCTRL = 0x1;
}

void sendVerify(uint8_t check_value)
{ GPIO->P[USART_CS_PORT].DOUTSET = (1 << USART_CS_PIN);
  if(check_value)
  USART0_sendString("Firmware VALID!\n\r");
  else
  USART0_sendString("Firmware INVALID!!\n\r");
  GPIO->P[USART_CS_PORT].DOUTSET = (1 << USART_CS_PIN);
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{ uint8_t *boot_flag = (uint8_t*)0x380D;
  /* Chip errata */
  CHIP_Init();
  Init_Clocks();
  Init_GPIO();
  Init_USART0();
  Init_WD();
  Init_TIMER0();
  Init_Interrupts();

  if(*boot_flag == 0x00 && check_firmware() == 0x01 )
	  BOOT_boot();

  /* Infinite loop */
  while (1)
  {
	  sendWelcomeMessage();
	  wait_for_package();
	  //  		  crc = 0x0000;
	  //  		  for(int i = 0; i < (received_frame[LENGTH]-2) ; i++)
	  //  		  {
	  //  			  crc = CRC16(crc,received_frame[i]);
	  //  		  }
	  //  		  crc_ex = (uint16_t)received_frame[CRCH];               // Store CRC-m_checksum
	  //  		  crc_ex <<= 8;
	  //  		  crc_ex |= received_frame[CRCL];
	  //  		  /*
	  //  		   * Check if Preamble OK and frame fully received
	  //  		   */
	  //  		  if(crc == crc_ex)
	  //  		  {

	  		  if((received_frame[PREAMBLE_ONE] == 0xAA) && (received_frame[PREAMBLE_TWO] == 0xAA))
	  			{
	  			  /*
	  			   * Check for address
	  			   */
	  			  if(received_frame[ADDRESS] == 0x7F)
	  			  {	/*
	  				 * select received_frame
	  				 */
	  				switch(received_frame[COMMAND])
	  				  {		/*
	  						 *	Flash Device
	  						 */
	  						case 0x00:	flash_device();
	  									sendVerify(check_firmware());
	  									break;

	  						case 0x01: 	sendVerify(check_firmware());
	  									break;

	  						case 0x02:	if(check_firmware())
	  										BOOT_boot();
	  									else
	  										USART0_sendString("Firmware INVALID!!\n\r");
	  									break;

	  						default:
	  									break;
	  					  } // End SWITCH

	  				  } // End Address-Check
	  			  } // End Preamble-Check
	    		  //} // End CRC
	  	  } // End While(1)

	    } // End Main
