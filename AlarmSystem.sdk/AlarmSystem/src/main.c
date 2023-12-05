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
char* formatDate(int day, int month, int year, char* buffer);
char* formatTimePrint();

int main() {

	EnableCaches();
	// PmodRTCC myRTCC;
	//declare needed instances
//    PmodOLED myOLED;

	//init Clock
    Time currentTime;
    Time alarm;

    // peripherals
    Clock_Init(SET_CLOCK);
//    OLED_Initialize(&myOLED, XPAR_PMODOLED_0_AXI_LITE_GPIO_BASEADDR,
//                    XPAR_PMODOLED_0_AXI_LITE_SPI_BASEADDR, 0, 0); // Assuming normal orientation and no inversion


    //init interrupts

    //configure interrupts

    //enable interrupts



	// Clean up, if necessary
    OLED_Terminate(&myOLED);
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

char* formatDate(int day, int month, int year, char* buffer) {
    int intYear = bcd2int(year); // Convert BCD to integer if necessary
    snprintf(buffer, 20, "%02x %s, %04d", day, months[month - 1], intYear);
    return buffer;
}
