#ifndef IO_H
#define IO_H

#include <stdint.h>
#include "xgpio.h" // Assuming use of Xilinx GPIO driver
#include "xscutimer.h"

// Global variables for device instances
static XGpio LedInstance;
static XGpio ButtonInstance;
static XGpio SwitchInstance;

#define LED_CHANNEL 1

// PS Timer related definitions
XScuTimer_Config *ConfigPtr;
extern XScuTimer Timer; // Declare Timer as extern
extern XScuTimer *TimerInstancePtr; // Declare TimerInstancePtr as extern

typedef enum {
    LED_ALARM,      // Blinking pattern for alarm
    LED_COUNTER     // Binary countdown display
} LED_State;

// Function prototypes
void IO_Initialize();
void IO_SetLedValue(LED_State state, uint8_t counterValue) ;
uint8_t IO_ReadButtonValue();
uint8_t IO_ReadSwitchValue();
void IO_EnableInterrupts();
void IO_DisableInterrupts();
void IO_RegisterInterruptHandler(XInterruptHandler handler, void *data);
void IO_ButtonInterruptHandler(void *InstancePtr);
void IO_SwitchInterruptHandler(void *InstancePtr);

#endif // IO_H
