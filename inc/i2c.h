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
void i2cSendStop(I2C_TypeDef* , GPIO_TypeDef* );
void i2cSendAddr7bit(I2C_TypeDef*, uint8_t, uint8_t);
void i2cSendData(I2C_TypeDef*, uint8_t);
uint8_t i2cRecvData(I2C_TypeDef*);
uint8_t i2cStateCheck(I2C_TypeDef*, uint16_t, uint16_t);


#endif /* AMP_I2C_H */
