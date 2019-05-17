#include <stdio.h>
#include <LPC23xx.H> /* LPC23xx definitions                */
#include "LCD.h"     /* Graphic LCD function prototypes    */
#include "demo.h"

//#define CCLK        4000
#define PAGE_SIZE 1024

unsigned long CCLK = 4000;

struct sIAP
{               // IAP Structure
    U32 cmd;    // Command
    U32 par[4]; // Parameters
    U32 stat;   // Status
    U32 res;    // Result
} IAP;

void IAP_Execute(struct sIAP *pIAP);

int Init(unsigned long adr, unsigned long clk, unsigned long fnc)
{

    IAP.cmd = 54;      // Read Part ID
    IAP_Execute(&IAP); // Execute IAP Command
    if (IAP.stat)
        return (1); // Command Failed

#if 1
    switch ((IAP.res >> 24) & 0xFF)
    {
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
        // LPC 23xx/24xx
    IRC:
        CCLK = 4000; // 4MHz Internal RC Oscillator
        break;
    case 0x05:
    case 0x06:
    case 0x07:
        if ((IAP.res & 0x000F0000) == 0x00030000)
            goto IRC;
    default:
        CCLK = (1049 * (clk >> 10)) >> 10; // Approximate (no Library Code)
    }
#else // based on old Part IDs
    if ((IAP.res & 0x000F0000) == 0x00030000)
    {                // LPC23xx/24xx
        CCLK = 4000; // 4MHz RC Oscillator
    }
    else
    {
        //  CCLK /=  1000;                             // Clock Frequency in kHz
        CCLK = (1049 * (clk >> 10)) >> 10; // Approximate (no Library Code)
    }
#endif

    PLLCON = 0x00;  // Disable PLL (use Oscillator)
    PLLFEED = 0xAA; // Feed Sequence Part #1
    PLLFEED = 0x55; // Feed Sequence Part #2

    MEMMAP = 0x01; // User Flash Mode

    return (0);
}

U32 get_sect_num(U32 addr)
{

    /*U32 sector;
	
	sector = (address >> 12) & 0x7F;

	if (sector >= 0x78) 
		sector -= 0x62;
	else 
	if (sector >= 0x08) 
		sector  = 0x07 + (sector >> 3);
	return sector;*/

    if (addr <= 0x00000FFF)
    {
        return 0;
    }

    if ((addr >= 0x00001000) && (addr <= 0x00001FFF))
    {
        return 1;
    }

    if ((addr >= 0x00002000) && (addr <= 0x00002FFF))
    {
        return 2;
    }

    if ((addr >= 0x00003000) && (addr <= 0x00003FFF))
    {
        return 3;
    }

    if ((addr >= 0x00004000) && (addr <= 0x00004FFF))
    {
        return 4;
    }

    if ((addr >= 0x00005000) && (addr <= 0x00005FFF))
    {
        return 5;
    }

    if ((addr >= 0x00006000) && (addr <= 0x00006FFF))
    {
        return 6;
    }

    if ((addr >= 0x00007000) && (addr <= 0x00007FFF))
    {
        return 7;
    }

    if ((addr >= 0x00008000) && (addr <= 0x0000FFFF))
    {
        return 8;
    }

    if ((addr >= 0x00010000) && (addr <= 0x00017FFF))
    {
        return 9;
    }

    if ((addr >= 0x00018000) && (addr <= 0x0001FFFF))
    {
        return 10;
    }

    if ((addr >= 0x00020000) && (addr <= 0x00027FFF))
    {
        return 11;
    }

    if ((addr >= 0x00028000) && (addr <= 0x0002FFFF))
    {
        return 12;
    }

    if ((addr >= 0x00030000) && (addr <= 0x00037FFF))
    {
        return 13;
    }

    if ((addr >= 0x00038000) && (addr <= 0x0003FFFF))
    {
        return 14;
    }

    if ((addr >= 0x00040000) && (addr <= 0x00047FFF))
    {
        return 15;
    }

    if ((addr >= 0x00048000) && (addr <= 0x0004FFFF))
    {
        return 16;
    }

    if ((addr >= 0x00050000) && (addr <= 0x00057FFF))
    {
        return 17;
    }

    if ((addr >= 0x00058000) && (addr <= 0x0005FFFF))
    {
        return 18;
    }

    if ((addr >= 0x00060000) && (addr <= 0x00067FFF))
    {
        return 19;
    }

    if ((addr >= 0x00068000) && (addr <= 0x0006FFFF))
    {
        return 20;
    }

    if ((addr >= 0x00070000) && (addr <= 0x00077FFF))
    {
        return 21;
    }

    if ((addr >= 0x00078000) && (addr <= 0x00078FFF))
    {
        return 22;
    }

    if ((addr >= 0x00079000) && (addr <= 0x00079FFF))
    {
        return 23;
    }

    if ((addr >= 0x0007A000) && (addr <= 0x0007AFFF))
    {
        return 24;
    }

    if ((addr >= 0x0007B000) && (addr <= 0x0007BFFF))
    {
        return 25;
    }

    if ((addr >= 0x0007C000) && (addr <= 0x0007CFFF))
    {
        return 26;
    }

    if ((addr >= 0x0007D000) && (addr <= 0x0007DFFF))
    {
        return 27;
    }

    /* no valid address within flash */
    return 0;
}

