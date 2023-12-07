#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

#include "xscutimer.h"

#include "xscugic.h"

extern XScuTimer_Config *ConfigPtr;
extern XScuTimer Timer;		/* Cortex A9 SCU Private Timer Instance */
extern XScuTimer *TimerInstancePtr;

extern XScuGic IntcInstance;      /* Interrupt Controller Instance */
extern XScuGic *IntcInstancePtr;
extern XScuGic_Config *IntcConfig;


int Init_GIC();
int Configure_GIC();
void ExceptionInit();
void EnableInts();
void DisableIntr();
void Push_Intr_Handler(void *CallBackRef);
void Dip_Intr_Handler(void *CallBackRef);
void TimerIntrHandler(void *CallBackRef);
void TimerDisableIntrSystem(XScuGic *IntcInstPtr, u16 TimerIntrId);

#endif // end of INTERRUPT_HANDLER_H
