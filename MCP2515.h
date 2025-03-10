/*
 * MCP2515.h
 *
 * Created: 18/06/2024 9:55:42 AM
 *  Author: QUOC THANG
 */ 


#ifndef MCP2515_H_
#define MCP2515_H_

#include <avr/io.h>
#include <stdbool.h>
#include "spi.h"

#define MCP2515_CS_PORT		PORTB
#define MCP2515_CS_DDR		DDRB
#define MCP2515_CS_PIN		PINB4

/* MCP2515 SPI Instruction Set */
#define MCP2515_RESET           0xC0

#define MCP2515_READ            0x03
#define MCP2515_READ_RXB0SIDH   0x90
#define MCP2515_READ_RXB0D0     0x92
#define MCP2515_READ_RXB1SIDH   0x94
#define MCP2515_READ_RXB1D0     0x96

#define MCP2515_WRITE           0x02
#define MCP2515_LOAD_TXB0SIDH   0x40    /* TX0 ID location */
#define MCP2515_LOAD_TXB0D0     0x41    /* TX0 Data location */
#define MCP2515_LOAD_TXB1SIDH   0x42    /* TX1 ID location */
#define MCP2515_LOAD_TXB1D0     0x43    /* TX1 Data location */
#define MCP2515_LOAD_TXB2SIDH   0x44    /* TX2 ID location */
#define MCP2515_LOAD_TXB2D0     0x45    /* TX2 Data location */

#define MCP2515_RTS_TX0         0x81
#define MCP2515_RTS_TX1         0x82
#define MCP2515_RTS_TX2         0x84
#define MCP2515_RTS_ALL         0x87
#define MCP2515_READ_STATUS     0xA0
#define MCP2515_RX_STATUS       0xB0
#define MCP2515_BIT_MOD         0x05


/* TXRTSCTRL: TXnRTS PIN CONTROL AND STATUS REGISTER (ADDRESS: 0Dh) */

#define MCP2515_TXRTSCTRL   0x0D

/* MCP25152515 Register Adresses */
#define MCP2515_RXF0SIDH	0x00
#define MCP2515_RXF0SIDL	0x01
#define MCP2515_RXF0EID8	0x02
#define MCP2515_RXF0EID0	0x03
#define MCP2515_RXF1SIDH	0x04
#define MCP2515_RXF1SIDL	0x05
#define MCP2515_RXF1EID8	0x06
#define MCP2515_RXF1EID0	0x07
#define MCP2515_RXF2SIDH	0x08
#define MCP2515_RXF2SIDL	0x09
#define MCP2515_RXF2EID8	0x0A
#define MCP2515_RXF2EID0	0x0B
#define MCP2515_CANSTAT		0x0E
#define MCP2515_CANCTRL		0x0F

#define MCP2515_RXF3SIDH	0x10
#define MCP2515_RXF3SIDL	0x11
#define MCP2515_RXF3EID8	0x12
#define MCP2515_RXF3EID0	0x13
#define MCP2515_RXF4SIDH	0x14
#define MCP2515_RXF4SIDL	0x15
#define MCP2515_RXF4EID8	0x16
#define MCP2515_RXF4EID0	0x17
#define MCP2515_RXF5SIDH	0x18
#define MCP2515_RXF5SIDL	0x19
#define MCP2515_RXF5EID8	0x1A
#define MCP2515_RXF5EID0	0x1B
#define MCP2515_TEC		0x1C
#define MCP2515_REC		0x1D

#define MCP2515_RXM0SIDH	0x20
#define MCP2515_RXM0SIDL	0x21
#define MCP2515_RXM0EID8	0x22
#define MCP2515_RXM0EID0	0x23
#define MCP2515_RXM1SIDH	0x24
#define MCP2515_RXM1SIDL	0x25
#define MCP2515_RXM1EID8	0x26
#define MCP2515_RXM1EID0	0x27
#define MCP2515_CNF3		0x28
#define MCP2515_CNF2		0x29
#define MCP2515_CNF1		0x2A
#define MCP2515_CANINTE		0x2B
#define MCP2515_CANINTF		0x2C
#define MCP2515_EFLG		0x2D

#define MCP2515_TXB0CTRL	0x30
#define MCP2515_TXB1CTRL	0x40
#define MCP2515_TXB2CTRL	0x50
#define MCP2515_RXB0CTRL	0x60
#define MCP2515_RXB0SIDH	0x61
#define MCP2515_RXB1CTRL	0x70
#define MCP2515_RXB1SIDH	0x71

/*Transmit buffer */

#define MCP2515_TXB0SIDH	0x31
#define MCP2515_TXB0SIDL	0x32
#define MCP2515_TXB0EID8	0x33
#define MCP2515_TXB0EID0	0x34
#define MCP2515_TXB0DLC	    0x35
#define MCP2515_TXB0Dm		0x36

#define MCP2515_TXB1SIDH	0x41
#define MCP2515_TXB1SIDL	0x42
#define MCP2515_TXB1EID8	0x43
#define MCP2515_TXB1EID0	0x44
#define MCP2515_TXB1DLC	    0x45
#define MCP2515_TXB1Dm		0x46

