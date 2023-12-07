#include "interrupt_handler.h"
#include "oled.h"
#include "clock.h"
#include "data_handle.h"
#include "io.h"
#include <stdio.h>
#include "sleep.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xgpio.h"
#include "xil_printf.h"

#define ONE_TENTH 32500000 // Half of the CPU clock speed/10


//------global variables------
extern XGpio led, dip, push;
extern int led_count;
extern PmodOLED oled;
extern PmodRTCC clock;
extern u8 AlarmTriggered;

//------local variables------
int psb_check, psb_prev, dip_check, dip_check_prev, count = 10;
char timeBuffer[100];
char dateBuffer[100];
char* infoBuffer;
int led_state = 0;

// PS Timer related definitions
XScuTimer_Config *ConfigPtr;
XScuTimer Timer; // Cortex A9 SCU Private Timer Instance
XScuTimer *TimerInstancePtr = &Timer;

//------function prototypes------
static int Init_Peripherals();
void CheckButtonPress();
void CheckDipSwitch();
void BlinkLEDs();
void ResetAlarm();
int HandleCenterButtonPress();


int main() {
    // Setup
    xil_printf("-- CLOCK SYSTEM --\r\n\n");
    count = 0;
    xil_printf("-Init process-\r\n");

    // Initialize peripherals
    if (Init_Peripherals() != XST_SUCCESS) {
        xil_printf("Peripherals initialization failed\r\n");
        return XST_FAILURE;
    }

    // Interrupts setup
    Init_GIC();
    Configure_GIC();
    EnableInts();
    ExceptionInit();

    // Initial values
    dip_check_prev = XGpio_DiscreteRead(&dip, 1);
    psb_prev = XGpio_DiscreteRead(&push, 1);

    // Timer setup
    XScuTimer_LoadTimer(&Timer, ONE_TENTH * dip_check_prev);
    XScuTimer_EnableAutoReload(&Timer);
    XScuTimer_Start(&Timer);
    xil_printf("Timer ON!\r\n");

    // Clock and OLED setup
    Time currentTime = Clock_GetTime(CLOCK);
    FormatDate(currentTime, dateBuffer);
    FormatTime(currentTime, timeBuffer);
    xil_printf("-Init process DONE-\r\n\n");
    xil_printf("Starting Time: %s | date: %s ||\r\n", timeBuffer, dateBuffer);

    // Set alarm
    Time alarm = Clock_IncrementTime(currentTime, 30);
    Clock_SetAlarm(alarm);
    Oled_WriteToDateSection(dateBuffer);
    Oled_WriteToTimeSection(timeBuffer);

    while (TRUE) {
    	  // Check for button press

//    	psb_check = XGpio_DiscreteRead(&push, 1);
    	CheckButtonPress();
		if (HandleCenterButtonPress()) {
//			break; // Exit the main loop
		}
        CheckDipSwitch();
//        BlinkLEDs();

        // Update Time and Display
        currentTime = Clock_GetTime(CLOCK);
        FormatDate(currentTime, dateBuffer);
        FormatTime(currentTime, timeBuffer);
        Oled_WriteToDateSection(dateBuffer);
        Oled_WriteToTimeSection(timeBuffer);

        // Update Active Alarm Section
        infoBuffer = (Clock_IsAlarmActive()) ? " %" : "    ";
        Oled_WriteToInfoSection(infoBuffer);

        // Check if Alarm Went Off
        if (RTCC_checkFlag(&clock, RTCC_TARGET_ALM0)) {
            AlarmTriggered = 1;
            xil_printf("ALARM 0!!!");
            RTCC_disableAlarm(&clock, RTCC_TARGET_ALM0);
            xil_printf("\r\n");
        }
        // Blink LEDs if alarm triggered
		  if (AlarmTriggered) {
			  led_state = !led_state; // Toggle LED state
			  XGpio_DiscreteWrite(&led, LED_CHANNEL, led_state ? 0xFF : 0x00);
			  usleep(500000); // Blink interval (500 ms)
		  }
    }

    DisableIntr();
    return 0;
}

// Check and handle button presses
void CheckButtonPress() {
    psb_check = XGpio_DiscreteRead(&push, 1);
    if (psb_check != psb_prev) {
        psb_check = XGpio_DiscreteRead(&push, 1);

        if (psb_check == psb_prev) {
            HandleCenterButtonPress(); // Handle specific button press
        }
    }
    psb_prev = psb_check; // Update previous button state
}

// Check and handle dip switch changes
void CheckDipSwitch() {
    dip_check = XGpio_DiscreteRead(&dip, 1);
    if (dip_check != dip_check_prev) {
        xil_printf("DIP Switch Status %x, %x\r\n", dip_check_prev, dip_check);
        dip_check_prev = dip_check;
        XScuTimer_LoadTimer(TimerInstancePtr, ONE_TENTH * dip_check);
        led_count = 10; // Reset here as well
        count = 10;
    }
}

// Blink LEDs when alarm is triggered
void BlinkLEDs() {
    if (AlarmTriggered) {
        led_state = !led_state; // Toggle LED state
        XGpio_DiscreteWrite(&led, LED_CHANNEL, led_state ? 0xFF : 0x00);
        usleep(500000); // Blink interval (500 ms)
    }
}

// Handle the center button press
int HandleCenterButtonPress() {
    if (psb_check == 8) { // Center button pressed
//        xil_printf("Center button pressed.\r\n");
        if (AlarmTriggered) {
        	xil_printf("Alarm OFF");
            ResetAlarm(); // Reset alarm if it was triggered
        } else {
            XScuTimer_Stop(TimerInstancePtr); // Stop timer if exiting
            return 1; // Signal to exit the program
        }
    }
    return 0; // Continue the program
}

// Reset the alarm
void ResetAlarm() {
    AlarmTriggered = 0; // Reset alarm triggered flag
    led_count = 0; // Reset LED count
    XGpio_DiscreteWrite(&led, LED_CHANNEL, 0x00); // Turn off LEDs
}

static int Init_Peripherals() {
    int xResult;

    // Initialize LEDs, switches, and buttons
    xResult = IO_Initialize();
    if (xResult != XST_SUCCESS) {
        xil_printf("GPIOs init failed\r\n");
        return XST_FAILURE;
    }

    // Initialize timer
    ConfigPtr = XScuTimer_LookupConfig(XPAR_PS7_SCUTIMER_0_DEVICE_ID);
    xResult = XScuTimer_CfgInitialize(&Timer, ConfigPtr, ConfigPtr->BaseAddr);
    if (xResult != XST_SUCCESS) {
        xil_printf("Timer init() failed\r\n");
        return XST_FAILURE;
    }

    // Initialize clock and OLED
    Oled_Init();
    Clock_Init(SET_CLOCK);
    return xResult;
}
