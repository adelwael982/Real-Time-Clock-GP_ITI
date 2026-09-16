/*



 * LCD_program.c
 *
 *  Created on: Aug 27, 2026
 *      Author: mosta
 */

#include <util/delay.h>
#include "../../STD_TYPES.h"
#include "../../BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "../../MCAL/DIO/DIO_private.h"
#include "LCD_config.h"
#include "LCD_interface.h"

void LCD_init(void)
{
	DIO_SetPinDirection(LCD_RS_PORT,LCD_RS_PIN,DIO_PIN_OUTPUT);
	DIO_SetPinDirection(LCD_RW_PORT,LCD_RW_PIN,DIO_PIN_OUTPUT);
	DIO_SetPinDirection(LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_OUTPUT);
	DIO_SetPortDirection(LCD_DATA_PORT,DIO_PORT_OUTPUT );
	_delay_ms(35);

#if LCD_MODE == LCD_8_BIT_MODE
	LCD_sendcmnd  (0b00111000);
#elif LCD_MODE == LCD_4_BIT_MODE
	DIO_SetPinValue(LCD_RS_PORT,LCD_RS_PIN,DIO_PIN_LOW );
	DIO_SetPinValue(LCD_RW_PORT,LCD_RW_PIN,DIO_PIN_LOW);
	writeHalfPort (0b0010);
	_delay_ms(1);
	DIO_SetPinValue(LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_LOW );
	LCD_sendcmnd(0b00101000);
#endif
	_delay_us(45);
	LCD_sendcmnd(0b00001111);
	_delay_us(45);
	LCD_sendcmnd(0b00000001);
	_delay_ms(2);
	LCD_sendcmnd(0b00000110);
}

void LCD_sendcmnd( u8 cmnd)
{
	DIO_SetPinValue(LCD_RS_PORT,LCD_RS_PIN,DIO_PIN_LOW);
	DIO_SetPinValue(LCD_RW_PORT,LCD_RW_PIN,DIO_PIN_LOW);
#if LCD_MODE == LCD_8_BIT_MODE
	DIO_SetPortValue(LCD_DATA_PORT,cmnd);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_HIGH );
	_delay_ms(1);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_LOW );
#elif  LCD_MODE == LCD_4_BIT_MODE
	writeHalfPort(cmnd>>4);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_HIGH );
	_delay_ms(1);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_LOW );
	writeHalfPort(cmnd);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_HIGH );
	_delay_ms(1);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_LOW );
#endif
}

void LCD_sendChar (u8 data)
{
	DIO_SetPinValue(LCD_RS_PORT,LCD_RS_PIN,DIO_PIN_HIGH);
	DIO_SetPinValue(LCD_RW_PORT,LCD_RW_PIN,DIO_PIN_LOW);
#if LCD_MODE == LCD_8_BIT_MODE
	DIO_SetPortValue(LCD_DATA_PORT ,data);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_HIGH );
	_delay_ms(1);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_LOW );
#elif LCD_MODE == LCD_4_BIT_MODE
	writeHalfPort(data>>4);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_HIGH );
	_delay_ms(1);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_LOW );
	writeHalfPort(data);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_HIGH );
	_delay_ms(1);
	DIO_SetPinValue(  LCD_EN_PORT,LCD_EN_PIN,DIO_PIN_LOW );
#endif
}

void LCD_writeString(u8 * string)
{
	if(string !=NULL )
	{
		u8 stringLength =0 ;
		while(string[stringLength]!= '\0')
		{
			LCD_sendChar(string[stringLength]);
			++stringLength ;
		}
	}
}

void LCD_writeNumber( u32 number)
{
	u32 Local_reversed =1;
	if(number==0) { LCD_sendChar('0'); }
	else
	{
		while (number != 0)
		{
			Local_reversed = Local_reversed*10 + (number%10 );
			number /=10 ;
		}
		do
		{
			LCD_sendChar((Local_reversed%10)+'0');
			Local_reversed /= 10;
		}
		while(Local_reversed!=1);
	}
}

void LCD_clear(void)
{
	LCD_sendcmnd(0b00000001);
	_delay_ms(2);
}

void LCD_shift(u8 shiftingdirection)
{
	if(shiftingdirection == LCD_SHIFT_LEFT)
	{
		LCD_sendcmnd(0b00011000);
		_delay_ms(10);
	}
	else if(shiftingdirection == LCD_SHIFT_RIGHT)
	{
		LCD_sendcmnd(0b00011100);
		_delay_ms(10);
	}
}

void LCD_goToSpecificPosition (u8 LineNumber , u8 Position)
{
	if(LineNumber == LCD_LINE_ONE)
	{
		if(Position<=15) { LCD_sendcmnd(0x80 + Position ); }
	}
	else if(LineNumber == LCD_LINE_TWO)
	{
		if(Position<=15) { LCD_sendcmnd(0xc0 + Position ); }
	}
}

void writeHalfPort(u8 Value)
{
	if (0== GET_BIT(Value,0 ) ) { DIO_SetPinValue(LCD_D4_PORT,LCD_D4_PIN,DIO_PIN_LOW); }
	else { DIO_SetPinValue(LCD_D4_PORT,LCD_D4_PIN,DIO_PIN_HIGH); }
	if (0== GET_BIT(Value,1 ) ) { DIO_SetPinValue(LCD_D5_PORT,LCD_D5_PIN,DIO_PIN_LOW); }
	else { DIO_SetPinValue(LCD_D5_PORT,LCD_D5_PIN,DIO_PIN_HIGH); }
	if (0== GET_BIT(Value,2 ) ) { DIO_SetPinValue(LCD_D6_PORT,LCD_D6_PIN,DIO_PIN_LOW); }
	else { DIO_SetPinValue(LCD_D6_PORT,LCD_D6_PIN,DIO_PIN_HIGH); }
	if (0== GET_BIT(Value,3 ) ) { DIO_SetPinValue(LCD_D7_PORT,LCD_D7_PIN,DIO_PIN_LOW); }
	else { DIO_SetPinValue(LCD_D7_PORT,LCD_D7_PIN,DIO_PIN_HIGH); }
}