#define MCP2515_TXB2SIDH	0x51
#define MCP2515_TXB2SIDL	0x52
#define MCP2515_TXB2EID8	0x53
#define MCP2515_TXB2EID0	0x54
#define MCP2515_TXB2DLC	    0x55
#define MCP2515_TXB2Dm		0x56


/* Defines for Rx Status */
#define MSG_IN_RXB0             0x01
#define MSG_IN_RXB1             0x02
#define MSG_IN_BOTH_BUFFERS     0x03

typedef union{
	struct{
		unsigned RX0IF      : 1;
		unsigned RX1IF      : 1;
		unsigned TXB0REQ    : 1;
		unsigned TX0IF      : 1;
		unsigned TXB1REQ    : 1;
		unsigned TX1IF      : 1;
		unsigned TXB2REQ    : 1;
		unsigned TX2IF      : 1;
	};
	uint8_t ctrl_status;
}ctrl_status_t;

typedef union{
	struct{
		unsigned filter     : 3;
		unsigned msgType    : 2;
		unsigned unusedBit  : 1;
		unsigned rxBuffer   : 2;
	};
	uint8_t ctrl_rx_status;
}ctrl_rx_status_t;

typedef union{
	struct{
		unsigned EWARN      :1;
		unsigned RXWAR      :1;
		unsigned TXWAR      :1;
		unsigned RXEP       :1;
		unsigned TXEP       :1;
		unsigned TXBO       :1;
		unsigned RX0OVR     :1;
		unsigned RX1OVR     :1;
	};
	uint8_t error_flag_reg;
}ctrl_error_status_t;

typedef union{
	struct{
		uint8_t RXBnSIDH;
		uint8_t RXBnSIDL;
		uint8_t RXBnEID8;
		uint8_t RXBnEID0;
		uint8_t RXBnDLC;
		uint8_t RXBnD0;
		uint8_t RXBnD1;
		uint8_t RXBnD2;
		uint8_t RXBnD3;
		uint8_t RXBnD4;
		uint8_t RXBnD5;
		uint8_t RXBnD6;
		uint8_t RXBnD7;
	};
	uint8_t rx_reg_array[13];
}rx_reg_t;

/* MCP2515 Registers */
typedef struct{
	uint8_t RXF0SIDH;
	uint8_t RXF0SIDL;
	uint8_t RXF0EID8;
	uint8_t RXF0EID0;
}RXF0;

typedef struct{
	uint8_t RXF1SIDH;
	uint8_t RXF1SIDL;
	uint8_t RXF1EID8;
	uint8_t RXF1EID0;
}RXF1;

typedef struct{
	uint8_t RXF2SIDH;
	uint8_t RXF2SIDL;
	uint8_t RXF2EID8;
	uint8_t RXF2EID0;
}RXF2;

typedef struct{
	uint8_t RXF3SIDH;
	uint8_t RXF3SIDL;
	uint8_t RXF3EID8;
	uint8_t RXF3EID0;
}RXF3;

typedef struct{
	uint8_t RXF4SIDH;
	uint8_t RXF4SIDL;
	uint8_t RXF4EID8;
	uint8_t RXF4EID0;
}RXF4;

typedef struct{
	uint8_t RXF5SIDH;
	uint8_t RXF5SIDL;
	uint8_t RXF5EID8;
	uint8_t RXF5EID0;
}RXF5;

typedef struct{
	uint8_t RXM0SIDH;
	uint8_t RXM0SIDL;
	uint8_t RXM0EID8;
	uint8_t RXM0EID0;
}RXM0;

typedef struct{
	uint8_t RXM1SIDH;
	uint8_t RXM1SIDL;
	uint8_t RXM1EID8;
	uint8_t RXM1EID0;
}RXM1;

typedef struct{
	uint8_t tempSIDH;
	uint8_t tempSIDL;
	uint8_t tempEID8;
	uint8_t tempEID0;
}id_reg_t;

// void MCP2515_init(void);
// void MCP2515_select(void);
// void MCP2515_deselect(void); 
// uint8_t MCP2515_read_status(void);
// void MCP2515_write(uint8_t address, uint8_t data);
// void MCP2515_reset(void);


bool MCP2515_SetNormalMode(void);
bool MCP2515_SetConfigMode(void);
void MCP2515_reset(void);
uint8_t MCP2515_read(uint8_t address);
void MCP2515_ReadRxSequence(uint8_t instruction, uint8_t *data, uint8_t length);
void MCP2515_write(volatile uint8_t address, volatile uint8_t data);
void MCP2515_WriteByteSequence(uint8_t startAddress, uint8_t endAddress, uint8_t *data);
void MCP2515_LoadTxBuffer(uint8_t instruction, uint8_t data);
void MCP2515_RequestToSend(uint8_t instruction);
uint8_t MCP2515_read_status(void);
uint8_t MCP2515_GetRxStatus(void);
void MCP2515_BitModify(uint8_t address, uint8_t mask, uint8_t data);
void MCP2515_LoadTxSequence(uint8_t instruction, uint8_t *idReg, uint8_t dlc, uint8_t *data);

uint8_t MCP2515_Trans0(uint8_t *data);
uint8_t MCP2515_Trans1(uint8_t *data);
uint8_t MCP2515_Trans2(uint8_t *data);

#endif /* MCP2515_H_ */