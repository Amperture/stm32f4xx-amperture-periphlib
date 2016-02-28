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

// Public Functions
void i2cInit(I2C_TypeDef* , GPIO_TypeDef* , uint8_t, uint8_t, uint8_t);
void i2cSendStop(I2C_TypeDef*);
void i2cSendStart(I2C_TypeDef*);
void i2cSendAddr7bit(I2C_TypeDef*, uint8_t, uint8_t);
void i2cSendData(I2C_TypeDef*, uint8_t);
uint8_t i2cRecvData(I2C_TypeDef*);
uint8_t i2cStateCheck(I2C_TypeDef*, uint16_t, uint16_t);
void i2cActivateAck(I2C_TypeDef*);
void i2cDeactivateAck(I2C_TypeDef*);

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

#endif /* AMP_I2C_H */
