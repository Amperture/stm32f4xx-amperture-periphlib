#include "mod_at24c32.h"

void _at24c32_delay(){
    int i = 0;
    for(i = 0; i < AT24C32_MAX_BUS_TIME; i++){};
}

uint8_t at24c32_writeByte(I2C_TypeDef* I2Cx, uint8_t toWrite, uint16_t addr){
    // Define memory address into single bytes.
    uint8_t memAd[2];
    memAd[0] = (uint8_t) ((0xFF00 & addr) >> 8);
    memAd[1] = (uint8_t) (0x00FF & addr);

    // Activate the ACK functionality on the I2C controller.
    i2cActivateAck(I2Cx);

    // Send the Start Bit
    i2cSendStart(I2Cx);
    while( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_MODE_ACTIVE_SR2));

    // Send Device Address
    i2cSendAddr7bit(I2Cx, AT24C32_DEVICE_ADDRESS, 0);
    while( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR2));

    // Send Memory Address
    i2cSendData(I2Cx, memAd[0]);
    while( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_BYTE_TRANSMITTED_SR1,
                I2C_STATE_MASTER_BYTE_TRANSMITTED_SR2));
    i2cSendData(I2Cx, memAd[1]);
    while( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_BYTE_TRANSMITTED_SR1,
                I2C_STATE_MASTER_BYTE_TRANSMITTED_SR2));

    // Send the actual byte of data
    i2cSendData(I2Cx, toWrite);
    while( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_BYTE_TRANSMITTED_SR1,
                I2C_STATE_MASTER_BYTE_TRANSMITTED_SR2));

    // Send STOP and deactivate peripheral.
    i2cDeactivateAck(I2Cx);
    i2cSendStop(I2Cx);

    _at24c32_delay();

    return 0;

}

uint8_t at24c32_readByte(I2C_TypeDef* I2Cx, uint16_t addr){
    
    // Convert 16 bit address into 8-bit chunks
    uint8_t memAd[2];
    memAd[0] = (uint8_t) ((0xFF00 & addr) >> 8);
    memAd[1] = (uint8_t) (0x00FF & addr);

    GPIOA -> ODR ^= (1 << 5);
    i2cActivateAck(I2Cx);

    // Start bit
    i2cSendStart(I2Cx);
    while( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_MODE_ACTIVE_SR2));

    // Address
    i2cSendAddr7bit(I2Cx, AT24C32_DEVICE_ADDRESS, 0);
    while( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR2));

    // Send Memory Address
    i2cSendData(I2Cx, memAd[0]);
    while( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_BYTE_TRANSMITTED_SR1,
                I2C_STATE_MASTER_BYTE_TRANSMITTED_SR2));
    i2cSendData(I2Cx, memAd[1]);
    while( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_BYTE_TRANSMITTED_SR1,
                I2C_STATE_MASTER_BYTE_TRANSMITTED_SR2));

    // Re-START
    i2cSendStart(I2Cx);
    while( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_MODE_ACTIVE_SR2));

    // Re-ADDRESS, with a read bit this time instead of write.
    i2cSendAddr7bit(I2Cx, AT24C32_DEVICE_ADDRESS, 1);
    while( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_RECEIVER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_RECEIVER_MODE_ACTIVE_SR2));

    while ( !i2cStateCheck(I2Cx, I2C_STATE_MASTER_BYTE_RECEIVED_SR1,
                I2C_STATE_MASTER_BYTE_RECEIVED_SR2));

    i2cDeactivateAck(I2Cx);
    i2cSendStop(I2Cx);

    _at24c32_delay();

    return i2cRecvData(I2Cx);
}

/** IGNORE EVERYTHING INSIDE COMMENT
 *
uint8_t at24c32_writeByteMulti(I2C_TypeDef* I2Cx, 
        uint8_t* toWrite, 
        uint16_t addr, 
        uint8_t len);


uint8_t* at24c32_readByteMulti(I2C_TypeDef* I2Cx, 
        uint16_t addr, 
        uint8_t len);

uint8_t at24c32_write16bit(I2C_TypeDef* I2Cx, 
        uint16_t toWrite, 
        uint16_t addr); 

uint8_t at24c32_write16bitMulti(I2C_TypeDef* I2Cx, 
        uint16_t* toWrite, 
        uint16_t addr, 
        uint8_t len); 

uint16_t at24c32_read16bit(I2C_TypeDef* I2Cx, uint16_t addr); 

uint16_t* at24c32_read16bitMulti(I2C_TypeDef* I2Cx, 
        uint16_t addr, 
        uint8_t len); 

uint16_t at24c32_write32bit(I2C_TypeDef* I2Cx, 
        uint32_t toWrite, 
        uint16_t addr); 

uint16_t at24c32_write32bitMulti(I2C_TypeDef* I2Cx, 
        uint32_t* toWrite, 
        uint16_t addr, 
        uint8_t len); 

uint32_t at24c32_read32bit(I2C_TypeDef* I2Cx, uint16_t addr); 
uint32_t* at24c32_read32bitMulti(I2C_TypeDef* I2Cx, 
        uint16_t addr, 
        uint8_t len); 
*/
