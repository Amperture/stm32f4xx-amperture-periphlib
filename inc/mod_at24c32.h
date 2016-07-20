// THINGS TO KNOW ABOUT DEVICE
// 12-Bit Address Length (13 for 64-KB)


#ifndef _AMP_AT24C32_H
#define _AMP_AT24C32_H

#include <stm32f4xx.h>
#include <stdint.h>
#include "i2c.h"


#define AT24C32_DEVICE_ADDRESS 0x50 

/* Write Single Byte to given 12-bit address
 *     High bits of address are DONTCARES
 */
uint8_t at24c32_writeByte(I2C_TypeDef* I2Cx, uint8_t toWrite, uint16_t addr);

/* Write Multiple Bytes sequentially to 12-bit address.
 *     High bits of address are DONTCARES
 *     Length cannot exceed 32. 
 */
uint8_t at24c32_writeByteMulti(I2C_TypeDef* I2Cx, 
        uint8_t* toWrite, 
        uint16_t addr, 
        uint8_t len);

/* Read Single Byte from supplied 12-bit address.
 *     High bits of address are DONTCARES
 */
uint8_t at24c32_readByte(I2C_TypeDef* I2Cx, uint16_t addr);

/* Read Multiple Bytes from supplied 12-bit address.
 *     High bits of address are DONTCARES
 *     No Max Read Limit
 */
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

#endif //_AMP_AT24C32_H


