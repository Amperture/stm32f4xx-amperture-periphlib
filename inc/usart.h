/**
 * @file usart.h
 * @author W. Alex Best
 * @date 13 Jan 2016
 * @website http://www.amperture.com
 * @license Modified BSD License
 * @brief USART Library for stm32f4xx-amperture-periphlib package.
 *
 * This file details public function declarations for the USART peripheral
 * on the stm32f4 family of microcontrollers, as part of the 
 * stm32f4xx-amperture-periphlib package.
 */
#ifndef AMP_USART_H
#define AMP_USART_H

// Public Functions

/** USART Init
 * @brief Init function for USART peripheral.
 * @param *USARTx: Which USART internal peripheral to use.
 * @param *GPIOx: Which GPIO Port to use.
 * @param txPin: GPIO Pin to use for Tx, send as integer, NOT Bitmask.
 * @param rxPin: GPIO Pin to use for Rx, send as integer, NOT Bitmask.
 * @param ctsPin: GPIO Pin to use for CTS, send as integer, NOT Bitmask.
 * @param rtsPin: GPIO Pin to use for RTS, send as integer, NOT Bitmask.
 * @param ckPin: GPIO Pin to use for CK, send as integer, NOT Bitmask.
 * @param afMode: Alternate Function mode for GPIO pins, refer to datasheet.
 * @retval void
 * @see http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00102166.pdf
 */
void usartInit(USART_TypeDef* USARTx, GPIO_TypeDef* GPIOx, uint8_t txPin,
        uint8_t rxPin, uint8_t ctsPin, uint8_t rtsPin, 
        uint8_t ckPin, uint8_t afMode);


/** USART Send Single Byte
 * @brief Sends a single byte over the configured USART peripheral.
 * @param *USARTx: Which USART peripheral to send data over.
 * @param data: Byte to send.
 */
void usartByteSend(USART_TypeDef *USARTx, uint8_t data);


/** USART Receive Single Byte
 * @brief Pulls a single byte over the configured USART peripheral.
 * @param *USARTx: Which USART peripheral to send data over.
 * @retval 8-bit data pulled from USART.
 */
uint8_t usartByteReceive(USART_TypeDef *USARTx);


/** USART Send String 
 * @brief Send a string of data in 8-bit chunks over UART.
 * @param USARTx: Which USART peripheral to send data over.
 * @param data: Pointer to **null-terminated** data set. 
 */
void usartStringSend(USART_TypeDef* USARTx, uint8_t* dataPtr);

#endif /* AMP_USART_H */
