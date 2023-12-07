#include "interrupt_handler.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "io.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "clock.h"

#include "sleep.h"

extern  u8 AlarmTriggered;
extern XGpio SwitchInstance, ButtonInstance;
extern XScuTimer Timer;
extern int led_count;

extern XGpio dip, push,led;

extern int btn1, btn2, btn3, btn4;


XScuGic IntcInstance;      /* Interrupt Controller Instance */
XScuGic *IntcInstancePtr = &IntcInstance;
XScuGic_Config *IntcConfig;

XScuTimer_Config *ConfigPtr;
XScuTimer Timer;		/* Cortex A9 SCU Private Timer Instance */
XScuTimer *TimerInstancePtr;


int Init_GIC() {
    IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);
    if (IntcConfig == NULL) {
        xil_printf("GIC LookupConfig Failure\r\n");
        return XST_FAILURE;
    }

    int Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig, IntcConfig->CpuBaseAddress);
    if (Status != XST_SUCCESS) {
        xil_printf("GIC CfgInitialize Failure\r\n");
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

int Configure_GIC() {
    XScuGic_SetPriorityTriggerType(IntcInstancePtr, XPAR_SCUTIMER_INTR, 0xA8, 0x3);
    int Status = XScuGic_Connect(IntcInstancePtr, XPAR_SCUTIMER_INTR, (Xil_ExceptionHandler)TimerIntrHandler, &Timer);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    XScuGic_SetPriorityTriggerType(IntcInstancePtr, XPAR_FABRIC_SWITCHES_IP2INTC_IRPT_INTR, 0xA0, 0x3);
    Status = XScuGic_Connect(IntcInstancePtr, XPAR_FABRIC_SWITCHES_IP2INTC_IRPT_INTR, (Xil_ExceptionHandler)Dip_Intr_Handler, &dip);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    XScuGic_SetPriorityTriggerType(IntcInstancePtr, XPAR_FABRIC_BUTTONS_IP2INTC_IRPT_INTR, 0xA0, 0x3);
    Status = XScuGic_Connect(IntcInstancePtr, XPAR_FABRIC_BUTTONS_IP2INTC_IRPT_INTR, (Xil_ExceptionHandler)Push_Intr_Handler, &push);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

void EnableInts() {
    XScuGic_Enable(IntcInstancePtr, XPAR_SCUTIMER_INTR);
    XScuGic_Enable(IntcInstancePtr, XPAR_FABRIC_SWITCHES_IP2INTC_IRPT_INTR);
    XScuGic_Enable(IntcInstancePtr, XPAR_FABRIC_BUTTONS_IP2INTC_IRPT_INTR);

    XScuGic_CPUWriteReg(IntcInstancePtr, 0x0, 0xf);
    XGpio_InterruptEnable(&dip, 0xF);
    XGpio_InterruptGlobalEnable(&dip);

    XGpio_InterruptEnable(&push, 0xF);
    XGpio_InterruptGlobalEnable(&push);

    XScuTimer_EnableInterrupt(TimerInstancePtr);
    xil_printf("Interrupts On\r\n");
}

void DisableIntr() {
	/*
		 * Disable and disconnect the interrupt system.
		 */
		TimerDisableIntrSystem(IntcInstancePtr, XPAR_SCUTIMER_INTR);

		XGpio_InterruptDisable(&dip, 0x3);
		XGpio_InterruptDisable(&push, 0x3);

		/* Disconnect the interrupt */
		XScuGic_Disable(IntcInstancePtr, XPAR_FABRIC_SWITCHES_IP2INTC_IRPT_INTR);
		XScuGic_Disconnect(IntcInstancePtr, XPAR_FABRIC_SWITCHES_IP2INTC_IRPT_INTR);
		XScuGic_Disable(IntcInstancePtr, XPAR_FABRIC_BUTTONS_IP2INTC_IRPT_INTR);
		XScuGic_Disconnect(IntcInstancePtr, XPAR_FABRIC_BUTTONS_IP2INTC_IRPT_INTR);
}

void ExceptionInit() {
    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
    							(Xil_ExceptionHandler)XScuGic_InterruptHandler,
    							IntcInstancePtr);

    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_FIQ_INT,
    			(Xil_ExceptionHandler) XScuGic_InterruptHandler,
    			(void *)IntcInstancePtr);
    Xil_ExceptionEnableMask(XIL_EXCEPTION_ALL);
}

