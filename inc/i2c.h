/**
 * @file i2c.h
 * @author W. Alex Best
 * @date 3 Jan 2016
 * @website http://www.amperture.com
 * @license Modified BSD License
 * @brief I2C Library for stm32f4xx-amperture-periphlib package.
 *
 * This file details public function definitions for the I2C peripheral
 * on the stm32f4 family of microcontrollers, as part of the
 * stm32f4xx-amperture-periphlib package.
 */

#ifndef AMP_I2C_H
#define AMP_I2C_H

// Defines

//BUSY, MSL, SB
#define I2C_STATE_MASTER_MODE_ACTIVE_SR1 ((uint16_t)0x0001)
#define I2C_STATE_MASTER_MODE_ACTIVE_SR2 ((uint16_t)0x0003)

// BUSY, MSL, ADDR, TXE, TRA
#define I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR1 ((uint16_t)0x0082)
#define I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR2 ((uint16_t)0x0007)

// BUSY, MSL, ADDR
#define I2C_STATE_MASTER_RECEIVER_MODE_ACTIVE_SR1 ((uint16_t)0x0002)
#define I2C_STATE_MASTER_RECEIVER_MODE_ACTIVE_SR2 ((uint16_t)0x0003)

// BUSY, MSL, ADD10
#define I2C_STATE_MASTER_MODE_ADDRESS10_SR1 ((uint16_t)0x0008)
#define I2C_STATE_MASTER_MODE_ADDRESS10_SR2 ((uint16_t)0x0003)

// BUSY, MSL, RXNE
#define I2C_STATE_MASTER_BYTE_RECEIVED_SR1 ((uint16_t)0x0040)
#define I2C_STATE_MASTER_BYTE_RECEIVED_SR2 ((uint16_t)0x0003)

// BUSY, MSL, TXE, TRA
#define I2C_STATE_MASTER_BYTE_TRANSMITTING_SR1 ((uint16_t)0x0080)
#define I2C_STATE_MASTER_BYTE_TRANSMITTING_SR2 ((uint16_t)0x0007)

// BUSY, MSL, TRA, TXE, BTF
#define I2C_STATE_MASTER_BYTE_TRANSMITTED_SR1 ((uint16_t)0x0084)
#define I2C_STATE_MASTER_BYTE_TRANSMITTED_SR2 ((uint16_t)0x0007)

// Public Functions

/** I2C Init
 * @brief Init function for i2c peripheral.
 * @param *I2Cx: Which I2C internal peripheral to use.
 * @param *GPIOx: Which GPIO Port to use.
 * @param sclPin: GPIO pin to use for SCL, send as integer, NOT Bitmask.
 * @param sdaPin: GPIO pin to use for SDA, send as integer, NOT Bitmask.
 * @param afMode: Alternate Function mode for GPIO pins, refer to datasheet.
 * @see http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00102166.pdf
 */
void i2cInit(I2C_TypeDef* I2Cx, 
        GPIO_TypeDef* GPIOx, 
        uint8_t sclPin, 
        uint8_t sdaPin, 
        uint8_t afMode
);


/** I2C Send Start
 * @brief Sends the Start Bit on the I2C Peripheral.
 * @param *I2Cx: Which peripheral to send the Start bit over.
 */
void i2cSendStart(I2C_TypeDef* I2Cx);


/** I2C Send Stop
 * @brief Sends the Stop Bit on the I2C Peripheral.
 * @param *I2Cx: Which peripheral to send the Stop bit over.
 */
void i2cSendStop(I2C_TypeDef* I2Cx);


/** I2C Send 7 Bit Address
 * @brief Sends the 7 Bit Address call over the I2C peripheral.
 * @param *I2Cx: Which peripheral to send the address over.
 * @param addr: 7 Bit Address
 * @param dir: 0 = Write, 1 = Read
 */
void i2cSendAddr7bit(I2C_TypeDef* I2Cx, 
        uint8_t addr, 
        uint8_t dir
);


/** I2C Send Byte
 * @brief Sends an 8 bit data value over the I2C peripheral.
 * @param *I2Cx: Which peripheral to send the data through.
 * @param data: byte to send
 * @retval void
 */
void i2cSendData(I2C_TypeDef* I2Cx, 
        uint8_t addr
);


/** I2C Receive Byte
 * @brief Retreives the data on the I2C peripheral.
 * @param *I2Cx: Which peripheral to receive data from.
 * @retval Byte pulled from I2C Data Register
 */
uint8_t i2cRecvData(I2C_TypeDef* I2Cx);


/** I2C State Check
 * @brief Check the state of both status registers on the I2C Peripheral.
 * @param *I2Cx: Where x can be 1, 2, or 3.
 * @param i2cStateSR1: Status Register 1 state desired.
 * @param i2cStateSR2: Status Register 2 state desired.
 * @retval Will return 1 for success, 0 for failure.
 * 
 * TODO: Don't like the fact that StdPeriph uses single variable for 
 * I2C_CheckEvent(), but that allows them to use preprocessor macros for 
 * states, giving better readability. Think about this one and look for a 
 * desired solution.
 */
uint8_t i2cStateCheck(I2C_TypeDef* I2Cx, 
        uint16_t i2cStateSR1, 
        uint16_t i2cStateSR2
);


/** I2C Activate ACK
 * @brief Activates the ACK feature of the given I2C Peripheral
 * @param *I2Cx: Which peripheral to send the ACK bit over.
 * @retval Byte pulled from I2C Data Register
 */
void i2cActivateAck(I2C_TypeDef* I2Cx);


/** I2C Deactivate ACK
 * @brief Activates the ACK feature of the given I2C Peripheral
 * @param *I2Cx: Which peripheral to send the ACK bit over.
 * @retval Byte pulled from I2C Data Register
 */
void i2cDeactivateAck(I2C_TypeDef* I2Cx);


#endif /* AMP_I2C_H */
