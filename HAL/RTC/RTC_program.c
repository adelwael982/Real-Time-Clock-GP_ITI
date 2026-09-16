#include "../STD_TYPES.h"
#include "../BIT_MATH.h"
#include "../MCAL/I2C/I2C_interface.h"
#include "RTC_interface.h"

#define DS1307_ADDR_WRITE   0xD0
#define DS1307_ADDR_READ    0xD1

void RTC_Init(void)
{
    I2C_Init();
}

u8 RTC_GetTime(RTC_Time_t *pTime)
{
    u8 i;
    u8 bcd;
    u8 *pTimeBytes = (u8*)pTime;

    if (pTime == NULL)
        return STD_TYPES_NOK;

    /* Phase 1: point to register 0x00 (Seconds) */
    I2C_Start();
    if (I2C_GetStatus() != 0x08) { I2C_Stop(); return STD_TYPES_NOK; }

    I2C_WriteByte(DS1307_ADDR_WRITE);
    if (I2C_GetStatus() != 0x18) { I2C_Stop(); return STD_TYPES_NOK; }

    I2C_WriteByte(0x00);
    if (I2C_GetStatus() != 0x28) { I2C_Stop(); return STD_TYPES_NOK; }

    /* Phase 2: repeated start, switch to read */
    I2C_RepeatedStart();
    if (I2C_GetStatus() != 0x10) { I2C_Stop(); return STD_TYPES_NOK; }

    I2C_WriteByte(DS1307_ADDR_READ);
    if (I2C_GetStatus() != 0x40) { I2C_Stop(); return STD_TYPES_NOK; }

    for (i = 0; i < 6; i++)
    {
        bcd = I2C_ReadByteACK();
        pTimeBytes[i] = ((bcd >> 4) * 10) + (bcd & 0x0F);
    }

    /* last byte so NACK */
    bcd = I2C_ReadByteNACK();
    pTimeBytes[i] = ((bcd >> 4) * 10) + (bcd & 0x0F);

    I2C_Stop();

    return STD_TYPES_OK;
}

u8 RTC_SetTime(RTC_Time_t *pTime)
{
    u8 i;
    u8 dec;
    u8 bcd;
    u8 *pTimeBytes = (u8*)pTime;

    if (pTime == NULL)
        return STD_TYPES_NOK;

    if (pTime->hours > 23 || pTime->minutes > 59 || pTime->seconds > 59)
        return STD_TYPES_NOK;

    /* Phase 1: pointer to register 0x00 (Seconds) */
    I2C_Start();
    if (I2C_GetStatus() != 0x08) { I2C_Stop(); return STD_TYPES_NOK; }

    I2C_WriteByte(DS1307_ADDR_WRITE);
    if (I2C_GetStatus() != 0x18) { I2C_Stop(); return STD_TYPES_NOK; }

    I2C_WriteByte(0x00);
    if (I2C_GetStatus() != 0x28) { I2C_Stop(); return STD_TYPES_NOK; }

    /* Phase 2: write all 7 data bytes */
    for (i = 0; i < 7; i++)
    {
        dec = pTimeBytes[i];
        bcd = ((dec / 10) << 4) | (dec % 10);

        I2C_WriteByte(bcd);
        if (I2C_GetStatus() != 0x28) { I2C_Stop(); return STD_TYPES_NOK; }
    }

    I2C_Stop();

    return STD_TYPES_OK;
}
