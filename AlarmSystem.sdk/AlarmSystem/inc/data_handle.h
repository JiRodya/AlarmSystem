#ifndef DATA_HANDLE_H
#define DATA_HANDLE_H

#include <stdint.h> // Include for uint8_t
#include "clock.h"

uint8_t int2bcd(uint8_t data);
uint8_t bcd2int(uint8_t data);
void FormatDate(Time time, char* buffer);
void FormatTime(Time time, char* buffer);

#endif
