#include "oled.h"
#include <stdio.h>

/***************************** String Values needed *******************************/
const char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                              "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

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

/***************************** Forward declaration *******************************/
static void OLED_DisplayBitmap(PmodOLED *InstancePtr, const uint8_t bitmap[16][4], int startColumn);


/***************************** Generic Functions *******************************/
void OLED_Initialize(PmodOLED *InstancePtr, u32 GPIO_Address, u32 SPI_Address, u8 orientation, u8 invert) {
    // Initialize the OLED module with orientation and inversion settings
	OLED_Begin(InstancePtr, XPAR_PMODOLED_0_AXI_LITE_GPIO_BASEADDR,
	         XPAR_PMODOLED_0_AXI_LITE_SPI_BASEADDR, orientation, invert);
    OLED_DisplayOn(InstancePtr);
    OLED_Clear(InstancePtr);
}

void OLED_ClearDisplay(PmodOLED *InstancePtr) {
    OLED_Clear(InstancePtr);
    OLED_Update(InstancePtr);
}

void OLED_Terminate(PmodOLED *InstancePtr) {
    // Turn off the OLED display and clean up
    OLED_DisplayOff(InstancePtr);
    OLED_End(InstancePtr);
}



/***************************** Custom Functions *******************************/
//void OLED_DisplayTime(PmodOLED *InstancePtr, const char *timeString) {
//    // Clear the screen and display the time string
//    OLED_Clear(InstancePtr);
//    OLED_SetCursor(InstancePtr, 0, 0); // Set cursor to top-left corner
//    OLED_PutString(InstancePtr, (char *)timeString);
//    OLED_Update(InstancePtr);
//}

void OLED_DisplayTime(PmodOLED *InstancePtr, int hour, int minute, int second) {
    char timeBuffer[20];
    char ampm[] = "AM";
    int intHour = bcd2int(hour); // Convert BCD to integer

    // Convert 24-hour format to 12-hour format
    if (intHour == 0) {
        intHour = 12; // Midnight hour in 12-hour format
    } else if (intHour == 12) {
        strcpy(ampm, "PM"); // Noon hour
    } else if (intHour > 12) {
        intHour -= 12;
        strcpy(ampm, "PM");
    }

    // Format the time as HH:MM:SS AM/PM
    snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02x:%02x %s", intHour, minute, second, ampm);

    // Display the time
    OLED_Clear(InstancePtr);
    OLED_SetCursor(InstancePtr, 0, 0); // Set cursor to top-left corner
    OLED_PutString(InstancePtr, timeBuffer);
    OLED_Update(InstancePtr);
}

//// Helper function: Convert BCD to integer
//int bcd2int(int bcdValue) {
//    return ((bcdValue / 16) * 10) + (bcdValue % 16);
//}


void OLED_DisplayDate(PmodOLED *InstancePtr, int day, int month, int year) {
    char dateBuffer[20];

    // Assuming the year is in BCD format, convert it to integer
    // If the year is already an integer, you can comment out the next line
    int intYear = bcd2int(year);

    // Format the date as DD Mon, YYYY (e.g., 11 Nov, 2023)
    snprintf(dateBuffer, sizeof(dateBuffer), "%02x %s, %04d", day, months[month - 1], intYear);

    // Display the date
    OLED_SetCursor(InstancePtr, 0, 3); // Adjust for position
    OLED_PutString(InstancePtr, dateBuffer);

    OLED_Update(InstancePtr);
}


void OLED_DisplayIcons(PmodOLED *InstancePtr, enum bitmaps choice) {
    switch (choice) {
        case ALARM:
            OLED_DisplayBitmap(InstancePtr, alarm_bitmap,127);
            break;
        case CLOCK:
            OLED_DisplayBitmap(InstancePtr, clock_bitmap,127);
            break;
        // ... other cases if needed
    }
}

/***************************** Helper Functions *******************************/
static void OLED_DisplayBitmap(PmodOLED *InstancePtr, const uint8_t bitmap[16][4], int startColumn) {
    for (int row = 0; row < 16; row++) {
        OLED_SetCursor(InstancePtr, startColumn, row);
        for (int col = 0; col < 4; col++) {
            OLED_WriteByte(InstancePtr, bitmap[row][col]);
        }
    }
    OLED_Update(InstancePtr);
}



