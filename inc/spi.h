/**
 * @file spi.h
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

/** SPI Init
 * @brief Init function for spi peripheral.
 * @param *SPIx: Which SPI internal peripheral to use.
 * @param *GPIOx: Which GPIO Port to use.
 * @param mosiPin: GPIO Pin to use for MOSI, send as integer, NOT Bitmask.
 * @param misoPin: GPIO Pin to use for MISO, send as integer, NOT Bitmask.
 * @param sckPin: GPIO Pin to use for SCK, send as integer, NOT Bitmask.
 * @param afMode: Alternate Function mode for GPIO pins, refer to datasheet.
 * @retval void
 * @see http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00102166.pdf
 */
void spiInit(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint8_t mosiPin,
        uint8_t misoPin, uint8_t sckPin, uint8_t afMode);

/** Send Single Byte
 * @brief Sends a single byte over the configured SPI peripheral.
 * @param *SPIx: Which SPI peripheral to send data over.
 * @param data: Byte to send.
 */
void spiByteSend(SPI_TypeDef *SPIx, uint8_t data);

/** Receive Single Byte
 * @brief Pulls a single byte over the configured SPI peripheral.
 * @param *SPIx: Which SPI peripheral to send data over.
 * @retval The value pulled from the SPI peripheral.
 */
uint8_t spiByteReceive( SPI_TypeDef SPIx, uint8_t data);

#endif /* AMP_SPI_H */
