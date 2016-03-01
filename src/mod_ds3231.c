// .c
// Created 2-28-2016
// @author W. Alex Best
// @website amperture.com

#include "mod_ds3231.h"

/* Register Definitions
 *  --- Reg  -- Value
 *  --- 0x00 -- Seconds
 *  --- 0x01 -- Minutes
 *  --- 0x02 -- Hours
 *  --- 0x03 -- Day of Week ( 1 - 7, 1 = Sunday, 7 = Saturday )
 *  --- 0x04 -- Day of Month 
 *  --- 0x05 -- Month ( 1 - 12 )
 *  --- 0x05 -- Year ( 00 - 99 ) 
 */

void ds3231_readDate(I2C_TypeDef* I2Cx, ds3231Date* date){
    uint8_t buffer[7];
    uint8_t i;

    // Initiate and pull data from the DS3231 over I2C
    i2cActivateAck(I2Cx);

    i2cSendStart(I2Cx);
    while ( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_MODE_ACTIVE_SR2));

    i2cSendAddr7bit(I2Cx, DS3231_MODULE_ADDR, 0);
    while ( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR2));

    i2cSendData(I2Cx, DS3231_SECONDS_REGISTER);
    while ( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_BYTE_TRANSMITTED_SR1,
                I2C_STATE_MASTER_BYTE_TRANSMITTED_SR2));

    i2cSendStart(I2Cx);
    while ( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_MODE_ACTIVE_SR2));

    i2cSendAddr7bit(I2Cx, DS3231_MODULE_ADDR, 1);
    while ( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_RECEIVER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_RECEIVER_MODE_ACTIVE_SR2));

    // Loop 4 times to receive 4 bytes, then send NACK-STOP before receiving
    // 5th byte.
    for(i=0; i < 6; i++){
        while ( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_BYTE_RECEIVED_SR1,
                    I2C_STATE_MASTER_BYTE_RECEIVED_SR2));
        buffer[i] = i2cRecvData(I2Cx);
    }

    i2cDeactivateAck(I2Cx);
    i2cSendStop(I2Cx);

    buffer[6] = i2cRecvData(I2Cx);

    date.second       = buffer[0];
    date.minute       = buffer[1];
    date.hour         = buffer[2];
    date.dayOfWeek    = buffer[3];
    date.dayOfMonth   = buffer[4];
    date.month        = buffer[5];
    date.year         = buffer[6];

    // Send raw data through the converter.
    _ds3231_dateConvert(date);

    return;
}

void ds3231_writeDate(I2C_TypeDef* I2Cx, ds3231Date* date){
    uint8_t buffer[7];
    uint8_t i;

    _ds3231_dateConvert_fromReadToRaw(date);
    buffer[0] = date.second;
    buffer[1] = date.minute;
    buffer[2] = date.hour;
    buffer[3] = date.dayOfWeek;
    buffer[4] = date.dayOfMonth;
    buffer[5] = date.month;
    buffer[6] = date.year;

    i2cActivateAck(I2Cx);

    i2cSendStart(I2Cx);
    while ( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_MODE_ACTIVE_SR2));

    i2cSendAddr7bit(I2Cx, DS3231_MODULE_ADDR, 0);
    while ( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR2));

    i2cSendData(I2Cx, DS3231_SECONDS_REGISTER);
    while ( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_BYTE_TRANSMITTED_SR1,
                I2C_STATE_MASTER_BYTE_TRANSMITTED_SR2));

    for(i = 0; i < 7, i++){
        i2cSendData(I2Cx, buffer[i]);
        while ( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_BYTE_TRANSMITTED_SR1,
                    I2C_STATE_MASTER_BYTE_TRANSMITTED_SR2));
    }
}

//// Private Functions

// Converts data from raw device output to psuedo-human-readable
// See https://datasheets.maximintegrated.com/en/ds/DS3231.pdf
void _ds3231_dateConvert_fromRawToRead(ds3231Date* raw){
    raw.second = ((raw.second & 0x0f) + ((raw.second >> 4)*10));
    raw.minute = ((raw.minute & 0x0f) + ((raw.minute >> 4)*10));
    raw.hour = ((raw.hour & 0x0f) + ((raw.hour >> 4)*10));
    raw.dayOfMonth = ((raw.dayOfMonth & 0x0f) + 
            ((raw.dayOfMonth >> 4)*10));
    raw.year = ((raw.year & 0x0f) + ((raw.year >> 4)*10));
    // Don't worry, Day Of Week is already in human-readable. 
}

void _ds3131_dateConvert_fromReadToRaw(ds3231Date* raw){
    raw.second = ((raw.second/10) << 4) + (raw.second % 10);
    raw.minute = ((raw.minute/10) << 4) + (raw.minute % 10);
    raw.hour = ((raw.hour/10) << 4) + (raw.hour % 10);
    raw.dayOfMonth = ((raw.dayOfMonth/10) << 4) + (raw.dayOfMonth % 10);
    raw.year = ((raw.year/10) << 4) + (raw.year % 10);
}


