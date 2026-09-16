#ifndef RTC_INTERFACE_H_
#define RTC_INTERFACE_H_

void RTC_init(void);
void RTC_setTime(u8 hours, u8 minutes, u8 seconds);
void RTC_getTime(u8* hours, u8* minutes, u8* seconds);
void RTC_setDate(u8 day, u8 month, u8 year);
void RTC_getDate(u8* day, u8* month, u8* year);

#endif