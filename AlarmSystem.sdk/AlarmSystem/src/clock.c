// clock.h
// Declaration of clock module functions


// clock.c
#include "clock.h"
#include "sleep.h"
#include "xil_cache.h"
#include "xparameters.h"
#include <stdio.h> // Include for printf

/************************** Function Declarations ***************************/

u8 bcd2int(u8 data);
u8 int2bcd(u8 data);

/************************** Global Declarations ****************************/
//PmodRTCC clock;

// Initialize the clock module
void Clock_Init() {

    RTCC_begin(&clock, XPAR_PMODRTCC_0_AXI_LITE_IIC_BASEADDR, 0x6F);

    // Start the RTCC clock
    RTCC_startClock(&clock);

    // check and enable backup battery
    if (!RTCC_checkVbat(&clock)) {
        RTCC_enableVbat(&clock);
    }

    // Clear any power-fail flags
    RTCC_clearPWRFAIL(&clock);
}



/************************** Function Definitions ****************************/

/** RTCC_Time GetTime(PmodRTCC *InstancePtr, RTCC_Target src)
**
**  Parameters:
**     InstancePtr - the target device to retrieve data from
**     src         - RTCC_TARGET_RTCC - real-time clock
**                   RTCC_TARGET_ALM0 - Alarm 0
**                   RTCC_TARGET_ALM1 - Alarm 1
**                   RTCC_TARGET_PWRD - power-down time-stamp
**                   RTCC_TARGET_PWRU - power-up time-stamp
**
**  Return Value:
**     val - the contents of all time registers in the target area
**
**  Description:
**     This function retrieves the contents of one of the Pmod RTCC's time areas
*/
Time Clock_GetTime( RTCC_Target src) {
   Time val;

   if (src != RTCC_TARGET_PWRD && src != RTCC_TARGET_PWRU) {
      val.second = RTCC_getSec(&clock, src);
   }

   val.minute = RTCC_getMin(&clock, src);
   val.hour   = RTCC_getHour(&clock, src);
   val.ampm   = RTCC_getAmPm(&clock, src);
   val.day    = RTCC_getDay(&clock, src);
   val.date   = RTCC_getDate(&clock, src);
   val.month  = RTCC_getMonth(&clock, src);

   if (src == RTCC_TARGET_RTCC) {
      val.year = RTCC_getYear(&clock);
   } else {
      val.year = 0;
   }

   return val;
}

/** void SetTime(PmodRTCC *InstancePtr, RTCC_Target src, RTCC_Time val)
**
**  Parameters:
**     InstancePtr - the target device to retrieve data from
**     src         - RTCC_TARGET_RTCC - real-time clock
**                   RTCC_TARGET_ALM0 - Alarm 0
**                   RTCC_TARGET_ALM1 - Alarm 1
**                   RTCC_TARGET_PWRD - power-down time-stamp
**                   RTCC_TARGET_PWRU - power-up time-stamp
**     val         - container for the time data to be written into the target
**                   area's registers
**
**  Return Value:
**     None
**
**  Description:
**     This function writes data into each of the registers of one of the
**     PmodRTCC's time fields
*/
void Clock_SetTime(Time val, Target t) {


   RTCC_setMin(&clock, t, val.minute);
   RTCC_setHour12(&clock, t, val.hour, val.ampm);
   RTCC_setDay(&clock, t, val.day);
   RTCC_setDate(&clock, t, val.date);
   RTCC_setMonth(&clock, t, val.month);
   xil_printf("SET: %x, %x, %x||\n",val.hour,val.minute,val.second);


}


//void Clock_SetAlarm( int hours, int minutes, int seconds) {
//    Time alarmTime;
//
//    // Set the desired alarm time
//    alarmTime.hour = hours;   // Set hours
//    alarmTime.minute = minutes; // Set minutes
//    alarmTime.second = seconds; // Set seconds
//
//    // Write the alarm time to the RTCC (using alarm 0)
//    Clock_SetTime( RTCC_TARGET_ALM0, alarmTime);
//
//    // Enable the alarm: setting an alarm to match hours, minutes, and seconds
//    RTCC_enableAlarm(&clock, RTCC_TARGET_ALM0, RTCC_ALM_POL | RTCC_ALMC2 | RTCC_ALMC1 | RTCC_ALMC0);
//}
void Clock_SetAlarm(Time alarmTime) {
	Time counterAlarm = alarmTime;

	counterAlarm.second = alarmTime.second - 10;

    // Write the alarm time to the RTCC (using alarm 0)
    Clock_SetTime(alarmTime,RTCC_TARGET_ALM0);
    Clock_SetTime(counterAlarm, RTCC_TARGET_ALM1);

    // Enable the alarm: setting an alarm to match hours, minutes, and seconds
    RTCC_enableAlarm(&clock, RTCC_TARGET_ALM0, RTCC_ALM_POL | RTCC_ALMC2 | RTCC_ALMC1 | RTCC_ALMC0);
    RTCC_enableAlarm(&clock, RTCC_TARGET_ALM1, RTCC_ALM_POL | RTCC_ALMC2 | RTCC_ALMC1 | RTCC_ALMC0);

    activeAlarm = 1;
    // Print the set alarm time for diagnostic purposes
    printf("Alarm Set: %02x:%02x:%02x\n", alarmTime.hour, alarmTime.minute, alarmTime.second);
}

u8 bcd2int(u8 bcdValue) {
    return ((bcdValue / 16) * 10) + (bcdValue % 16);
}

u8 int2bcd(u8 intValue) {
    return ((intValue / 10) * 16) + (intValue % 10);
}


u8 Clock_CheckAlarm(uint8_t alarm_type){

	if(!activeAlarm)
		return FALSE;

	else{
		// Retrieve the current time
		u8 currentMin = RTCC_getMin(&clock, alarm_type);
		u8 currentHour = RTCC_getHour(&clock, alarm_type);

		// Retrieve the alarm time
		u8 alarmMin = RTCC_getMin(&clock, alarm_type);
		u8 alarmHour = RTCC_getHour(&clock, alarm_type);

		if ((currentHour == alarmHour) && (currentMin == alarmMin)){
			switch(alarm_type){
					case MAIN:
						AFlag0 = 1;
						break;
					case WARNING:
						AFlag1 = 1;
						break;
					default:
						break;
			}
		}
	}
	return TRUE;
}


