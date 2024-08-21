/*
 * MCP2515.c
 *
 * Created: 18/06/2024 9:55:32 AM
 *  Author: QUOC THANG
 */ 
#include "MCP2515.h"
#include "spi.h"
#include <util/delay.h>

#define MCP2515_CS_HIGH()   MCP2515_CS_PORT |= (1 << MCP2515_CS_PIN)
#define MCP2515_CS_LOW()	MCP2515_CS_PORT &= ~(1 << MCP2515_CS_PIN)


bool MCP2515_SetNormalMode(void)
{
	/* configure CANCTRL Register */
	MCP2515_write(MCP2515_CANCTRL, 0x08);
	
	uint8_t loop = 10;
	
	do {
		/* confirm mode configuration */
		if((MCP2515_read(MCP2515_CANSTAT) & 0xE0) == 0x00)
		return true;
		
		loop--;
	} while(loop > 0);
	
	return false;
}

/* change mode as configuration mode */
bool MCP2515_SetConfigMode(void)
{
	/* configure CANCTRL Register */
	MCP2515_write(MCP2515_CANCTRL, 0x80);
	
	uint8_t loop = 10;
	
	do {
		/* confirm mode configuration */
		if((MCP2515_read(MCP2515_CANSTAT) & 0xE0) == 0x80)
		return true;
		
		loop--;
	} while(loop > 0);
	
	return false;
}

void MCP2515_reset(void)
{
	MCP2515_CS_LOW();
	SPI_SendByte(MCP2515_RESET);
	MCP2515_CS_HIGH();
}

/* read single byte */
uint8_t MCP2515_read(uint8_t address)
{
	uint8_t retVal;
	
	MCP2515_CS_LOW();
	
	SPI_SendByte(MCP2515_READ);
	SPI_SendByte(address);
	retVal = SPI_SendByte(0x00);
	
	MCP2515_CS_HIGH();
	
	return retVal;
}

/* read buffer */
void MCP2515_ReadRxSequence(uint8_t instruction, uint8_t *data, uint8_t length)
{
	MCP2515_CS_LOW();
	
	SPI_SendByte(instruction);
	SPI_RxBuffer(data, length);
	
	MCP2515_CS_HIGH();
}

/* write single byte */
void MCP2515_write(volatile uint8_t address, volatile uint8_t data)
{
	MCP2515_CS_LOW();
	SPI_SendByte(MCP2515_WRITE);
	SPI_SendByte(address);
	SPI_SendByte(data);
	MCP2515_CS_HIGH();
}


void MCP2515_WriteByteSequence(uint8_t startAddress, uint8_t endAddress, uint8_t *data)
{
	MCP2515_CS_LOW();
	
	SPI_SendByte(MCP2515_WRITE);
	SPI_SendByte(startAddress);
	SPI_TxBuffer(data, (endAddress - startAddress + 1));
	
	MCP2515_CS_HIGH();
}

/* write to TxBuffer(1 byte) */
void MCP2515_LoadTxBuffer(uint8_t instruction, uint8_t data)
{
	MCP2515_CS_LOW();
	
	SPI_SendByte(instruction);
	SPI_SendByte(data);
	
	MCP2515_CS_HIGH();
}

/* request to send */
void MCP2515_RequestToSend(uint8_t instruction)
{
	MCP2515_CS_LOW();
	
	SPI_SendByte(instruction);
	
	MCP2515_CS_HIGH();
}


uint8_t MCP2515_read_status(void)
{
	uint8_t result;
	MCP2515_CS_LOW();
	SPI_SendByte(MCP2515_READ_STATUS);
	result = SPI_SendByte(0x00); // G?i dummy byte ?? nh?n d? li?u
	MCP2515_CS_HIGH();
	return result;
}

/* read RX STATUS register */
uint8_t MCP2515_GetRxStatus(void)
{
	uint8_t retVal;
	
	MCP2515_CS_LOW();
	
	SPI_SendByte(MCP2515_RX_STATUS);
	retVal = SPI_ReadByte();
	
	MCP2515_CS_HIGH();
	
	return retVal;
}

/* Use when changing register value */
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data)
{
	MCP2515_CS_LOW();
	
	SPI_SendByte(MCP2515_BIT_MOD);
	SPI_SendByte(address);
	SPI_SendByte(mask);
	SPI_SendByte(data);
	
	MCP2515_CS_HIGH();
}

/* write to TxBuffer */
void MCP2515_LoadTxSequence(uint8_t instruction, uint8_t *idReg, uint8_t dlc, uint8_t *data)
{
	MCP2515_CS_LOW();
	
	SPI_SendByte(instruction);
	SPI_TxBuffer(idReg, 4);
	SPI_SendByte(dlc);
	SPI_TxBuffer(data, dlc);
	
	MCP2515_CS_HIGH();
}

