#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "MCAL/DIO/DIO_interface.h"
#include "MCAL/TIMERS/TIMERS_interface.h"
#include "MCAL/GLOBAL_INTERRUPT/GI_interface.h"
#include "RTC/RTC_interface.h"
#include "HAL/LCD/LCD_interface.h"
#include "HAL/KEYPAD/KEYPAD_interface.h"
#include "HAL/ALARM/ALARM_interface.h"
#include "HAL/ALARM/ALARM_config.h"

#define BUZZER_PORT DIO_PORTC
#define BUZZER_PIN DIO_PIN4
#define KEY_1 ALARM_KEY_1
#define KEY_2 ALARM_KEY_2
#define KEY_3 ALARM_KEY_3
#define KEY_4 ALARM_KEY_4
#define KEY_5 ALARM_KEY_5
#define KEY_6 ALARM_KEY_6
#define KEY_7 ALARM_KEY_7
#define KEY_8 ALARM_KEY_8
#define KEY_9 ALARM_KEY_9
#define KEY_0 ALARM_KEY_0
#define KEY_LEFT ALARM_KEY_LEFT
#define KEY_RIGHT ALARM_KEY_RIGHT
#define KEY_SELECT ALARM_KEY_SELECT
#define KEY_START ALARM_KEY_START
#define KEY_STOP ALARM_KEY_STOP
#define KEY_SILENCE ALARM_KEY_SILENCE

typedef enum { MODE_CLOCK, MODE_ALARM, MODE_STOPWATCH, MODE_COUNTDOWN, MODE_SET_TIME, MODE_COUNT } Mode_t;
typedef enum { RUN_IDLE, RUN_RUNNING, RUN_PAUSED } RunState_t;
typedef enum { F_HOUR, F_MIN, F_SEC, F_DAY, F_MONTH, F_YEAR } Field_t;
volatile Mode_t g_mode=MODE_CLOCK;
Mode_t g_lastMode=MODE_COUNT;
RunState_t g_runState=RUN_IDLE, g_lastRunState=RUN_IDLE;
volatile u8 hour=0,min=0,sec=0,g_isRunning=0,g_countdownDone=0;
u8 entryInput=0; Field_t entryField=F_HOUR,g_lastEntryField=F_HOUR;
u8 setHour=0,setMin=0,setSec=0,setDay=1,setMonth=1,setYear=0;

void Timer_Tick(void)
{
    static u16 tickCount=0;
    if(!g_isRunning) return;
    tickCount++;
    if(tickCount<500) return;
    tickCount=0;
    if(g_mode==MODE_STOPWATCH){ sec++; if(sec>=60){sec=0;min++; if(min>=60){min=0;hour++;}} }
    else if(g_mode==MODE_COUNTDOWN)
    {
        if(sec>0) sec--;
        else if(min>0){min--;sec=59;}
        else if(hour>0){hour--;min=59;sec=59;}
        else {g_isRunning=0;g_countdownDone=1;}
    }
}

u8 KeyToDigit(u8 key){switch(key){case KEY_0:return 0;case KEY_1:return 1;case KEY_2:return 2;case KEY_3:return 3;case KEY_4:return 4;case KEY_5:return 5;case KEY_6:return 6;case KEY_7:return 7;case KEY_8:return 8;case KEY_9:return 9;default:return 0xFF;}}
u8 FieldMax(Field_t f){switch(f){case F_HOUR:return 23;case F_MIN:return 59;case F_SEC:return 59;case F_DAY:return 31;case F_MONTH:return 12;default:return 99;}}
void PrintTwoDigits(u8 value){LCD_sendChar((u8)('0'+((value/10)%10)));LCD_sendChar((u8)('0'+(value%10)));}
void PrintField(u8 storedValue,u8 isActive,u8 typedValue){if(isActive){LCD_writeString((u8*)"[");LCD_writeNumber(typedValue);LCD_writeString((u8*)"]");}else PrintTwoDigits(storedValue);}
void DisplayLabel(const u8 *name){LCD_goToSpecificPosition(LCD_LINE_TWO,0);LCD_writeString((u8*)name);}
void DisplayHMS(u8 h,u8 m,u8 s){LCD_goToSpecificPosition(LCD_LINE_ONE,0);PrintTwoDigits(h);LCD_sendChar(':');PrintTwoDigits(m);LCD_sendChar(':');PrintTwoDigits(s);}
void HandleDigitEntry(u8 key,u8 *out,Field_t nextField){u8 digit=KeyToDigit(key);if(digit!=0xFF){entryInput=(entryInput*10)+digit;if(entryInput>FieldMax(entryField))entryInput=0;}else if(key==KEY_SELECT){*out=entryInput;entryInput=0;entryField=nextField;}}

