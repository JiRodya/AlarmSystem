#include "data_handle.h"
#include "stdio.h"

uint8_t int2bcd(uint8_t data) {
    return (((data / 10) & 0xF) << 4) + ((data % 10) & 0xF);
}

uint8_t bcd2int(uint8_t data) {
    return ((data >> 4) * 10) + (data & 0xF);
}

void FormatDate(Time time, char* buffer) {
   int intYear = bcd2int(time.year); // Convert BCD to integer, if necessary
   // Format date - Day Month/Date/Year
   snprintf(buffer, 30, "%s %02x/%02x/%04d", weekdays[time.day], time.month, time.date, intYear + 2000);
}

void FormatTime(Time time, char* buffer) {
  // Format time - hour:minute AM/PM
  if (time.ampm) {
     snprintf(buffer, 20, "%02x:%02x:%02x PM", time.hour, time.minute,time.second);
  } else {
     snprintf(buffer, 20, "%02x:%02x:%02x AM", time.hour, time.minute,time.second);
  }
}
