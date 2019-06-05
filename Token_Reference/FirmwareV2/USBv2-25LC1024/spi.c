/*****************************************************************************
 *   25LC1024 EEPROM Function
 *	 Luong Anh Hoang
 *	 01/01/2010
 *
*****************************************************************************/
#include <stdio.h>
#include <AT91SAM7S256.H>                    /* AT91SAM7S256 definitions */
#include "type.h"
#include "spi.h"
#include "lib_at91sam7s256.h"
#include "board.h"

#define SPI_SCKDIV 8
volatile DWORD SPI0Status = 0;
volatile DWORD TxCounter = 0;

AT91PS_SPI s_pSpi = AT91C_BASE_SPI;
AT91PS_PIO s_pPio = AT91C_BASE_PIOA;
AT91PS_PMC s_pPMC = AT91C_BASE_PMC;
AT91PS_PDC s_pPDC = AT91C_BASE_PDC_SPI;

#define SPI_NPCS0	(unsigned int)1<<11
#define SPI_MISO	(unsigned int)1<<12
#define SPI_MOSI	(unsigned int)1<<13
#define SPI_SPCK	(unsigned int)1<<14
#define SPI_CS0	(1<<11)	///< PIO11 SPI CS 0
#define SPI_MASTER 1


BOOL InitSPI(void)
{
     // enable clock to SPI interface
     AT91C_BASE_PMC->PMC_PCER |= (1<<AT91C_ID_SPI);
 
     // setup PIO pins for SPI bus
     *AT91C_PIOA_ASR   = AT91C_PA12_MISO|AT91C_PA13_MOSI|AT91C_PA14_SPCK;    // assign pins to SPI interface
     *AT91C_PIOA_PDR   = AT91C_PA12_MISO|AT91C_PA13_MOSI|AT91C_PA14_SPCK;
     *AT91C_PIOA_PPUER = AT91C_PA12_MISO|AT91C_PA13_MOSI|AT91C_PA14_SPCK;    // set pullups
     // setup PIO pins for SPI chip selects
     //AT91C_BASE_PIOA->PIO_ASR = AT91C_PA11_NPCS0|AT91C_PA31_NPCS1;
     //AT91C_BASE_PIOA->PIO_PDR = AT91C_PA11_NPCS0|AT91C_PA31_NPCS1;
     //AT91C_BASE_PIOA->PIO_OER = AT91C_PA11_NPCS0|AT91C_PA31_NPCS1;
 
     // reset and enable SPI
     *AT91C_SPI_CR = AT91C_SPI_SPIEN | AT91C_SPI_SWRST;
     *AT91C_SPI_CR = AT91C_SPI_SPIEN;
 
     // set master mode with:
    //  - SPI master
     //  - no mode fault
     //  - variable peripheral chip select
     *AT91C_SPI_MR = AT91C_SPI_MODFDIS | AT91C_SPI_PS_VARIABLE | AT91C_SPI_MSTR;
 //  *AT91C_SPI_MR = AT91C_SPI_MODFDIS | AT91C_SPI_PS_FIXED | AT91C_SPI_MSTR | (0x0E<<16);
 
     // setup data transfer format and rate for device 0-3 => 8bits, CPOL=0, NCPHA=1
    AT91C_SPI_CSR[0] = AT91C_SPI_NCPHA|AT91C_SPI_BITS_8|(SPI_SCKDIV<<8);
    //AT91C_SPI_CSR[1] = AT91C_SPI_NCPHA|AT91C_SPI_BITS_8|(SPI_SCKDIV<<8);
    //AT91C_SPI_CSR[2] = AT91C_SPI_NCPHA|AT91C_SPI_BITS_8|(SPI_SCKDIV<<8);
    //AT91C_SPI_CSR[3] = AT91C_SPI_NCPHA|AT91C_SPI_BITS_8|(SPI_SCKDIV<<8);
    AT91F_PIO_CfgOutput(AT91C_BASE_PIOA, SPI0_SEL);//Cau hinh Output cho chan CS0
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);//Dat chan nay o muc cao
	return (TRUE);
}
BYTE spiTransferByte(BYTE data)
{
     // wait for transmit completion/ready
     while(!(*AT91C_SPI_SR & AT91C_SPI_TDRE));
     // write data to be transmitted
     *AT91C_SPI_TDR = data;
     // wait for completion
     while(!(*AT91C_SPI_SR & AT91C_SPI_RDRF));
     // return received data
     return *AT91C_SPI_RDR;
}

