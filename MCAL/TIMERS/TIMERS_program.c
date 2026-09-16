#include "../../STD_TYPES.h"
#include "../../BIT_MATH.h"
#include "TIMERS_private.h"
#include "TIMERS_interface.h"

void TIMERS_voidTimer1Init (void){
	SET_BIT(TIMERS_TCCR1A_REG,1); CLR_BIT(TIMERS_TCCR1A_REG,0);
	SET_BIT(TIMERS_TCCR1B_REG,4); SET_BIT(TIMERS_TCCR1B_REG,3);
	SET_BIT(TIMERS_TCCR1A_REG,7); CLR_BIT(TIMERS_TCCR1A_REG,6);
	TIMERS_ICR1AH_REG=(u8)(19999>>8); TIMERS_ICR1AL_REG=(u8)(19999&0x00FF);
	CLR_BIT(TIMERS_TCCR1B_REG,0); SET_BIT(TIMERS_TCCR1B_REG,1); CLR_BIT(TIMERS_TCCR1B_REG,2);
}

void TIMERS_voidTimer1SetCompareMatchValue(u16 Copy_OCR1Value)
{
	TIMERS_OCR1AH_REG=(u8)(Copy_OCR1Value>>8);
	TIMERS_OCR1AL_REG=(u8)(Copy_OCR1Value&0x00FF);
}

static void(*TIMERS_pfTimer0OVF)(void)=NULL;
static void(*TIMERS_pfTimer0CTC)(void)=NULL;

void TIMERS_voidTimer0SetCompareMatchValue(u8 Copy_OCR0Value){ TIMERS_OCR0_REG=Copy_OCR0Value; }

void TIMERS_voidTimer0Init (void)
{
	/* Select CTC Mode: WGM01=1, WGM00=0 */
	SET_BIT(TIMERS_TCCR0_REG,3); CLR_BIT(TIMERS_TCCR0_REG,6);
	/* OC0 disconnected - normal port operation */
	CLR_BIT(TIMERS_TCCR0_REG,5); CLR_BIT(TIMERS_TCCR0_REG,4);
	/* Enable Timer0 Compare Match interrupt */
	SET_BIT(TIMERS_TIMSK_REG,1);
	/* CLK / 64 */
	SET_BIT(TIMERS_TCCR0_REG,0); SET_BIT(TIMERS_TCCR0_REG,1); CLR_BIT(TIMERS_TCCR0_REG,2);
}

u8 TIMERS_u8Timer0OVFSetCallBack(void(*copy_pf)(void))
{
	u8 Local_u8ErrorState=STD_TYPES_OK;
	if(copy_pf!=NULL) TIMERS_pfTimer0OVF=copy_pf;
	else Local_u8ErrorState=STD_TYPES_NOK;
	return Local_u8ErrorState;
}

u8 TIMERS_u8Timer0CTCSetCallBack(void(*copy_pf)(void))
{
	u8 Local_u8ErrorState=STD_TYPES_OK;
	if(copy_pf!=NULL) TIMERS_pfTimer0CTC=copy_pf;
	else Local_u8ErrorState=STD_TYPES_NOK;
	return Local_u8ErrorState;
}

void __vector_11(void) __attribute__((signal));
void __vector_11(void)
{
	static u16 Local_u16CounterOVF=0;
	Local_u16CounterOVF++;
	if(Local_u16CounterOVF==3907)
	{
		TIMERS_TCNT0_REG=192; Local_u16CounterOVF=0;
		if(TIMERS_pfTimer0OVF!=NULL) TIMERS_pfTimer0OVF();
	}
}

void __vector_10(void) __attribute__((signal));
void __vector_10(void)
{
	if(TIMERS_pfTimer0CTC!=NULL) TIMERS_pfTimer0CTC();
}
