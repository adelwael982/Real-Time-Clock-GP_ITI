/*

 * KEYPAD_config.h
 *
 *  Created on: Aug 30, 2026
 *      Author: mosta
 */

#ifndef HAL_KEYPAD_KEYPAD_CONFIG_H_
#define HAL_KEYPAD_KEYPAD_CONFIG_H_


extern const  u8 KPD_Switch [4][4] ;

/* FIXED: schematic wires the keypad rows/columns to PORTB (B0-B7), not
 * PORTD. With PORTD, KEYPAD_GetPressed() was scanning/reading a port the
 * keypad isn't physically connected to, so it always returned 0 - no
 * key press was ever detected, regardless of which button was pressed. */
#define  KEYPAD_PORT 			DIO_PORTB

#define  KEYPAD_COLUMN_START 	DIO_PIN0
#define  KEYPAD_COLUMN_END   	DIO_PIN3

#define  KEYPAD_ROW_START 	DIO_PIN4
#define  KEYPAD_ROW_END   	DIO_PIN7

#endif /* HAL_KEYPAD_KEYPAD_CONFIG_H_ */
