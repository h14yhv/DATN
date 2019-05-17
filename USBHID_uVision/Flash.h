#include <stdio.h>
#include <LPC23xx.H>                    /* LPC23xx definitions                */
#include "LCD.h"                        /* Graphic LCD function prototypes    */
#include "demo.h"

#define SIGNATURE_ADDR_IN_FLASH 0x00058000
#define PASSWORD_ADDR_IN_FLASH 0x00059000

int Init (unsigned long adr, unsigned long clk, unsigned long fnc) ;
U32 get_sect_num(U32 addr);
U8 flash_write(U32 address, U8 *buffer);
U8 flash_read(U32 address, U8 *buffer, U32 size) ;
U8 flash_erase(U32 address);
U8 flash_compare(U32 adr_flash, U32 adr_ram, U32 size);
