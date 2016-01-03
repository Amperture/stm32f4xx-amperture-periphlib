/**
 * @file i2c.h
 * @author W. Alex Best
 * @date 3 Jan 2016
 * @brief File containing public function definitions for i2c periph on stm32.
 *
 * This file details public function definitions for the I2C peripheral
 * on the stm32f4 family of microcontrollers, as part of the
 * stm32f4xx-amperture-periphlib package.
 */


// Public Functions
void i2cInit(I2C_TypeDef, GPIO_TypeDef);