uint8_t MCP2515_Trans0(uint8_t *data)
{
	uint8_t retVal;

	// write ID high
	MCP2515_write(MCP2515_TXB0SIDH, 0x24);
/*	retVal = MCP2515_read(MCP2515_TXB0SIDH);*/
	// write ID low
	MCP2515_write(MCP2515_TXB0SIDL, 0x60);
/*	retVal = MCP2515_read(MCP2515_TXB0SIDL);*/

	MCP2515_write(MCP2515_TXB0EID8,0x00);
/*	retVal=MCP2515_read(MCP2515_TXB0EID8);*/
	//write ID_ext low
	MCP2515_write(MCP2515_TXB0EID0,0x00);
/*	retVal=MCP2515_read(MCP2515_TXB0EID0);*/

	MCP2515_write(MCP2515_TXB0DLC, 0x08);
/*	retVal = MCP2515_read(MCP2515_TXB0DLC);*/
	
	// write 8 byte data
	for (int i = 0; i < 8; ++i)
	{
		MCP2515_write(MCP2515_TXB0Dm + i, data[i]);
	}
	MCP2515_write(MCP2515_TXB0CTRL, 0x0B);
	retVal = MCP2515_read(MCP2515_TXB0CTRL);


	if ((retVal >> 4) & 0x01)
	{
		return 0;
	}
		
	MCP2515_write(MCP2515_TXRTSCTRL, 0x01);
	MCP2515_CS_LOW();
	//  SPI_Tx(MCP2515_LOAD_TXB0D0);
	SPI_SendByte(MCP2515_RTS_TX0);
	MCP2515_CS_HIGH();
	return 1;
}

uint8_t MCP2515_Trans1(uint8_t *data)
{
	static uint8_t retVal;

	// write ID high
	MCP2515_write(MCP2515_TXB1SIDH, 0x24);
/*	retVal = MCP2515_read(MCP2515_TXB1SIDH);*/
	// write ID low
	MCP2515_write(MCP2515_TXB1SIDL, 0x60);
/*	retVal = MCP2515_read(MCP2515_TXB1SIDL);*/

	MCP2515_write(MCP2515_TXB1EID8,0x00);
/*	retVal=MCP2515_read(MCP2515_TXB1EID8);*/
	//write ID_ext low
	MCP2515_write(MCP2515_TXB1EID0,0x00);
/*	retVal=MCP2515_read(MCP2515_TXB1EID0);*/
	
	MCP2515_write(MCP2515_TXB1DLC, 0x08);
/*	retVal = MCP2515_read(MCP2515_TXB1DLC);*/
	// write 8 byte data
	for (int i = 0; i < 8; ++i)
	{
		MCP2515_write(MCP2515_TXB1Dm + i, data[i]);
	}
	MCP2515_write(MCP2515_TXB1CTRL, 0x0B);
	retVal = MCP2515_read(MCP2515_TXB1CTRL);

	if ((retVal >> 4) & 0x01)
	{
		return 0;
	}

	MCP2515_write(MCP2515_TXRTSCTRL, 0x02);
	MCP2515_CS_LOW();
	//  SPI_Tx(MCP2515_LOAD_TXB1D0);
	SPI_SendByte(MCP2515_RTS_TX1);
	MCP2515_CS_HIGH();
	return 1;

}

uint8_t MCP2515_Trans2(uint8_t *data)
{
	static uint8_t retVal;

	// write ID high
	MCP2515_write(MCP2515_TXB2SIDH, 0x24);
/*	retVal = MCP2515_read(MCP2515_TXB2SIDH);*/
	// write ID low
	MCP2515_write(MCP2515_TXB2SIDL, 0x60);
/*	retVal = MCP2515_read(MCP2515_TXB2SIDL);*/

	MCP2515_write(MCP2515_TXB2EID8,0x00);
// 	retVal=MCP2515_read(MCP2515_TXB2EID8);
	//write ID_ext low
	MCP2515_write(MCP2515_TXB2EID0,0x00);
/*	retVal=MCP2515_read(MCP2515_TXB2EID0);*/

	MCP2515_write(MCP2515_TXB2DLC, 0x08);
/*	retVal = MCP2515_read(MCP2515_TXB2DLC);*/
	// write 8 byte data
	for (int i = 0; i < 8; ++i)
	{
		MCP2515_write(MCP2515_TXB2Dm + i, data[i]);
	}

	MCP2515_write(MCP2515_TXB2CTRL, 0x0B);
	retVal = MCP2515_read(MCP2515_TXB2CTRL);

	if ((retVal >> 4) & 0x01)
	{
		return 0;
	}

	MCP2515_write(MCP2515_TXRTSCTRL, 0x04);
	MCP2515_CS_LOW();
	//  SPI_Tx(MCP2515_LOAD_TXB2D0);
	SPI_SendByte(MCP2515_RTS_TX2);
	MCP2515_CS_HIGH();
	return 1;
}