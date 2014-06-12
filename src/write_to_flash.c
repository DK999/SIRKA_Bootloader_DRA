/*
 * write_to_flash.c
 *
 *  Created on: 27.05.2014
 *      Author: Labor
 */


#include "write_to_flash.h"

#define PREAMBLE_ONE 0
#define PREAMBLE_TWO 1
#define LENGTH 2
#define ADDRESS 3
#define COMMAND 4
#define PARAMETER 5
#define CRCL 6
#define CRCH 7
#define USART_CS_PORT 2
#define USART_CS_PIN 14

#define true    1
#define false   0

uint32_t address = 0x4000;                                              // Stores address for flash writing
uint16_t nr_of_packages = 0;                                    // Stores number of packages

extern uint8_t received_frame[14];                              // Reveived frame by USART
extern short frame_position;                                    // Points at current frame position
uint16_t *crc_pack = (uint16_t*)0x3800;
uint32_t *file_size = (uint32_t*)0x3802;


/* Flash routine */
void flash_device()
{
        address = 0x4000;
        /* Enable Watchdog */
        WDOG_Enable(true);
        /* Get number of packages the fw was split into */
        get_nr_of_packages();
        /* Feed the watchdog! */
        WDOG_Feed();
        /* Send answer for next package */
        send_ok();
        for ( int i = 0; i < nr_of_packages ; i++)
        {
                wait_for_package();
                WDOG_Feed();
                /* Check CRC of package */
                if(check_crc())
                {       /* Write bytes to flash */
                        write_to_flash();
                        WDOG_Feed();
                        /* Send answer for next package */
                        send_ok();
                }
                else
                {       /* Send answer for retry */
                        send_crc_fail();
                }
        }
        /* Getting last frame with information about length of fw in bytes and CRC Value */
        wait_for_package();
        WDOG_Feed();
        /* Check CRC of package */
        if(check_crc())
        {       address = 0x3800;
                /* Delete preamble, overwrite with SIRKA information */
        		for ( int j = 0; j < 8; j++)
                received_frame[j] = received_frame[j+4];
                /* Erase Flash-Page */
                ErasePage(address);
                /* Write CRC and FW length */
                WriteWord(address,&received_frame,8);
        }
        /* disable watchdog */
        WDOG_Enable(false);

}
/* Checks written firmware via CRC, return uint8_t value true or false */
uint8_t check_firmware()
{		uint8_t *App_Byte = (uint8_t*)0x4000;
        volatile uint16_t crc = 0x0000;
        /* Read Byte, calculate CRC and increment address till FW is fully checked */
        for(uint32_t i = 0; i < *file_size ; i++)
        {
          crc = CRC16(crc,*App_Byte);
          App_Byte++;
        }
        /* If FW is correct send true, else false */
        if(crc == *crc_pack)
        {
                return true;
        }
        else
        {
                return false;
        }
}

void reset_bootflag()
{       /* Create array, store SIRKA config and Firmware Info, reset Bootloader Flag */
		uint8_t config[8];                     // Array for config of device
        address = 0x3800;                       // Location for Data to be stored
        config[0] = (uint8_t)*crc_pack;         // CRC Low Byte
        config[1] = (uint8_t)(*crc_pack>>8);    // CRC High Byte
        config[2] = (uint8_t)*file_size;        // Filesize Byte 0
        config[3] = (uint8_t)(*file_size>>8);   // Filesize Byte 1
        config[4] = (uint8_t)(*file_size>>16);  // Filesize Byte 2
        config[5] = (uint8_t)(*file_size>>24);  // Filesize Byte 3
        config[6]= 0x01;                       // Firmware valid flag
        config[7]= 0x00;                       // Bootloader Flag

        ErasePage(address);
        WriteWord(address,&config,8);
}
void get_nr_of_packages()
{       /* Wait for package */
        wait_for_package();
        /* Store 16Bit package length from frame ( LSB First ) */
        nr_of_packages = received_frame[5];
        nr_of_packages<<=8;
        nr_of_packages |= received_frame[4];

}

/* Writes to flash */
void write_to_flash()
{       /* Delete preamble, overwrite with information */
        for ( int j = 0; j < 8; j++)
        received_frame[j] = received_frame[j+4];
        /* Delete new page of flash before writing ( Page-Size is 512 Byte ) */
        if ( address == 0x4000 || address % 0x200 == 0 )
                        ErasePage(address);
        /* Write Data to flash */
        WriteWord(address,&received_frame,8);
        /* Increment address by 8 Byte */
        address+=0x08;
}
/* Waits for package in energymode 1
 * Controlled by interrupt via USART
 * Stores every character received in 'received_frame' and increments frame_position
 * leaves if frame is fully received
 */
void wait_for_package()
{
        frame_position = 0;
        received_frame[LENGTH] = 10;
        while( frame_position !=  received_frame[LENGTH] )                      // stay in sleep mode while not received frame length and address
                  {
                          EMU_EnterEM1();
                  }

}

/* Checks CRC of received package */
int check_crc()
{
  volatile uint16_t crc = 0x0000;
  volatile uint16_t crc_ex = 0x0000;
  for(int i = 0; i < (received_frame[LENGTH]-2) ; i++)
  {
          crc = CRC16(crc,received_frame[i]);
  }
  crc_ex = (uint16_t)received_frame[13];               // Store CRC-m_checksum
  crc_ex <<= 8;
  crc_ex |= received_frame[12];
  if(crc == crc_ex)
          return 1;
  else
          return 0;
}
