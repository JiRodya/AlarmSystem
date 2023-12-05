//// Include scutimer header file
//#include "xscugic.h"
//#include "xil_exception.h"
//#include "xil_printf.h"
//#include "interrupt_handler.h"
//#include "xparameters.h"
//#include "xscutimer.h"
//
////my peripherals
//#include "io.h"
//#include "clock.h"
//
////extern XScuTimer *TimerInstancePtr;
//
///***************************** Variables declaration *******************************/
//XScuGic IntcInstance;		/* Interrupt Controller Instance */
//XScuGic *IntcInstancePtr = &IntcInstance;
//XScuGic_Config *IntcConfig;
//
//
///************************* Forward declaration *******************************/
//
//static void Push_Intr_Handler(void *CallBackRef);
//static void Dip_Intr_Handler(void *CallBackRef);
//static void Alarm_Intr_Handler(void *CallBackRef);
//
//
///***************************** Generic Functions *******************************/
//
///**
//*
//* This function initialize interrupt controller
//*
//* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
//*
//* @note		None.
//*
//*/
//int Init_GIC()
//{
//	//Interrupt Controller initialization
//	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);
//	if(!IntcConfig)
//	{
//	   xil_printf("Looking for GIC failed! \r\n");
//	   return XST_FAILURE;
//	}
//
//	int xResult = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig, IntcConfig->CpuBaseAddress);
//	if(xResult != XST_SUCCESS)
//	{
//	   xil_printf("Init GIC failed! \r\n");
//	   return XST_FAILURE;
//	}
//
//	return XST_SUCCESS;
//}
//
//
//
///**
//*
//* This function connect GPIOes to GIC and configure GIC so that interrupts can occur
//*
//* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
//*
//* @note		None.
//*
//*/
//int Configure_GIC()
//{
//	//Interrupt Priority and Trigger Setup for dip switches
//	XScuGic_SetPriorityTriggerType(IntcInstancePtr, XPAR_FABRIC_SWITCHES_IP2INTC_IRPT_INTR, 0xA0, 0x3);
//	//Connect GPIO Interrupt to handler dipswitch
//	int xResult = XScuGic_Connect(IntcInstancePtr,
//							XPAR_FABRIC_SWITCHES_IP2INTC_IRPT_INTR,
//							(Xil_ExceptionHandler)Dip_Intr_Handler,
//							(void *)&SwitchInstance);
//	if(xResult != XST_SUCCESS)
//	{
//	   xil_printf("Connect dip switch interrupt failed! \r\n");
//	   return XST_FAILURE;
//	}
//
//	//Interrupt Priority and Trigger Setup for push button
//	XScuGic_SetPriorityTriggerType(IntcInstancePtr, XPAR_FABRIC_BUTTONS_IP2INTC_IRPT_INTR, 0x8, 0x3);
//	//Connect GPIO Interrupt to handler for push button
//	xResult = XScuGic_Connect(IntcInstancePtr,
//							XPAR_FABRIC_BUTTONS_IP2INTC_IRPT_INTR,
//							(Xil_ExceptionHandler)Push_Intr_Handler,
//							(void *)&ButtonInstance);
//	if(xResult != XST_SUCCESS)
//	{
//	   xil_printf("Connect push button interrupt failed! \r\n");
//	   return XST_FAILURE;
//	}
//
//	//alarm interrupt config
//	XScuGic_SetPriorityTriggerType(IntcInstancePtr, XPAR_FABRIC_PMODRTCC_0_I2C_INTERRUPT_INTR, 0xA0, 0x3);
//
//	xResult = XScuGic_Connect(IntcInstancePtr,
//				XPAR_FABRIC_PMODRTCC_0_I2C_INTERRUPT_INTR,
//	                        (Xil_ExceptionHandler)Alarm_Intr_Handler,
//	                        (void *)&clock); // Assuming 'AlarmInstance' is your RTC instance
//	if(xResult != XST_SUCCESS)
//	{
//	   xil_printf("Connect alarm interrupt failed! \r\n");
//	   return XST_FAILURE;
//	}
//
//	return XST_SUCCESS;
//}
//
///**
// * @brief Enable the interrupt for devices
//*/
//void EnableInts()
//{
//
////   //Enable GPIO and Timer Interrupts in the controller
////   XScuGic_Enable(IntcInstancePtr, XPAR_FABRIC_SWITCHES_IP2INTC_IRPT_INTR);
////   XScuGic_Enable(IntcInstancePtr, XPAR_FABRIC_BUTTONS_IP2INTC_IRPT_INTR);
////   XScuGic_Enable(IntcInstancePtr, XPAR_SCUTIMER_INTR);
////   XScuGic_CPUWriteReg(IntcInstancePtr, 0x0, 0xf);
////
////   //Enable GPIO Interrupts Interrupt
////	XGpio_InterruptEnable(&SwitchInstance, 0xF);
////    XGpio_InterruptGlobalEnable(&SwitchInstance);
////
////	XGpio_InterruptEnable(&ButtonInstance, 0xF);
////    XGpio_InterruptGlobalEnable(&ButtonInstance);
//
//    XScuGic_Enable(IntcInstancePtr, XPAR_FABRIC_PMODRTCC_0_I2C_INTERRUPT_INTR);
//}
//
//void DisableIntr()
//{
//	/*
//	 * Disable and disconnect the interrupt system.
//	 */
//	XScuGic_Disable(IntcInstancePtr, XPAR_FABRIC_PMODRTCC_0_I2C_INTERRUPT_INTR);
//
//	XGpio_InterruptDisable(&SwitchInstance, 0x3);
//	XGpio_InterruptDisable(&ButtonInstance, 0x3);
//
//	/* Disconnect the interrupt */
//	XScuGic_Disable(IntcInstancePtr, XPAR_FABRIC_SWITCHES_IP2INTC_IRPT_INTR);
//	XScuGic_Disconnect(IntcInstancePtr, XPAR_FABRIC_SWITCHES_IP2INTC_IRPT_INTR);
//	XScuGic_Disable(IntcInstancePtr, XPAR_FABRIC_BUTTONS_IP2INTC_IRPT_INTR);
//	XScuGic_Disconnect(IntcInstancePtr, XPAR_FABRIC_BUTTONS_IP2INTC_IRPT_INTR);
//}
//
//void ExceptionInit()
//{
//	Xil_ExceptionInit();
//
//// When an interrupt occurs, the processor first has to interrogate the interrupt
////	controller to find out which peripheral generated the interrupt which is done by
////
//	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
//							(Xil_ExceptionHandler)XScuGic_InterruptHandler,
//							IntcInstancePtr);
//
//	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_FIQ_INT,
//			(Xil_ExceptionHandler) XScuGic_InterruptHandler,
//			(void *)IntcInstancePtr);
//
//	//Xil_ExceptionEnable();
//	Xil_ExceptionEnableMask(XIL_EXCEPTION_ALL);
//}
//
///***************************** Handler Functions *******************************/
//static void Dip_Intr_Handler(void *CallBackRef)
//{
//
//}
//
//static void Push_Intr_Handler(void *CallBackRef)
//{
//
//}
//
//
//static void Alarm_Intr_Handler(void *CallBackRef) {
//	xil_printf("Alarm!!!!!\r\n");
//	Clock_ResetAlarm(MAIN_ALARM);
//}
//
