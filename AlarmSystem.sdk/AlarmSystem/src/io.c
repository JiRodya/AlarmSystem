#include "io.h"
#include "xscugic.h" // Xilinx GIC driver

#include "xparameters.h"
#include "xgpio.h"
#define ONE_TENTH 32500000 // half of the CPU clock speed/10


XScuTimer Timer; // Define Timer here
XScuTimer *TimerInstancePtr = &Timer; // Define TimerInstancePtr here

/*****************************************************************************/
/**
*
* This function initialize dip switches, push buttons and timer.
*
* @param	ButtonInstance is a pointer to the instance of Button XGpio.
* @param	SwitchInstance is a pointer to the instance of Switch XGpio.
* @param	LedInstance is a pointer to the instance of Leds XGpio.
* @param	pTimer is a pointer to the XScuTimer.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
void IO_Initialize() {
	int xResult = XGpio_Initialize(&SwitchInstance, XPAR_SWITCHES_DEVICE_ID);

	   if (xResult != XST_SUCCESS){
			xil_printf("Switches init failed\r\n");
			return XST_FAILURE;
	   }
	   XGpio_SetDataDirection(&SwitchInstance, 1, 0xffffffff);

	   xResult = XGpio_Initialize(&ButtonInstance, XPAR_BUTTONS_DEVICE_ID);
	   if (xResult != XST_SUCCESS){
		   xil_printf("Buttons init failed\r\n");
		   return XST_FAILURE;
	   }

	   xResult = XGpio_Initialize(&LedInstance, XPAR_ALARM_OUTPUT_DEVICE_ID);
	   if (xResult != XST_SUCCESS){
		   xil_printf("LED init failed\r\n");
		   return XST_FAILURE;
	   }
	   XGpio_SetDataDirection(&LedInstance, 1, 0xffffffff);



	   // Initialize the timer
	   ConfigPtr = XScuTimer_LookupConfig (XPAR_PS7_SCUTIMER_0_DEVICE_ID);
	   xResult = XScuTimer_CfgInitialize(&Timer, ConfigPtr, ConfigPtr->BaseAddr);
	   if (xResult != XST_SUCCESS){
		  xil_printf("Timer init() failed\r\n");
		  return XST_FAILURE;
	   }

	   return XST_SUCCESS;
}

void IO_SetLedValue(LED_State state, uint8_t counterValue) {
    static uint8_t blinkState = 0;
    uint8_t ledValue = 0;

    switch (state) {
        case LED_ALARM:
            // Blink all LEDs
            blinkState = ~blinkState; // Toggle blink state
            ledValue = blinkState ? 0xFF : 0x00;
            break;

        case LED_COUNTER:
            // Display countdown in binary on LEDs
            ledValue = counterValue;
            break;

        default:
            ledValue = 0x00; // Default to all LEDs off
            break;
    }

    // Write the value to the LED device
    XGpio_DiscreteWrite(&LedInstance, LED_CHANNEL, ledValue);
}


