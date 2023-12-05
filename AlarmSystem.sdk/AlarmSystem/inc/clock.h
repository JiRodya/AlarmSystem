#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include "PmodRTCC.h"

typedef enum {MAIN, WARNING} alarmType;

typedef enum{
	CLOCK_ALARM_1 = 0,
	CLOCK_ALARM_2,
	CLOCK_SYSTEM
}Target;
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

static volatile int alarmCount = 0;
static volatile int alarmCounter = 10;
static uint8_t AFlag0 =0;
static uint8_t AFlag1 =1;

static uint8_t activeAlarm =0;

PmodRTCC clock;
void Clock_Init();
Time Clock_GetTime( RTCC_Target src);
Time Clock_IncrementTime(Time time, int delta_seconds);
void Clock_SetTime(Time val, Target t);
void Clock_SetAlarm(Time alarmTime);
u8 bcd2int(u8 bcdValue);
u8 int2bcd(u8 intValue);
u8 Clock_CheckAlarm(uint8_t alarm_type);

#endif
