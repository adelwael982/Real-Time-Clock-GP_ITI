/*
 * I2C_program.c
 *
 *  Created on: Sep 9, 2026
 *      Author: DELL
 */

#include "../../STD_TYPES.h"
#include "../../BIT_MATH.h"
#include "I2C_interface.h"
#include "I2C_private.h"

void I2C_Init(void)
{
	/* Set SCL frequency to 100kHz*/
	TWBR = 32;
	//Prescaler to 0
	CLR_BIT(TWSR,TWPS0);
	CLR_BIT(TWSR,TWPS1);
	// Set bit enable
	SET_BIT(TWCR,TWEN);
}

/* Generate a START condition */
void I2C_Start(void)
{
	u8 Local_u8TWCRValue = 0x00;
	SET_BIT(Local_u8TWCRValue, TWINT);
	SET_BIT(Local_u8TWCRValue, TWSTA);
	SET_BIT(Local_u8TWCRValue, TWEN);
	TWCR = Local_u8TWCRValue;
	while ((GET_BIT(TWCR, TWINT)) == 0);
}

/* Generate a Repeated START condition*/
void I2C_RepeatedStart(void)
{
	u8 Local_u8TWCRValue = 0x00;
	SET_BIT(Local_u8TWCRValue, TWINT);
	SET_BIT(Local_u8TWCRValue, TWSTA);
	SET_BIT(Local_u8TWCRValue, TWEN);
	TWCR = Local_u8TWCRValue;
	while ((GET_BIT(TWCR, TWINT)) == 0);
}

/* Generate a STOP condition */
void I2C_Stop(void)
{
	u8 Local_u8TWCRValue = 0x00;
	SET_BIT(Local_u8TWCRValue, TWINT);
	SET_BIT(Local_u8TWCRValue, TWSTO);
	SET_BIT(Local_u8TWCRValue, TWEN);
	TWCR = Local_u8TWCRValue;
}

/* Load a byte into TWDR and transmit it*/
void I2C_WriteByte(u8 data)
{
	u8 Local_u8TWCRValue = 0x00;
	TWDR = data;
	SET_BIT(Local_u8TWCRValue, TWINT);
	SET_BIT(Local_u8TWCRValue, TWEN);
	TWCR = Local_u8TWCRValue;
	while ((GET_BIT(TWCR, TWINT)) == 0);
}

/* Reply with ACK*/
u8 I2C_ReadByteACK(void)
{
	u8 Local_u8TWCRValue = 0x00;
	SET_BIT(Local_u8TWCRValue, TWINT);
	SET_BIT(Local_u8TWCRValue, TWEN);
	SET_BIT(Local_u8TWCRValue, TWEA);
	TWCR = Local_u8TWCRValue;
	while ((GET_BIT(TWCR, TWINT)) == 0);
	return TWDR;
}

/* Reply with NACK */
u8 I2C_ReadByteNACK(void)
{
	u8 Local_u8TWCRValue = 0x00;
	SET_BIT(Local_u8TWCRValue, TWINT);
	SET_BIT(Local_u8TWCRValue, TWEN);
	TWCR = Local_u8TWCRValue;
	while ((GET_BIT(TWCR, TWINT)) == 0);
	return TWDR;
}

/* Return status code */
u8 I2C_GetStatus(void)
{
	return (TWSR & 0xF8);
}
