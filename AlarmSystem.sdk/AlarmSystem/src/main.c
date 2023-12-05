#include "clock.h"
#include "oled.h"
#include <stdio.h>
#include "sleep.h"
#include "xparameters.h"
//#include <unistd.h>

// Include scutimer header file
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"




#define ONE_TENTH 32500000 // half of the CPU clock speed/10



int main() {
//    PmodRTCC myRTCC;
    PmodOLED myOLED;
    Time currentTime;

    // Initialize Clock and OLED
    Clock_Init();
    OLED_Initialize(&myOLED, XPAR_PMODOLED_0_AXI_LITE_GPIO_BASEADDR,
                    XPAR_PMODOLED_0_AXI_LITE_SPI_BASEADDR, 0, 0); // Assuming normal orientation and no inversion

//    OLED_DisplayIcons(&myOLED, ALARM);
//    uint8_t h = 0x01;
//    uint8_t m = 58;
//    uint8_t s = 0x00;
//    Clock_SetAlarm(h, m, s);
    Clock_SetAlarm();
    while (1) {
            // Get the current time from RTCC
            currentTime = Clock_GetTime( RTCC_TARGET_RTCC);

            printf("Raw Time - Hours: %02x, Minutes: %02x, Seconds: %02x\n",
                           currentTime.hour, currentTime.minute, currentTime.second);
//            printf("Time - Hours: %d, Minutes:%d, Seconds:%d\n",
//                                       currentTime.hour, currentTime.minute, currentTime.second);
//            // Display the time in large format
//            OLED_DisplayLargeTime(&myOLED, currentTime.hour, currentTime.minute, currentTime.second);
//
//            // Display the date
//            OLED_DisplayDate(&myOLED, currentTime.date, currentTime.month, currentTime.year);

//            OLED_DisplayDateTime(&myOLED, currentTime.hour,currentTime.minute, currentTime.second,currentTime.day, currentTime.month,currentTime.year);


            // Add a delay as needed (e.g., to update the time every second)
            OLED_DisplayTime(&myOLED, currentTime.hour, currentTime.minute,currentTime.second);
            OLED_DisplayDate(&myOLED, currentTime.day, currentTime.month,currentTime.year);
//            OLED_DisplayIcons(&myOLED, ALARM);
//            u8 alarmFlag = RTCC_checkFlag(&clock, RTCC_TARGET_ALM0);
//            printf("flag %d",alarmFlag);
//            if (alarmFlag) {
//                    // Alarm 0 has been triggered
//                    xil_printf("ALARM 0 Triggered!\n");
//            }
            sleep(1);
        }

        // Clean up, if necessary
        OLED_Terminate(&myOLED);

    return 0;
}



