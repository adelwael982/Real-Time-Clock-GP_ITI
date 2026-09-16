/*
 * ALARM_interface.h
 *
 *  Alarm feature for Project Six (RTC).
 *
 *  NOTE: expects RTC_interface.h to live at HAL/RTC/RTC_interface.h and
 *  I2C_interface.h at MCAL/I2C/I2C_interface.h once your teammate's files
 *  are dropped into the shared project (they currently sit loose in F1's
 *  own folder) - update the include path below if you place them elsewhere.
 */

#ifndef HAL_ALARM_ALARM_INTERFACE_H_
#define HAL_ALARM_ALARM_INTERFACE_H_

#include "../../STD_TYPES.h"
#include "../../RTC/RTC_interface.h"


/* Sets up the buzzer pin and clears all alarm state. Call once at startup. */
void ALARM_Init(void);

/* Stores the alarm time. Returns STD_TYPES_NOK if Copy_u8Hours > 23 or
 * Copy_u8Minutes > 59 (nothing is changed in that case). */
u8   ALARM_SetTime(u8 Copy_u8Hours, u8 Copy_u8Minutes);

void ALARM_Enable(void);
void ALARM_Disable(void);   /* also silences the buzzer if it was ringing */
u8   ALARM_IsEnabled(void);
u8   ALARM_IsRinging(void);

/* Non-blocking getter so other screens (e.g. a live status line while
 * you're not inside ALARM_RunMenu) can read the stored alarm time. */
void ALARM_GetTime(u8 *Copy_pu8Hours, u8 *Copy_pu8Minutes);

/* Call every pass through the main loop, passing the latest RTC reading.
 * Compares hours/minutes and turns the buzzer on when they match. Only
 * rings once per matching minute (re-arms automatically once the clock
 * moves past that minute, so it will ring again the same time next day). */
void ALARM_Task(RTC_Time_t *Copy_psCurrentTime);

/* Bind this to the SILENCE key. Also used by the countdown feature to
 * silence its own buzzer use - safe to call even if the alarm isn't
 * the one ringing. */
void ALARM_Silence(void);

/* Blocking UI: "Set Alarm HH:MM" screen.
 *   - number keys type the digit at the cursor and auto-advance
 *   - LEFT/RIGHT move the cursor back to fix a digit
 *   - SELECT validates and saves; on success returns STD_TYPES_OK
 *   - STOP cancels and returns STD_TYPES_NOK without changing the
 *     stored alarm time
 */
u8   ALARM_RunSetTimeUI(void);

/* Blocking-until-key UI: shows "Alarm: ON/OFF" and "HH:MM" (+ "RINGING"
 * if applicable). Returns on any key press so the caller's menu loop
 * can decide what to do next. */
void ALARM_ShowStatusUI(void);

/* "1. Set alarm  2. View status" sub-menu, entered once the user has
 * selected Alarm mode from the top-level mode menu. Navigate with
 * LEFT/RIGHT, choose with SELECT. Returns when the user presses STOP
 * to back out, so the caller can return to the top-level mode menu. */
void ALARM_RunMenu(void);


#endif /* HAL_ALARM_ALARM_INTERFACE_H_ */