int main(void)
{
    LCD_init(); RTC_Init(); KEYPAD_Init(); ALARM_Init();
    DIO_SetPinDirection(BUZZER_PORT,BUZZER_PIN,DIO_PIN_OUTPUT);
    DIO_SetPinValue(BUZZER_PORT,BUZZER_PIN,DIO_PIN_LOW);
    TIMERS_u8Timer0CTCSetCallBack(Timer_Tick); TIMERS_voidTimer0SetCompareMatchValue(249); TIMERS_voidTimer0Init(); GI_Enable();
    while(1)
    {
        u8 key=KEYPAD_GetPressed(); RTC_Time_t currentTime; u8 haveTime=(RTC_GetTime(&currentTime)==STD_TYPES_OK);
        if(haveTime) ALARM_Task(&currentTime);
        if(g_mode!=g_lastMode){LCD_clear();if(g_mode==MODE_SET_TIME||g_mode==MODE_COUNTDOWN){entryField=F_HOUR;entryInput=0;}g_lastMode=g_mode;}
        if(entryField!=g_lastEntryField){LCD_clear();g_lastEntryField=entryField;}
        if(g_runState!=g_lastRunState){LCD_clear();g_lastRunState=g_runState;}
        if(key==KEY_LEFT&&g_runState==RUN_IDLE) g_mode=(g_mode+MODE_COUNT-1)%MODE_COUNT;
        else if(key==KEY_RIGHT&&g_runState==RUN_IDLE) g_mode=(g_mode+1)%MODE_COUNT;
        else if(key==KEY_START&&(g_mode==MODE_STOPWATCH||g_mode==MODE_COUNTDOWN)&&g_runState!=RUN_RUNNING){g_isRunning=1;g_runState=RUN_RUNNING;}
        else if(key==KEY_STOP&&(g_mode==MODE_STOPWATCH||g_mode==MODE_COUNTDOWN))
        {if(g_runState==RUN_RUNNING){g_isRunning=0;g_runState=RUN_PAUSED;}else if(g_runState==RUN_PAUSED){hour=0;min=0;sec=0;g_runState=RUN_IDLE;}}
        else if(key==KEY_SILENCE&&(g_countdownDone||ALARM_IsRinging()))
        {if(g_countdownDone){DIO_SetPinValue(BUZZER_PORT,BUZZER_PIN,DIO_PIN_LOW);g_countdownDone=0;g_runState=RUN_IDLE;}if(ALARM_IsRinging())ALARM_Silence();}
        else if(key==KEY_SELECT&&g_mode==MODE_ALARM){ALARM_RunMenu();g_lastMode=MODE_COUNT;}
        else if(g_mode==MODE_SET_TIME)
        {
            switch(entryField)
            {case F_HOUR:HandleDigitEntry(key,&setHour,F_MIN);break;case F_MIN:HandleDigitEntry(key,&setMin,F_SEC);break;case F_SEC:HandleDigitEntry(key,&setSec,F_DAY);break;case F_DAY:HandleDigitEntry(key,&setDay,F_MONTH);break;case F_MONTH:HandleDigitEntry(key,&setMonth,F_YEAR);break;case F_YEAR:{Field_t before=entryField;HandleDigitEntry(key,&setYear,F_HOUR);if(before!=F_HOUR&&entryField==F_HOUR){RTC_Time_t t;t.hours=setHour;t.minutes=setMin;t.seconds=setSec;t.date=setDay;t.day=1;t.month=setMonth;t.year=setYear;RTC_SetTime(&t);g_mode=MODE_CLOCK;}break;}}
        }
        else if(g_mode==MODE_COUNTDOWN&&g_runState==RUN_IDLE)
        {switch(entryField){case F_HOUR:HandleDigitEntry(key,(u8*)&hour,F_MIN);break;case F_MIN:HandleDigitEntry(key,(u8*)&min,F_SEC);break;case F_SEC:HandleDigitEntry(key,(u8*)&sec,F_HOUR);break;default:entryField=F_HOUR;break;}}
        if(g_countdownDone) DIO_SetPinValue(BUZZER_PORT,BUZZER_PIN,DIO_PIN_HIGH);
        switch(g_mode)
        {
            case MODE_CLOCK:
                DisplayLabel((u8*)"CLOCK ");
                if(haveTime){DisplayHMS(currentTime.hours,currentTime.minutes,currentTime.seconds);LCD_goToSpecificPosition(LCD_LINE_TWO,6);PrintTwoDigits(currentTime.date);LCD_sendChar('/');PrintTwoDigits(currentTime.month);LCD_sendChar('/');PrintTwoDigits(currentTime.year);}break;
            case MODE_ALARM:
            {u8 aH,aM;DisplayLabel((u8*)"ALARM   SEL=menu");ALARM_GetTime(&aH,&aM);LCD_goToSpecificPosition(LCD_LINE_ONE,0);if(ALARM_IsEnabled())LCD_writeString((u8*)"ON");else LCD_writeString((u8*)"OFF");PrintTwoDigits(aH);LCD_sendChar(':');PrintTwoDigits(aM);LCD_writeString(ALARM_IsRinging()?(u8*)" RING":(u8*)"     ");break;}
            case MODE_STOPWATCH:DisplayLabel((u8*)"STOPWATCH");DisplayHMS(hour,min,sec);break;
            case MODE_COUNTDOWN:
                DisplayLabel((u8*)"COUNTDOWN");
                if(g_runState==RUN_IDLE){LCD_goToSpecificPosition(LCD_LINE_ONE,0);PrintField(hour,entryField==F_HOUR,entryInput);LCD_writeString((u8*)":");PrintField(min,entryField==F_MIN,entryInput);LCD_writeString((u8*)":");PrintField(sec,entryField==F_SEC,entryInput);}else DisplayHMS(hour,min,sec);break;
            case MODE_SET_TIME:
                LCD_goToSpecificPosition(LCD_LINE_ONE,0);PrintField(setHour,entryField==F_HOUR,entryInput);LCD_writeString((u8*)":");PrintField(setMin,entryField==F_MIN,entryInput);LCD_writeString((u8*)":");PrintField(setSec,entryField==F_SEC,entryInput);
                LCD_goToSpecificPosition(LCD_LINE_TWO,0);PrintField(setDay,entryField==F_DAY,entryInput);LCD_writeString((u8*)"/");PrintField(setMonth,entryField==F_MONTH,entryInput);LCD_writeString((u8*)"/");PrintField(setYear,entryField==F_YEAR,entryInput);break;
            default:break;
        }
    }
}
