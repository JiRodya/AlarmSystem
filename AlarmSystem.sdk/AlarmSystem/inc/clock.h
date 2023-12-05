#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include "PmodRTCC.h"

//typedef enum {MAIN, WARNING} alarmType;
#define ALARM_CONFIG (RTCC_ALM_POL | RTCC_ALMC2 | RTCC_ALMC1 | RTCC_ALMC0)

typedef enum Target {
   CLOCK = 0, 	// Real-time clock
   MAIN_ALARM,     // Alarm 0
   WARNING_ALARM,     // Alarm 1
   CLOCK_PWRD,     // Power-down time-stamp
   CLOCK_PWRU      // Power-up time-stamp
} Target;

extern const char *weekdays[7];

typedef struct {
	u8 second;
	u8 minute;
	u8 hour;
	u8 ampm;
	u8 day;
	u8 date;
	u8 month;
	u8 year;
} Time;

// If RTCC is already set, change 1 to 0
#define SET_CLOCK 1

PmodRTCC clock;

//core functiosn
void Clock_Init(u8 mode);

//clock functions
Time Clock_GetTime(Target src);
void Clock_SetTime(Time val, Target t);
void Clock_SetAlarm(Time time);
u8  Clock_CheckAlarm();
void Clock_ResetAlarm (Target alarm);
//void PrintTime(RTCC_Target src);
Time Clock_IncrementTime(Time time, int delta_seconds);

#endif
