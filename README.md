# Real-Time Clock Project

An embedded-systems real-time clock project developed during ITI Embedded Systems training.

## Project Structure

- `main.c` — Main application with clock, alarm, stopwatch, countdown, and time-setting modes.
- `HAL/ALARM/` — Alarm feature.
- `HAL/KEYPAD/` — Keypad driver.
- `HAL/LCD/` — LCD driver.
- `MCAL/DIO/` — Digital I/O driver.
- `MCAL/I2C/` — I2C/TWI driver used to communicate with the RTC.
- `MCAL/GLOBAL_INTERRUPT/` — Global interrupt control.
- `MCAL/TIMERS/` — Timer drivers and Timer0 callback handling.
- `RTC/` — RTC interface and implementation.
- `BIT_MATH.h` and `STD_TYPES.h` — Common project headers.

## Team

- Yousef Mahmoud Elsayed
- Adel Wael Adel
- Abdullah Mohammed Abdullah
