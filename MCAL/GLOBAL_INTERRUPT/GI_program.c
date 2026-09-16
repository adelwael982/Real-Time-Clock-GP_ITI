/*

* GI_program.c
 *
 *  Created on: Sep 2, 2026
 *      Author: mosta
 */

#include "../../STD_TYPES.h"
#include "../../BIT_MATH.h"
#include "GI_interface.h"
#include "GI_private.h"

void GI_Enable(void)
{
	SET_BIT (SREG,SREG_I );
}

void GI_Disable(void)
{
	CLR_BIT(SREG,SREG_I );
}
