// .h

#include <stm32f4xx.h>
#include "i2c.h"

#define DS3231_DEVICE_ADDRESS 0x68 
#define DS3231_SECONDS_REGISTER 0x00
#define DS3231_ALARM1_SECONDS_REGISTER 0x07
#define DS3231_ALARM2_SECONDS_REGISTER 0x0B
#define DS3231_TEMPERATURE_MSB_REGISTER 0x11
#define DS3231_CONTROL_REGISTER 0x0F

typedef struct date {
    uint8_t dayOfWeek;
    uint8_t dayOfMonth;
    uint8_t month;
    uint8_t hour;           // Stored in 24hr format.
    uint8_t minute;
    uint8_t second;
    uint8_t year;
} ds3231Date;
