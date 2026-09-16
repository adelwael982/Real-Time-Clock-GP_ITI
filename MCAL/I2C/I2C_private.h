/*
 * I2C_private.h
 *
 *  Created on: Sep 9, 2026
 *      Author: DELL
 */

#ifndef MCAL_I2C_I2C_PRIVATE_H_
#define MCAL_I2C_I2C_PRIVATE_H_

#define TWBR	*((volatile u8*)0x20)
#define TWSR	*((volatile u8*)0x21)
#define TWAR	*((volatile u8*)0x22)
#define TWDR	*((volatile u8*)0x23)
#define TWCR	*((volatile u8*)0x56)

#define TWIE	0
#define TWEN	2
#define TWWC	3
#define TWSTO	4
#define TWSTA	5
#define TWEA	6
#define TWINT	7

#define TWPS0	0
#define TWPS1	1

#endif /* MCAL_I2C_I2C_PRIVATE_H_ */
