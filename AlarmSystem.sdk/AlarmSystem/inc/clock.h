#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include "PmodRTCC.h"

typedef struct {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t ampm;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint16_t year;
} Time;


PmodRTCC clock;
void Clock_Init();
Time Clock_GetTime( RTCC_Target src);
Time Clock_IncrementTime(Time time, int delta_seconds);
void Clock_SetTime( RTCC_Target dest, Time val);
void Clock_SetAlarm();
u8 bcd2int(u8 bcdValue);
u8 int2bcd(u8 intValue);

#endif
