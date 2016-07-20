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
uint8_t at24c32_writeByte(uint8_t toWrite, uint16_t addr);

/* Write Multiple Bytes sequentially to 12-bit address.
 *     High bits of address are DONTCARES
 *     Length cannot exceed 32. 
 */
uint8_t at24c32_writeByteMulti(uint8_t* toWrite, uint16_t addr, uint8_t len);

/* Read Single Byte from supplied 12-bit address.
 *     High bits of address are DONTCARES
 */
uint8_t at24c32_readByte(uint16_t addr);

/* Read Multiple Bytes from supplied 12-bit address.
 *     High bits of address are DONTCARES
 *     No Max Read Limit
 */
uint8_t* at24c32_readByteMulti(uint16_t addr, uint8_t len);

uint8_t at24c32_write16bit(uint16_t toWrite, uint16_t addr); 
uint8_t at24c32_write16bitMulti(uint16_t* toWrite, uint16_t addr, 
        uint8_t len); 

uint16_t at24c32_read16bit(uint16_t addr); 
uint16_t* at24c32_read16bitMulti(uint16_t addr, uint8_t len); 

uint16_t at24c32_write32bit(uint32_t toWrite, uint16_t addr); 
uint16_t at24c32_write32bitMulti(uint32_t* toWrite, uint16_t addr, 
        uint8_t len); 

uint32_t at24c32_read32bit(uint16_t addr); 
uint32_t* at24c32_read32bitMulti(uint16_t addr, uint8_t len); 

#endif //_AMP_AT24C32_H


