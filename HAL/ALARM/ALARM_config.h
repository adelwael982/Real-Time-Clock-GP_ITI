/*
 * ALARM_config.h
 *
 *  Alarm feature configuration for Project Six (RTC).
 */

#ifndef HAL_ALARM_ALARM_CONFIG_H_
#define HAL_ALARM_ALARM_CONFIG_H_


/* Buzzer is simulated with D1 (LED-RED) on PD4 in the Proteus schematic */
#define ALARM_BUZZER_PORT		DIO_PORTC
#define ALARM_BUZZER_PIN		DIO_PIN4

/* Which pin level actually lights D1. If the LED's cathode is wired
 * toward PD4 (sink/active-low), it lights on LOW, not HIGH - if the LED
 * doesn't light when the alarm rings, swap these two lines first. */
#define ALARM_BUZZER_ON_LEVEL	DIO_PIN_HIGH
#define ALARM_BUZZER_OFF_LEVEL	DIO_PIN_LOW


/*
 * Raw codes returned by KEYPAD_GetPressed(), mapped to logical keys.
 *
 * These were derived from:
 *   1) the schematic wiring (keypad rows A-D on B0-B3, columns 1-4 on B4-B7)
 *   2) KEYPAD_config.h (COLUMN_START = pin0 => drives B0-B3 = rows A-D,
 *      ROW_START = pin4 => reads B4-B7 = columns 1-4)
 *   3) KEYPAD_program.c's KPD_Keys[][] lookup table (KPD_Keys[row][col],
 *      where the driver's "row" loop variable ends up being the physical
 *      COLUMN index, and its "col" variable ends up being the
 *      physical ROW index - the two are swapped relative to the names)
 *
 * Physical key layout (from the schematic):
 *      7  8  9  /   (SILENCE)
 *      4  5  6  x   (START)
 *      1  2  3  -   (STOP)
 *      < 0 >   +   (LEFT, RIGHT, SELECT)
 *
 * DO NOT reorder these to "look" sequential - they are intentionally
 * non-sequential because of the swap described above. If the keypad
 * wiring in Proteus ever changes, this table has to be re-derived.
 */
#define ALARM_KEY_7				1
#define ALARM_KEY_4				2
#define ALARM_KEY_1				3
#define ALARM_KEY_LEFT			4
#define ALARM_KEY_8				5
#define ALARM_KEY_5				6
#define ALARM_KEY_2				7
#define ALARM_KEY_0				8
#define ALARM_KEY_9				9
#define ALARM_KEY_6				10
#define ALARM_KEY_3				11
#define ALARM_KEY_RIGHT			12
#define ALARM_KEY_SILENCE		13
#define ALARM_KEY_START			14
#define ALARM_KEY_STOP			15
#define ALARM_KEY_SELECT		16


#endif /* HAL_ALARM_ALARM_CONFIG_H_ */
