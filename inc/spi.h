/**
 * @file i2c.h
 * @author W. Alex Best
 * @date 6 Jan 2016
 * @website http://www.amperture.com
 * @license Modified BSD License
 * @brief SPI Library for stm32f4xx-amperture-periphlib package.
 *
 * This file details public function declarations for the SPI peripheral
 * on the stm32f4 family of microcontrollers, as part of the 
 * stm32f4xx-amperture-periphlib package.
 */
#ifndef AMP_SPI_H
#define AMP_SPI_H

// Public Functions
void spiInit(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint8_t mosiPin,
        uint8_t misoPin, uint8_t sckPin, uint8_t afMode);
void spiByteSend(SPI_TypeDef *SPIx, uint8_t data);

#endif /* AMP_SPI_H */