void TimerDisableIntrSystem(XScuGic *IntcInstPtr, u16 TimerIntrId)
{
	/*
	 * Disconnect and disable the interrupt for the Timer.
	 */
	XScuGic_Disconnect(IntcInstPtr, TimerIntrId);
}

void TimerIntrHandler(void *CallBackRef) {
   XScuTimer *TimerInstancePtr = (XScuTimer *) CallBackRef;

	if (XScuTimer_IsExpired(TimerInstancePtr)) {
		XScuTimer_ClearInterruptStatus(TimerInstancePtr);
		XGpio_DiscreteWrite(&led, LED_CHANNEL, led_count);
		if (led_count > 0) {
			led_count--;
		} else {
			led_count = 10; // Reset the count
		}
	}
}



void Push_Intr_Handler(void *CallBackRef) {
	int buttonState;

	xil_printf("Inside Button ISR!\r\n");

	// Read the state of the buttons
	buttonState = XGpio_DiscreteRead(&push, 1);

	usleep(DEBOUNCE_DELAY); // Defined delay for debouncing
	int debouncedState = XGpio_DiscreteRead(&push, 1);
	if (buttonState != debouncedState) {
		// If the button state has changed during the debounce delay, ignore this trigger
		return;
	}

	// Disable GPIO interrupts
	XGpio_InterruptDisable(&push, 0xF);

	// Check which button was pressed and perform corresponding actions
	if (buttonState & BUTTON1_MASK) {
		int switchState = XGpio_DiscreteRead(&dip, 1);
		// Action for Button 1
		xil_printf("Button 1 pressed\r\n");
		if(switchState){
			if(Clock_IsAlarmActive()){
				Time alarm = Clock_GetTime(MAIN_ALARM);
				alarm = Clock_IncrementTime(alarm, 10);
				Clock_SetAlarm(alarm);
			}
		}

	}

	if (buttonState & BUTTON2_MASK) {
		// Action for Button 2
		xil_printf("Button 2 pressed\r\n");
		// ... Your code for Button 2 ...
	}

	// Check which button was pressed and perform corresponding actions
	if (buttonState & BUTTON3_MASK) {
		// Action for Button 3
		xil_printf("Button 3 pressed\r\n");
		// ... Your code for Button 1 ...
	}

	if (buttonState & BUTTON4_MASK) {
		// Action for Button 4
		if (AlarmTriggered) {
			xil_printf("Alarm OFF\r\n");
			AlarmTriggered = 0; // Reset alarm triggered flag
			led_count = 0; // Reset LED count
			XGpio_DiscreteWrite(&led, LED_CHANNEL, 0x00); // Turn off LEDs
		}
	}

	(void)XGpio_InterruptClear(&push, BUTTON_INTR_MASK);
	XGpio_InterruptEnable(&push, BUTTON_INTR_MASK);
}

void Dip_Intr_Handler(void *CallBackRef) {
	XGpio *dip_ptr = (XGpio *) CallBackRef;

	xil_printf("Insider Switch ISR! Ha Ha Ha Ha Ha Ha Ha Ha... \r\n");

	// Disable GPIO interrupts
	// not sure how to setup interrupt mask yet ...
	XGpio_InterruptDisable(dip_ptr, 0xF);
	// Ignore additional button presses (not sure about this function yet)
//	if ((XGpio_InterruptGetStatus(dip_ptr) & INTR_MASK) !=
//			INTR_MASK) {
//			return;
//		}

	int dip_check = XGpio_DiscreteRead(dip_ptr, 1);
	led_count = dip_check;
	XGpio_DiscreteWrite(&led, LED_CHANNEL, led_count);

   (void)XGpio_InterruptClear(dip_ptr, 0xF);
   // Enable GPIO interrupts
   XGpio_InterruptEnable(dip_ptr, 0xF);
}
