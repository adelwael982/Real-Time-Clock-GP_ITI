/*
 * RTC_interface.h
 *
 *  Created on: Sep 10, 2026
 *      Author: DELL
 */

#ifndef HAL_RTC_RTC_INTERFACE_H_
#define HAL_RTC_RTC_INTERFACE_H_

typedef struct
{
	u8 seconds;
	u8 minutes;
	u8 hours;
	u8 day;
	u8 date;
	u8 month;
	u8 year;
} RTC_Time_t;

void RTC_Init(void);
u8 RTC_GetTime(RTC_Time_t *ptime);
u8 RTC_SetTime(RTC_Time_t *ptime);

#endif /* HAL_RTC_RTC_INTERFACE_H_ */
