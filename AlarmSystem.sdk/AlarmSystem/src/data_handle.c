#include "data_handle.h"

uint8_t int2bcd(uint8_t data) {
    return (((data / 10) & 0xF) << 4) + ((data % 10) & 0xF);
}

uint8_t bcd2int(uint8_t data) {
    return ((data >> 4) * 10) + (data & 0xF);
}