/*****************************************************************************
** Function name:		SPISend
**
** Descriptions:		Send a block of data to the SPI port, the first
**				parameter is the buffer pointer, the 2nd 
**				parameter is the block length.
**
** parameters:			buffer pointer, and the block length
** Returned value:		None
** 
*****************************************************************************/
void SPISend( BYTE *buf, DWORD Length )
{

  DWORD i;
  BYTE dump;
  if ( Length == 0 )
	return;
  for ( i = 0; i < Length; i++ )
  {
     // wait for transmit completion/ready
     while(!(*AT91C_SPI_SR & AT91C_SPI_TDRE));
     // write data to be transmitted
     *AT91C_SPI_TDR = *buf;
     // wait for completion
     while(!(*AT91C_SPI_SR & AT91C_SPI_RDRF));
	 dump = *AT91C_SPI_RDR; //flush RDR
	 buf++;
  }
  return; 
}

/*****************************************************************************
** Function name:		SPIReceive
** Descriptions:		the module will receive a block of data from 
**				the SPI, the 2nd parameter is the block length.
** parameters:			buffer pointer, and block length
** Returned value:		None
** 
*****************************************************************************/
void SPIReceive( BYTE *buf, DWORD Length )
{
  DWORD i;

  for ( i = 0; i < Length; i++ )
  {
	*buf = SPIReceiveByte();
	buf++;
  }
  return; 
}

/*****************************************************************************
** Function name:		SPIReceiveByte
**
** Descriptions:		Receive one byte of data from the SPI port
**				Write a dummy byte, wait until SPI transfer
**				complete, then, read the data register to
**				get the SPI data.
**
** parameters:			None
** Returned value:		the data byte received
** 
*****************************************************************************/
BYTE SPIReceiveByte( void )
{

  BYTE data, dump;

/* Cho LPC2378
  // wrtie dummy byte out to generate clock, then read data from MISO 
  S0SPDR = 0xFF;
  // Wait for transfer complete, SPIF bit set 

  while ( !(S0SPSR & SPIF) );
  //while((S0SPSR & 0x80)!=0x80){;;}  // wait for data transfer
  data = S0SPDR;
*/

   // wait for transmit completion/ready
   while(!(*AT91C_SPI_SR & AT91C_SPI_TDRE));
   // write data to be transmitted
   *AT91C_SPI_TDR = dump;
  // wait for completion
   while(!(*AT91C_SPI_SR & AT91C_SPI_RDRF));
   data = *AT91C_SPI_RDR;
   return ( data ); 
}

 /****************************************
 WARNING : THIS FUNCTION WILL ERASE A PAGE 
 Input: 	Page Address
 Output:	None 
 *****************************************/
void EEPROM_ErasePage(DWORD Addr)
{
	BYTE SPICmd[4];
	BYTE SPISRData;
	SPICmd[0] = WREN;
	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
	    			/* set write enable latch */
	SPISend( SPICmd, 1 );
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);
	do
	{
			
			//Gui lenh yeu cau doc thanh ghi trang thai RDSR
			//IOCLR0 = SPI0_SEL;
			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
		  	SPICmd[0] = RDSR;	/* check status to see if write enabled is latched */
		  	SPISend( SPICmd, 1 );
			//Doc ve gia tri thanh ghi trang thai
		  	SPISRData = SPIReceiveByte();
			AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);
		  	//IOSET0 = SPI0_SEL;
			//Kiem tra trang thai san sang
	}   while (SPISRData & (RDSR_WEN|RDSR_RDY) != RDSR_WEN);
	// Bat dau xoa
	SPICmd[0] = ERASEPAGE;
	SPICmd[1] = Addr>>16;
	SPICmd[2] = (Addr>>8)&0xFF;
	SPICmd[3] = Addr&0xFF;

	// Gui lenh xoa mot page
	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
	SPISend( SPICmd, 4 );
	SPISRData = SPIReceiveByte();
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);

	// Kiem tra qua trinh hoan tat
	do
	{
		AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
		SPICmd[0] = RDSR;	/* check status to see if write enabled is latched */
		SPISend( SPICmd, 1 );
		//Doc ve gia tri thanh ghi trang thai
	  	SPISRData = SPIReceiveByte();
		AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);
	} while (SPISRData & RDSR_RDY);
} 


