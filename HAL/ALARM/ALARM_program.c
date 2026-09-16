#include "../../STD_TYPES.h"
#include "../../BIT_MATH.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "../KEYPAD/KEYPAD_interface.h"
#include "../LCD/LCD_interface.h"
#include "../../RTC/RTC_interface.h"
#include "ALARM_config.h"
#include "ALARM_interface.h"

static u8 ALARM_u8Hours=0, ALARM_u8Minutes=0, ALARM_u8Enabled=0, ALARM_u8Ringing=0, ALARM_u8AlreadyRung=0;

static u8 ALARM_u8WaitForKey(void)
{
    u8 Local_u8Key;
    do { Local_u8Key = KEYPAD_GetPressed(); } while (Local_u8Key == 0 || Local_u8Key == 0xFF || Local_u8Key == 255);
    while (KEYPAD_GetPressed() == Local_u8Key);
    return Local_u8Key;
}

static void ALARM_vPrintTwoDigits(u8 Copy_u8Value)
{
    LCD_sendChar('0' + (Copy_u8Value / 10));
    LCD_sendChar('0' + (Copy_u8Value % 10));
}

static u8 ALARM_u8KeyToDigit(u8 Copy_u8Key, u8 *Copy_pu8Digit)
{
    switch (Copy_u8Key)
    {
        case ALARM_KEY_0: *Copy_pu8Digit=0; return STD_TYPES_OK;
        case ALARM_KEY_1: *Copy_pu8Digit=1; return STD_TYPES_OK;
        case ALARM_KEY_2: *Copy_pu8Digit=2; return STD_TYPES_OK;
        case ALARM_KEY_3: *Copy_pu8Digit=3; return STD_TYPES_OK;
        case ALARM_KEY_4: *Copy_pu8Digit=4; return STD_TYPES_OK;
        case ALARM_KEY_5: *Copy_pu8Digit=5; return STD_TYPES_OK;
        case ALARM_KEY_6: *Copy_pu8Digit=6; return STD_TYPES_OK;
        case ALARM_KEY_7: *Copy_pu8Digit=7; return STD_TYPES_OK;
        case ALARM_KEY_8: *Copy_pu8Digit=8; return STD_TYPES_OK;
        case ALARM_KEY_9: *Copy_pu8Digit=9; return STD_TYPES_OK;
        default: return STD_TYPES_NOK;
    }
}

void ALARM_Init(void)
{
    DIO_SetPinDirection(ALARM_BUZZER_PORT, ALARM_BUZZER_PIN, DIO_PIN_OUTPUT);
    DIO_SetPinValue(ALARM_BUZZER_PORT, ALARM_BUZZER_PIN, ALARM_BUZZER_OFF_LEVEL);
    ALARM_u8Hours=0; ALARM_u8Minutes=0; ALARM_u8Enabled=0; ALARM_u8Ringing=0; ALARM_u8AlreadyRung=0;
}

u8 ALARM_SetTime(u8 Copy_u8Hours, u8 Copy_u8Minutes)
{
    if (Copy_u8Hours > 23 || Copy_u8Minutes > 59) return STD_TYPES_NOK;
    ALARM_u8Hours=Copy_u8Hours; ALARM_u8Minutes=Copy_u8Minutes; ALARM_u8AlreadyRung=0;
    return STD_TYPES_OK;
}

void ALARM_Enable(void) { ALARM_u8Enabled=1; }
u8 ALARM_IsEnabled(void) { return ALARM_u8Enabled; }
u8 ALARM_IsRinging(void) { return ALARM_u8Ringing; }

void ALARM_GetTime(u8 *Copy_pu8Hours, u8 *Copy_pu8Minutes)
{
    if (Copy_pu8Hours != NULL) *Copy_pu8Hours=ALARM_u8Hours;
    if (Copy_pu8Minutes != NULL) *Copy_pu8Minutes=ALARM_u8Minutes;
}

void ALARM_Disable(void) { ALARM_u8Enabled=0; if (ALARM_u8Ringing) ALARM_Silence(); }
void ALARM_Silence(void) { DIO_SetPinValue(ALARM_BUZZER_PORT, ALARM_BUZZER_PIN, ALARM_BUZZER_OFF_LEVEL); ALARM_u8Ringing=0; }

void ALARM_Task(RTC_Time_t *Copy_psCurrentTime)
{
    if (Copy_psCurrentTime == NULL || ALARM_u8Enabled == 0) return;
    if ((Copy_psCurrentTime->hours == ALARM_u8Hours) && (Copy_psCurrentTime->minutes == ALARM_u8Minutes))
    {
        if (ALARM_u8AlreadyRung == 0)
        {
            DIO_SetPinValue(ALARM_BUZZER_PORT, ALARM_BUZZER_PIN, ALARM_BUZZER_ON_LEVEL);
            ALARM_u8Ringing=1; ALARM_u8AlreadyRung=1;
        }
    }
    else ALARM_u8AlreadyRung=0;
}

