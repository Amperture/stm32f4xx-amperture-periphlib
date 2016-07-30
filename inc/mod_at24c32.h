// THINGS TO KNOW ABOUT DEVICE
// 12-Bit Address Length (13 for 64-KB)


#ifndef _AMP_AT24C32_H
#define _AMP_AT24C32_H

#include <stm32f4xx.h>
#include <stdint.h>
#include "i2c.h"


#ifndef AT24C32_DEVICE_ADDRESS
#define AT24C32_DEVICE_ADDRESS 0x57
#endif

#ifndef AT24C32_MAX_BUS_TIME
#define AT24C32_MAX_BUS_TIME 1716
#endif

/* Write Single Byte
 * @brief Write a Single Byte to the AT24C32 device on bus.
 *
 * @param *I2Cx: Which I2C peripheral AT24C32 is attached to.
 * @param toWrite: 8-bit value to write onto device.
 * @param addr: Address of memory location desired to write to.
 *
 * @retval: System returns success or failure. 0 for success.
 */
uint8_t at24c32_writeByte(I2C_TypeDef* I2Cx, uint8_t toWrite, uint16_t addr);

/* Write Multiple Bytes
 * @brief Write a string of 8-bit values to the device.
 * Please note that the high bits of the \c addr are \c DONTCARE
 * Please note that \c len  should not exceed 32
 *
 * @param *I2Cx: Which I2C peripheral AT24C32 is attached to.
 * @param *toWrite: Pointer to array of 8-bit values.
 * @param addr: First address in memory of device to write to.
 * @param len: Length of data array to write to device
 *
 * @retval: System returns success or failure. 0 for success.
 */
uint8_t at24c32_writeByteMulti(I2C_TypeDef* I2Cx, 
        uint8_t* toWrite, 
        uint16_t addr, 
        uint8_t len
);

/* Read Single Byte
 * @brief Read a single 8-bit value from the device.
 * Please note that the high bits of the \c addr are \c DONTCARE
 *
 * @param *I2Cx: Which I2C peripheral AT24C32 is attached to.
 * @param addr: Memory address on device to pull byte from.
 *
 * @retval System returns the contents of the memory location.
 */
uint8_t at24c32_readByte(I2C_TypeDef* I2Cx, uint16_t addr);


/* Read Multiple Bytes from supplied 12-bit address.
 *
 * @brief Read a series of 8-bit values from the device.
 * Please note that the high bits of the \c addr are \c DONTCARE
 *
 * @param *I2Cx: Which I2C peripheral AT24C32 is attached to.
 * @param addr: Memory address on device to pull byte from.
 * @param len: Length of data array to read from device
 *
 * @retval System returns the contents of the memory location.
 */
uint8_t at24c32_readByteMulti(I2C_TypeDef* I2Cx, 
        uint16_t addr, 
        uint8_t len,
        uint8_t* toRead
);

/* Write single 16-bit value to supplied 12-bit address.
 *
 * @brief Read a single 16-bit value from the device.
 * Please note that the high bits of the \c addr are \c DONTCARE
 *
 * @param *I2Cx: Which I2C peripheral AT24C32 is attached to.
 * @param toWrite: 16-bit value to write
 * @param addr: Memory address on device to pull byte from.
 *
 * @retval Returns pass/fail code. 0 = Success, -1 Failure
 */
uint8_t at24c32_write16bit(I2C_TypeDef* I2Cx, 
        uint16_t toWrite, 
        uint16_t addr
); 

/* Read single 16-bit value from supplied 12-bit address.
 *
 * @brief Read a single 16-bit value from the device.
 * Please note that the high bits of the \c addr are \c DONTCARE
 *
 * @param *I2Cx: Which I2C peripheral AT24C32 is attached to.
 * @param addr: Memory address on device to pull byte from.
 *
 * @retval Returns 16-bits from device.
 */
uint16_t at24c32_read16bit(I2C_TypeDef* I2Cx, uint16_t addr); 

/* EVERYTHING BELOW THIS LINE CONSIDER WIP.
  

uint8_t at24c32_write16bitMulti(I2C_TypeDef* I2Cx, 
        uint16_t* toWrite, 
        uint16_t addr, 
        uint8_t len); 

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

#endif //_AMP_AT24C32_H


