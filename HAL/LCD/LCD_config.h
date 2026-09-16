/*

 * LCD_config.h
 *
 *  Created on: Aug 27, 2026
 *      Author: mosta
 *
 *  FIXED to match the Proteus schematic:
 *   - RS/RW/E were on PORTB pins 0/1/2 (which collided with the keypad's
 *     columns on the same pins). Moved to PORTC pins 5/6/7, matching the
 *     schematic's C5/C6/C7 connections.
 *   - LCD_DATA_PORT was PORTC. Moved to PORTA, matching the schematic's
 *     A0-A7 connections to the LCD's D0-D7 pins.
 */

#ifndef HAL_LCD_LCD_CONFIG_H_
#define HAL_LCD_LCD_CONFIG_H_

#define  LCD_RS_PORT			DIO_PORTC
#define  LCD_RS_PIN				DIO_PIN5

#define LCD_RW_PORT				DIO_PORTC
#define LCD_RW_PIN				DIO_PIN6

#define LCD_EN_PORT			DIO_PORTC
#define LCD_EN_PIN			DIO_PIN7

#define LCD_MODE			LCD_8_BIT_MODE
#define LCD_DATA_PORT 		DIO_PORTA

#define LCD_D0_PIN			DIO_PIN0
#define	LCD_D0_PORT			DIO_PORTA
#define LCD_D1_PIN			DIO_PIN1
#define	LCD_D1_PORT			DIO_PORTA
#define LCD_D2_PIN			DIO_PIN2
#define	LCD_D2_PORT			DIO_PORTA
#define LCD_D3_PIN			DIO_PIN3
#define	LCD_D3_PORT			DIO_PORTA
#define LCD_D4_PIN			DIO_PIN4
#define	LCD_D4_PORT			DIO_PORTA
#define LCD_D5_PIN			DIO_PIN5
#define	LCD_D5_PORT			DIO_PORTA
#define LCD_D6_PIN			DIO_PIN6
#define	LCD_D6_PORT			DIO_PORTA
#define LCD_D7_PIN			DIO_PIN7
#define	LCD_D7_PORT			DIO_PORTA

#endif /* HAL_LCD_LCD_CONFIG_H_ */