U8 flash_write(U32 address, U8 *buffer)
{
    BOOL bStatus = 0;
    U32 sector;
    __disable_irq();

    // Get the sector number
    sector = get_sect_num(address);

    // Prepare sector for write
    IAP.cmd = 50;        // Command code
    IAP.par[0] = sector; // Start sector
    IAP.par[1] = sector; // End Sector
    IAP_Execute(&IAP);   // Execute IAP Command
    if (IAP.stat)
    {
        bStatus = 1;
        goto RET_LABEL; // Command Failed
    }

    // Copy RAM to FLASH
    IAP.cmd = 51;             // Command Code
    IAP.par[0] = address;     // Destination Flash Address
    IAP.par[1] = (U32)buffer; // Source RAM Address
    IAP.par[2] = PAGE_SIZE;   // Fixed Page Size		// PAGE_SIZE 1024
    IAP.par[3] = CCLK;        // CCLK in kHz
    IAP_Execute(&IAP);        // Execute IAP Command
    if (IAP.stat)
    {
        bStatus = 1;
        goto RET_LABEL; // Command Failed
    }

    bStatus = 0;
RET_LABEL:
    __enable_irq();
    return bStatus;
}

U8 flash_read(U32 address, U8 *buffer, U32 size)
{

    U32 i;
    __disable_irq();
    for (i = 0; i < size; i++)
    {
        buffer[i] = *((U8 *)address + i);
    }
    __enable_irq();
    return 0;
}

U8 flash_erase(U32 address)
{
    unsigned long n;

    n = (unsigned long)get_sect_num(address); // Get Sector Number

    IAP.cmd = 50;      // Prepare Sector for Erase
    IAP.par[0] = n;    // Start Sector
    IAP.par[1] = n;    // End Sector
    IAP_Execute(&IAP); // Execute IAP Command
    if (IAP.stat)
        return (1); // Command Failed

    IAP.cmd = 52;      // Erase Sector
    IAP.par[0] = n;    // Start Sector
    IAP.par[1] = n;    // End Sector
    IAP.par[2] = CCLK; // CCLK in kHz
    IAP_Execute(&IAP); // Execute IAP Command
    if (IAP.stat)
        return (1); // Command Failed

    return (0); // Finished without Errors
}

U8 flash_compare(U32 adr_flash, U32 adr_ram, U32 size)
{

    IAP.cmd = 56;
    IAP.par[0] = adr_flash;
    IAP.par[1] = adr_ram;
    IAP.par[2] = size;
    IAP.par[3] = 0;
    IAP_Execute(&IAP);
    if (IAP.stat)
        return (1); // Command Failed

    return (0); // Finished without Errors
}

// viet add