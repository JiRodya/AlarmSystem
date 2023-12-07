#ifndef OLED_H
#define OLED_h

#include <stdint.h>
#include "PmodOLED.h"



enum bitmaps{ALARM, TIME};
void Oled_Init();

void Oled__ClearDisplay();
void Oled_Terminate();

void Oled_WriteToDateSection(char* text);
void Oled_WriteToTimeSection(char* text);
void Oled_WriteToInfoSection(char* text);
void Oled_TimedMessage(char* message, int durationMs);

void Oled_DisplayDate( int day, int month, int year);
void Oled_DisplayTime(int hour, int minute, int second );
void Oled_DisplayIcons( enum bitmaps choice);



void Oled_DisplayMsg(char* msg, int x, int y);


#endif
