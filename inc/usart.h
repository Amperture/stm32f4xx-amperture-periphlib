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
void usartInit(USART_TypeDef *USARTx, GPIO_TypeDef *GPIOx, uint8_t txPin,
        uint8_t rxPin, uint8_t ctsPin, uint8_t rtsPin, 
        uint8_t ckPin, uint8_t afMode);
void usartByteSend(USART_TypeDef *USARTx, uint8_t data);
uint8_t usartByteReceive(USART_TypeDef *USARTx);
void usartStringSend(USART_TypeDef* USARTx, uint8_t* dataPtr);

#endif /* AMP_USART_H */
