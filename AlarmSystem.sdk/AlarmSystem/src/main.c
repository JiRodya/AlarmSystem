#include "clock.h"
#include "oled.h"
#include <stdio.h>
#include "sleep.h"
#include "xparameters.h"
#include "xil_cache.h"
//#include <unistd.h>

// Include scutimer header file
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "interrupt_handler.h"
#include "data_handle.h"




#define ONE_TENTH 32500000 // half of the CPU clock speed/10

/***************************** String Values needed *******************************/
const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                              "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

//forward declarations
void EnableCaches();
void DisableCaches();
void FormatDate(Time time, char* buffer);
void FormatTime(Time time, char* buffer);


int main() {

	EnableCaches();
	// PmodRTCC myRTCC;
	//declare needed instances
//    PmodOLED myOLED;

	//init Clock
    Time currentTime;
    Time alarm;
    char timeBuffer [100];
    char dateBuffer [100];

    // peripherals
    Clock_Init(SET_CLOCK);
//    OLED_Initialize(&myOLED, XPAR_PMODOLED_0_AXI_LITE_GPIO_BASEADDR,
//                    XPAR_PMODOLED_0_AXI_LITE_SPI_BASEADDR, 0, 0); // Assuming normal orientation and no inversion


    //init interrupts

    //configure interrupts

    //enable interrupts
    currentTime = Clock_GetTime(CLOCK);
    int secs = 30; // Number of seconds to increment
    alarm = Clock_IncrementTime(currentTime, secs); // Increment the current time by 30 seconds and store it in the 'alarm' variable

    Clock_SetAlarm(alarm);
    FormatDate(alarm, dateBuffer);
    FormatTime(alarm, timeBuffer);
    xil_printf("Alarm set: %s |  %s\r\n", timeBuffer, dateBuffer);

while(TRUE){
	 sleep(1);
	currentTime = Clock_GetTime(CLOCK);
	alarm = Clock_GetTime(MAIN_ALARM);

	//####TESTING####

	FormatDate(currentTime, dateBuffer);
	FormatTime(currentTime, timeBuffer);
	xil_printf("time: %s | date: %s\r\n", timeBuffer, dateBuffer);

	if(Clock_CheckAlarm()){
		xil_printf("Alarm!!!!!\r\n");
		Clock_ResetAlarm(MAIN_ALARM);

	}




	//###############
}

	// Clean up, if necessary
//    OLED_Terminate(&myOLED);
    DisableCaches();
    //disable interrupts

    return 0;
}

void DisableCaches() {
   Xil_DCacheFlush();
   Xil_DCacheDisable();
   Xil_ICacheInvalidate();
   Xil_ICacheDisable();
}

void EnableCaches(){
	Xil_ICacheEnable();
	Xil_DCacheEnable();
}


void FormatDate(Time time, char* buffer) {
    int intYear = bcd2int(time.year); // Convert BCD to integer, if necessary
    // Format date - Day Month/Date/Year
    snprintf(buffer, 30, "%s %x/%x/%04d", weekdays[time.day], time.month, time.date, intYear + 2000);
}

void FormatTime(Time time, char* buffer) {
   // Format time - hour:minute AM/PM
   if (time.ampm) {
      snprintf(buffer, 20, "%x:%02x:%02x PM", time.hour, time.minute,time.second);
   } else {
      snprintf(buffer, 20, "%x:%02x:%02x AM", time.hour, time.minute,time.second);
   }
}

