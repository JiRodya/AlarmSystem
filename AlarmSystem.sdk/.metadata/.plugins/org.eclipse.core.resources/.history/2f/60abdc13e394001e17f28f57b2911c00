#ifndef IO_H
#define IO_H

#include <stdint.h>
#include "xgpio.h" // Use of Xilinx GPIO driver

#define LED_CHANNEL 1
#define DEBOUNCE_DELAY 20000 // 20 milliseconds

// Extern declarations for global variables (defined in io.c)
extern XGpio led;
extern XGpio push;
extern XGpio dip;
extern int led_count;

#define LED_CHANNEL 1
#define BUTTON1_MASK 0x01 // First button on bit 0
#define BUTTON2_MASK 0x02 // Second button on bit 1
#define BUTTON3_MASK 0x04 // Third button on bit 2
#define BUTTON4_MASK 0x08 // Fourth button on bit 3

#define BUTTON_INTR_MASK 0x0F

// Extern declarations for Timer (defined in interrupt_handler.c)


typedef enum {
    LED_ALARM,      // Blinking pattern for alarm
    LED_COUNTER     // Binary countdown display
} LED_State;

// Function prototypes
int IO_Initialize();
void IO_SetLedValue(LED_State state, uint8_t counterValue);


#endif // IO_H