/**********************************************
WARNING: THIS FUNCTION WILL ERASE ALL CHIP DATA
**********************************************/
void EEPROM_EraseChip()
{
	BYTE SPICmd[4];
	BYTE SPISRData;
	SPICmd[0] = WREN;
	printf("EEPROM: Full chip erasing...\r\n");
	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
	    			/* set write enable latch */
	SPISend( SPICmd, 1 );
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);
	do
	{
			
			//Gui lenh yeu cau doc thanh ghi trang thai RDSR
			//IOCLR0 = SPI0_SEL;
			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
		  	SPICmd[0] = RDSR;	/* check status to see if write enabled is latched */
		  	SPISend( SPICmd, 1 );
			//Doc ve gia tri thanh ghi trang thai
		  	SPISRData = SPIReceiveByte();
			AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);
		  	//IOSET0 = SPI0_SEL;
			//Kiem tra trang thai san sang
	}   while (SPISRData & (RDSR_WEN|RDSR_RDY) != RDSR_WEN);
	printf("EEPROM: WREN is set\r\n");

	// Bat dau xoa
	SPICmd[0] = ERASECHIP;
	

	// Gui lenh xoa mot chip
	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
	SPISend( SPICmd, 1 );
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);

	// Kiem tra qua trinh hoan tat
	do
	{
		SPICmd[0] = RDSR;	/* check status to see if write enabled is latched */
		AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
		
		SPISend( SPICmd, 1 );
		//Doc ve gia tri thanh ghi trang thai
	  	SPISRData = SPIReceiveByte();
		AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);
	} while (SPISRData & RDSR_RDY);
	printf("EEPROM: Done\r\n");
} 
void EEPROM_Write(BYTE *srcbuf, DWORD length, DWORD Addr)
{
	BYTE SPICmd[4];
	BYTE SPISRData;
	DWORD dwBytesWritten = 0;

	//printf("EEPROM: Writing %d byte to address %d\r\n",length,Addr);
	
	do
	{
		//Bat dau moi chu ky ghi
		//Gui lenh cho phep ghi	EEPROM 25LC1024
		SPICmd[0] = WREN;
	    //IOCLR0 = SPI0_SEL;
		AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
	    			/* set write enable latch */
	    SPISend( SPICmd, 1 );
		AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);
	    //IOSET0 = SPI0_SEL;
	

		do
	  	{
			//Gui lenh yeu cau doc thanh ghi trang thai RDSR
			//IOCLR0 = SPI0_SEL;
			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
		  	SPICmd[0] = RDSR;	/* check status to see if write enabled is latched */
		  	SPISend( SPICmd, 1 );
			//Doc ve gia tri thanh ghi trang thai
		  	SPISRData = SPIReceiveByte();
			AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);
		  	//IOSET0 = SPI0_SEL;
			//Kiem tra trang thai san sang
		}   while (SPISRData & (RDSR_WEN|RDSR_RDY) != RDSR_WEN);


		//Ghi du lieu
		SPICmd[0] = WRITE;	    /* Write command is 0x02, low 256 bytes only */
	  	SPICmd[1] = Addr>>16;	/* write address offset is 16 bit, addrH addrL */
	  	SPICmd[2] = (Addr>>8)&0xFF;	
		SPICmd[3] = Addr&0xFF;
	  	
		AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
		SPISend(SPICmd,4);
		SPISend(srcbuf+dwBytesWritten,(length-dwBytesWritten)>=256?256:length-dwBytesWritten);
	  	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);
		Addr+=256;
		
		dwBytesWritten+=(length-dwBytesWritten>=256)?256:length-dwBytesWritten;
		// Wait for writing complete
		do
	  	{
			
			AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
		  	SPICmd[0] = RDSR;	/* check status to see if write enabled is latched */
		  	SPISend( SPICmd, 1 );
			//Doc ve gia tri thanh ghi trang thai
		  	SPISRData = SPIReceiveByte();
			AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);
		  	
		}   while (SPISRData & RDSR_RDY);
		//printf("EEPROM: %d bytes written\r\n",dwBytesWritten);		
	

	}
	while (dwBytesWritten<length);
	//printf("EEPROM: Done\r\n");

	return;
}
/* Ham doc du lieu tu 25LC1024 tai dia chi offset bat dau addrHaddL */
void EEPROM_Read(BYTE *desbuf, DWORD length, DWORD Addr)
{
	BYTE cmdRead[4];
	//printf("EEPROM: Reading %d byte from address %d\r\n",length,Addr);

	cmdRead[0] = READ;		/* Read command is 0x03, low 256 bytes only */
  	cmdRead[1] = Addr>>16;	
  	cmdRead[2] = (Addr>>8)&0xFF;
	cmdRead[3] = Addr&0xFF;
	//IOCLR0 = SPI0_SEL;
  	AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, SPI0_SEL);
	SPISend( cmdRead, 4 );  //Gui lenh READ
  	SPIReceive( desbuf, length);
	AT91F_PIO_SetOutput(AT91C_BASE_PIOA, SPI0_SEL);
	
	//IOSET0 = SPI0_SEL;
	
	
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

