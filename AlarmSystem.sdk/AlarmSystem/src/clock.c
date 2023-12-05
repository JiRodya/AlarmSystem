// clock.h
// Declaration of clock module functions


// clock.c
#include "clock.h"
#include "sleep.h"
#include "xil_cache.h"
#include "xparameters.h"
#include <stdio.h> // Include for printf
#include "data_handle.h"

/************************** Function Declarations ***************************/


//Time Clock_IncrementTime(Time time, int delta_seconds);
Time Clock_DecrementTime(Time time, int delta_seconds);

/************************** Global Declarations ****************************/
//PmodRTCC clock;
const char *weekdays[7] = {
   "Monday",
   "Tuesday",
   "Wednesday",
   "Thursday",
   "Friday",
   "Saturday",
   "Sunday"
};
/************************** Core Function Definitions  ****************************/
// Initialize the clock module
void Clock_Init(u8 mode) {

    Time time;

    RTCC_begin(&clock,XPAR_PMODRTCC_0_AXI_LITE_IIC_BASEADDR, 0x6F);

    if(!RTCC_checkVbat(&clock)||mode){
    	RTCC_stopClock(&clock);
    	time.second = 0x36;
		time.minute = 0x44;
		time.hour   = 0x11;
		time.ampm   = RTCC_AM;
		time.day    = 0x02;
		time.date   = 0x05;
		time.month  = 0x12;
		time.year   = 0x23;

		time = Clock_IncrementTime(time, 0); // TEST
		Clock_SetTime(time, CLOCK);

		RTCC_startClock(&clock);
		printf("Time set\n");
		RTCC_enableVbat(&clock);
    }
    else{
    	time = Clock_GetTime(CLOCK);
    }
    RTCC_enableVbat(&clock);
    RTCC_clearPWRFAIL(&clock);
    printf("Clock Init Done\n");
}


/************************** Clock Function Definitions ****************************/


Time Clock_GetTime( Target src) {
   Time val;
   RTCC_Target target;
   switch(src){
   	   case CLOCK:
   		   target = RTCC_TARGET_RTCC;
   		   break;
   	   case MAIN_ALARM:
   		   target = RTCC_TARGET_ALM0 ;
   		   break;
   	   case WARNING_ALARM:
   		   target = RTCC_TARGET_ALM1 ;
   		   break;
   	   case CLOCK_PWRD:
   		   target = RTCC_TARGET_PWRD;
   		   break;
   	   case CLOCK_PWRU:
   		   target = RTCC_TARGET_PWRU ;
   		   break;

   }
   if (target != CLOCK_PWRD && target != CLOCK_PWRU) {
      val.second = RTCC_getSec(&clock, target);
   }
	val.minute = RTCC_getMin(&clock, target);
	val.hour   = RTCC_getHour(&clock, target);
	val.ampm   = RTCC_getAmPm(&clock, target);
	val.day    = RTCC_getDay(&clock, target);
	val.date   = RTCC_getDate(&clock, target);
	val.month  = RTCC_getMonth(&clock, target);




   if (src == CLOCK) {
      val.year = RTCC_getYear(&clock);
   } else {
      val.year = 0;
   }

   return val;
}

void Clock_SetTime(Time val, Target t) {


   RTCC_setMin(&clock, t, val.minute);
   RTCC_setHour12(&clock, t, val.hour, val.ampm);
   RTCC_setDay(&clock, t, val.day);
   RTCC_setDate(&clock, t, val.date);
   RTCC_setMonth(&clock, t, val.month);
   xil_printf("SET: %x, %x, %x||\r\n",val.hour,val.minute,val.second);


}

void Clock_SetAlarm(Time time){
	Clock_SetTime(time, MAIN_ALARM ); //set the time

	//enable the alarm
	RTCC_enableAlarm(&clock, MAIN_ALARM,
			ALARM_CONFIG);
}

//----------- Helper Functions
Time Clock_IncrementTime(Time time, int delta_seconds) {
   Time result;
   int temp;
   result = time;
   temp = bcd2int(result.second) + delta_seconds;
   result.second = int2bcd(temp % 60);          // Convert seconds
   temp = bcd2int(result.minute) + temp / 60;   // Carry seconds -> minutes
   result.minute = int2bcd(temp % 60);          // Convert minutes
   temp = bcd2int(result.hour) + temp / 60 - 1; // Carry minutes -> hours
   result.hour = int2bcd((temp % 12) + 1);      // Convert hours
   return result;
}



Time Clock_DecrementTime(Time time, int delta_seconds) {
    Time result;
    int temp;
    result = time;

    // Convert BCD to integer
    temp = bcd2int(result.second) - delta_seconds;

    // Handle underflow for seconds and carry to minutes
    while (temp < 0) {
        temp += 60; // Add 60 seconds and subtract 1 minute
        result.minute = int2bcd(bcd2int(result.minute) - 1);
    }
    result.second = int2bcd(temp % 60); // Convert seconds

    // Handle underflow for minutes and carry to hours
    temp = bcd2int(result.minute);
    while (temp < 0) {
        temp += 60; // Add 60 minutes and subtract 1 hour
        result.hour = int2bcd(bcd2int(result.hour) - 1);
    }
    result.minute = int2bcd(temp % 60); // Convert minutes

    // Handle underflow for hours
    temp = bcd2int(result.hour);
    if (temp < 0) {
        temp += 24; // Loop back after 24 hours
    }
    result.hour = int2bcd(temp % 24); // Convert hours

    return result;
}

u8 Clock_CheckAlarm(){
	return RTCC_checkFlag(&clock, RTCC_TARGET_ALM0);
}
void Clock_ResetAlarm (Target alarm){
	RTCC_disableAlarm(&clock, alarm);
}

