#include "oled.h"
#include <stdio.h>
#include "data_handle.h"
#include "sleep.h"

/***************************** String Values needed *******************************/


//alarm_bitmap
const uint8_t alarm_bitmap[16][4] = {
    {0x1F, 0xC0, 0x03, 0xF8},
    {0x60, 0x10, 0x08, 0x06},
    {0x80, 0xC0, 0x07, 0x03},
    {0x87, 0x1F, 0xF8, 0xE1},
    {0xD8, 0xC0, 0x03, 0x1B},
    {0x23, 0x00, 0x00, 0xC4},
    {0x04, 0x00, 0x04, 0x20},
    {0x08, 0x18, 0x10, 0x10},
    {0x18, 0x06, 0x40, 0x10},
    {0x18, 0x01, 0x00, 0x18},
    {0x08, 0x00, 0x00, 0x10},
    {0x08, 0x00, 0x00, 0x10},
    {0x04, 0x00, 0x00, 0x60},
    {0x03, 0x00, 0x00, 0xC0},
    {0x0C, 0x60, 0x06, 0x30},
    {0x10, 0x07, 0xE0, 0x08},
};

//clock_bitmap
const uint8_t clock_bitmap[16][4] = {
    {0x00, 0x00, 0x00, 0x00},
    {0x00, 0x07, 0xF8, 0x00},
    {0x00, 0x60, 0x03, 0x00},
    {0x03, 0x00, 0x00, 0x60},
    {0x04, 0x00, 0x80, 0x10},
    {0x08, 0x00, 0x80, 0x08},
    {0x18, 0x00, 0x80, 0x0C},
    {0x10, 0x00, 0x80, 0x04},
    {0x10, 0x00, 0xE0, 0x04},
    {0x18, 0x00, 0x0C, 0x04},
    {0x08, 0x00, 0x00, 0x08},
    {0x04, 0x00, 0x00, 0x10},
    {0x03, 0x00, 0x00, 0x60},
    {0x00, 0xE0, 0x01, 0x80},
    {0x00, 0x0F, 0xF8, 0x00},
    {0x00, 0x00, 0x00, 0x00},
};

// Constants for section dimensions
#define TIME_SECTION_X 0
#define TIME_SECTION_Y 0


#define DATE_SECTION_X 0
#define DATE_SECTION_Y 3


#define INFO_SECTION_X 12
#define INFO_SECTION_Y 0

/***************************** Forward declaration *******************************/
//static void OLED_DisplayBitmap(PmodOLED *InstancePtr, const uint8_t bitmap[16][4], int startColumn);
static void DrawDivs();

PmodOLED oled;
/***************************** Generic Functions *******************************/
void Oled_Init() {
    // Initialize the OLED module with orientation and inversion settings
	OLED_Begin(&oled, XPAR_PMODOLED_0_AXI_LITE_GPIO_BASEADDR,
	         XPAR_PMODOLED_0_AXI_LITE_SPI_BASEADDR, 0, 0);
    OLED_DisplayOn(&oled);
    OLED_Clear(&oled);
    DrawDivs();
}

void Oled_ClearDisplay() {
    OLED_Clear(&oled);
    OLED_Update(&oled);
}

void Oled_Terminate() {
    // Turn off the OLED display and clean up
    OLED_DisplayOff(&oled);
    OLED_End(&oled);
}

static void DrawDivs(){
	char* H = "===================";

	char* V = "|";
	OLED_SetCursor(&oled, 0,2);
	OLED_PutString(&oled,H);
	OLED_SetCursor(&oled, 11,0);
	OLED_PutString(&oled, V);
}

/***************************** Custom Functions *******************************/
// Write text to the Info section
void Oled_WriteToInfoSection( char* text) {
    OLED_SetCursor(&oled, INFO_SECTION_X, INFO_SECTION_Y);
    OLED_PutString(&oled, text);
    // TODO: Implement logic to handle text that exceeds INFO_SECTION_WIDTH
}

// Write text to the Date section
void Oled_WriteToDateSection( char* text) {
    OLED_SetCursor(&oled, DATE_SECTION_X, DATE_SECTION_Y);
    OLED_PutString(&oled, text);
    // Implement logic to handle text that exceeds DATE_SECTION_WIDTH
}

// Write text to the Time section
void Oled_WriteToTimeSection(char* text) {
    OLED_SetCursor(&oled, TIME_SECTION_X, TIME_SECTION_Y);
    OLED_PutString(&oled, text);
    // Implement logic to handle text that exceeds TIME_SECTION_WIDTH
}

void Oled_DisplayMsg(char* msg, int x, int y)
{
	 OLED_SetCursor(&oled, x, y);
	OLED_PutString(&oled, msg);
}

void Oled_TimedMessage(char *message, int durationMs) {
    // Display the message
   Oled_WriteToInfoSection(message);

    // Wait for the specified duration
    usleep(durationMs * 1000); // usleep takes microseconds

    // Clear the message or display previous information
    Oled_WriteToInfoSection(" ");
}



