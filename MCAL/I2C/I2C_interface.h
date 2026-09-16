/*
 * I2C_interface.h
 *
 *  Created on: Sep 9, 2026
 *      Author: DELL
 */

#ifndef MCAL_I2C_I2C_INTERFACE_H_
#define MCAL_I2C_I2C_INTERFACE_H_

void I2C_Init(void);
void I2C_Start(void);
void I2C_RepeatedStart(void);
void I2C_Stop(void);
void I2C_WriteByte(u8 data);
u8   I2C_ReadByteACK(void);
u8   I2C_ReadByteNACK(void);
u8   I2C_GetStatus(void);

#endif /* MCAL_I2C_I2C_INTERFACE_H_ */