u8 ALARM_RunSetTimeUI(void)
{
    u8 Local_au8Digits[4]={0,0,0,0}, Local_u8Cursor=0, Local_u8Error=0;
    u8 Local_u8Key, Local_u8Val, Local_u8H, Local_u8M;
    for (;;)
    {
        LCD_clear(); LCD_goToSpecificPosition(LCD_LINE_ONE,0);
        LCD_writeString(Local_u8Error ? (u8*)"Invalid! 0-23/0-59" : (u8*)"Set Alarm HH:MM");
        LCD_goToSpecificPosition(LCD_LINE_TWO,0);
        LCD_sendChar('0'+Local_au8Digits[0]); LCD_sendChar('0'+Local_au8Digits[1]); LCD_sendChar(':');
        LCD_sendChar('0'+Local_au8Digits[2]); LCD_sendChar('0'+Local_au8Digits[3]);
        LCD_goToSpecificPosition(LCD_LINE_TWO,Local_u8Cursor+(Local_u8Cursor>=2));
        Local_u8Key=ALARM_u8WaitForKey(); Local_u8Error=0;
        if (ALARM_u8KeyToDigit(Local_u8Key,&Local_u8Val)==STD_TYPES_OK)
        { Local_au8Digits[Local_u8Cursor]=Local_u8Val; if(Local_u8Cursor<3) Local_u8Cursor++; }
        else if(Local_u8Key==ALARM_KEY_LEFT && Local_u8Cursor>0) Local_u8Cursor--;
        else if(Local_u8Key==ALARM_KEY_RIGHT && Local_u8Cursor<3) Local_u8Cursor++;
        else if(Local_u8Key==ALARM_KEY_STOP) return STD_TYPES_NOK;
        else if(Local_u8Key==ALARM_KEY_SELECT)
        {
            Local_u8H=(Local_au8Digits[0]*10)+Local_au8Digits[1]; Local_u8M=(Local_au8Digits[2]*10)+Local_au8Digits[3];
            if(ALARM_SetTime(Local_u8H,Local_u8M)==STD_TYPES_OK)
            { ALARM_Enable(); LCD_clear(); LCD_goToSpecificPosition(LCD_LINE_ONE,0); LCD_writeString((u8*)"Alarm Set!"); return STD_TYPES_OK; }
            Local_u8Error=1;
        }
    }
}

void ALARM_ShowStatusUI(void)
{
    LCD_clear(); LCD_goToSpecificPosition(LCD_LINE_ONE,0); LCD_writeString((u8*)"Alarm: "); LCD_writeString(ALARM_u8Enabled?(u8*)"ON":(u8*)"OFF");
    LCD_goToSpecificPosition(LCD_LINE_TWO,0); ALARM_vPrintTwoDigits(ALARM_u8Hours); LCD_sendChar(':'); ALARM_vPrintTwoDigits(ALARM_u8Minutes);
    if(ALARM_u8Ringing) LCD_writeString((u8*)" RINGING");
    ALARM_u8WaitForKey();
}

void ALARM_RunMenu(void)
{
    u8 Local_u8Sel=0, Local_u8Key;
    for(;;)
    {
        LCD_clear(); LCD_goToSpecificPosition(LCD_LINE_ONE,0); LCD_writeString((u8*)"Alarm Menu"); LCD_goToSpecificPosition(LCD_LINE_TWO,0);
        if(Local_u8Sel==0) LCD_writeString((u8*)">Set  View  ");
        else if(Local_u8Sel==1) LCD_writeString((u8*)" Set >View  ");
        else LCD_writeString((u8*)" Set  View >");
        LCD_writeString(ALARM_IsEnabled()?(u8*)"Off":(u8*)"On");
        Local_u8Key=ALARM_u8WaitForKey();
        if(Local_u8Key==ALARM_KEY_RIGHT) Local_u8Sel=(Local_u8Sel+1)%3;
        else if(Local_u8Key==ALARM_KEY_LEFT) Local_u8Sel=(Local_u8Sel+2)%3;
        else if(Local_u8Key==ALARM_KEY_SELECT)
        {
            if(Local_u8Sel==0) ALARM_RunSetTimeUI();
            else if(Local_u8Sel==1) ALARM_ShowStatusUI();
            else { if(ALARM_IsEnabled()) ALARM_Disable(); else ALARM_Enable(); LCD_clear(); LCD_goToSpecificPosition(LCD_LINE_ONE,0); LCD_writeString(ALARM_IsEnabled()?(u8*)"Alarm Enabled":(u8*)"Alarm Disabled"); ALARM_u8WaitForKey(); }
        }
        else if(Local_u8Key==ALARM_KEY_STOP) return;
    }
}
