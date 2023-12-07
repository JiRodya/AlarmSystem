#include "io.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"

//#define ONE_TENTH 32500000 // Half of the CPU clock speed/10

// Define global variables

XGpio led;
XGpio push;
XGpio dip;
int led_value =0;
int led_count = 10;



int IO_Initialize() {

	int xResult = XGpio_Initialize(&dip, XPAR_SWITCHES_DEVICE_ID);
	  if (xResult != XST_SUCCESS){
			xil_printf("Dip switch init failed\r\n");
			return XST_FAILURE;
	  }
	  XGpio_SetDataDirection(&dip, 1, 0xffffffff);

	  xResult = XGpio_Initialize(&push, XPAR_BUTTONS_DEVICE_ID);
	  if (xResult != XST_SUCCESS){
		   xil_printf("Push buttone init failed\r\n");
		   return XST_FAILURE;
	  }
	  XGpio_SetDataDirection(&push, 1, 0xffffffff);

	  xResult = XGpio_Initialize(&led, XPAR_LED_IP_DEVICE_ID);
	     if (xResult != XST_SUCCESS){
	  	   xil_printf("Push buttone init failed\r\n");
	  	   return XST_FAILURE;
	     }
	     XGpio_SetDataDirection(&led, 1, 0xffffffff);


	     XGpio_DiscreteWrite(&led, LED_CHANNEL, 0x00); // Turn off LEDs
	     xil_printf("Peripherals ON \r\n");
	 return XST_SUCCESS;
}

void IO_SetLedValue(LED_State state, uint8_t counterValue) {
    static uint8_t blinkState = 0;
    uint8_t ledValue = 0;

    switch (state) {
        case LED_ALARM:
            blinkState = ~blinkState; // Toggle blink state
            ledValue = blinkState ? 0xFF : 0x00;
            break;
        case LED_COUNTER:
            ledValue = (counterValue >= 0) ? counterValue : 0;
            break;
        default:
            ledValue = 0x00; // All LEDs off
            break;
    }

    XGpio_DiscreteWrite(&led, LED_CHANNEL, ledValue);
}

// Implement or remove the remaining functions as per your requirement
