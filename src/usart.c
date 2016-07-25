/**
 * @file usart.c
 * @author W. Alex Best
 * @website http://www.amperture.com
 * @date 12 Jan 2016
 * @brief USART Peripheral Driver Code for stm32f4xx
 *
 * This file contains private and public functions for using the USART 
 * peripherals on an stm32f4xx microcontroller. Comes as part of the 
 * stm32f4xx-amperture-periphlib package. At current does not support 
 * interrupts or DMA.
 *
 * This driver package at current is not meant to be simply included without
 * review into a project. It is fully expected that the programmer will 
 * read the functions they are calling, and edit them accordingly to their 
 * needs.
 *
 * @see http://www.st.com/web/en/resource/technical/document/reference_manual/DM00096844.pdf
 */

#include <stm32f4xx.h>
#include <stdint.h>
#include <usart.h>

void usartInit(USART_TypeDef* USARTx, GPIO_TypeDef* GPIOx, uint8_t txPin,
        uint8_t rxPin, uint8_t ctsPin, uint8_t rtsPin, uint8_t ckPin,
        uint8_t afMode){

    // Enable the USART clock in the RCC register.
    // Uncommment only two lines, one for USART, one for GPIO.
    //RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;
    //RCC -> APB1ENR |= RCC_APB1ENR_USART3EN;
    //RCC -> APB1ENR |= RCC_APB1ENR_UART4EN;
    //RCC -> APB1ENR |= RCC_APB1ENR_UART5EN;
    //RCC -> APB2ENR |= RCC_APB2ENR_USART6EN;
    //RCC -> APB1ENR |= RCC_APB1ENR_UART7EN;
    //RCC -> APB1ENR |= RCC_APB1ENR_UART8EN;

    // Enable RCC AHB Register for GPIO
    // Please replace RCC_AHB1ENR_GPIOxEN, x = (A, B, C, ..., I)
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Disable Internal USART Peripheral Clock.
    USARTx -> CR1   &=  ~( USART_CR1_UE );

    // Configure Peripheral
    USARTx -> CR1   =   (0
                        // | USART_CR1_SBK     // Send Break Character Enable
                                            // TODO: Research SBK Further

                        // | USART_CR1_RWU     // Receiver Wakeup
                                            // TODO: Research Further
                                            
                        | USART_CR1_RE      // Receiver Enable
                        | USART_CR1_TE      // Transmitter Enable
                        // | USART_CR1_IDLEIE  // IDLE Interrupt Enable
                                            // Research further
                                            
                        // | USART_CR1_RXNEIE  // Receiver Not Empty Int. Enable
                        // | USART_CR1_TCIE    // Transmission Complete Int.Enable
                        // | USART_CR1_TXEIE   // Transmitter Empty Int.Enable

                        // | USART_CR1_PCE     // Parity Control Enable
                        // | USART_CR1_PS      // Parity Selection, 0=Even, 1=Odd
                        // | USART_CR1_PEIE    // Parity Error Interrupt Enable

                        // | USART_CR1_WAKE    // Wake method,0=IdleLine,1=AddrMk
                        // | USART_CR1_M       // Word Length,0=8DataBits,1=9Data

                        // | USART_CR1_OVER8   // Oversampling Mode, 0=16, 1=8
                                            // TODO: Research Further
                        );

    /*
    USARTx -> CR2   |=  (0
                        | USART_CR2_LINEN   // LIN Enable TODO: Research
                        | USART_CR2_ADD     // Address of Node TODO:4-Bit

                                            // TODO: Research LIN
                        | USART_CR2_LBDIE   // LIN Break Int Enable
                        | USART_CR2_LBDL    // LIN Break detect Len, 0=10, 1=11
                        | USART_CR2_LBCL

                        | USART_CR2_CLKEN   // Clock Enable
                        | USART_CR2_CPHA    // Clock Phase, 0=FirstEdge
                        | USART_CR2_CPOL    // Clock Polarity, 0=Rising

                        | USART_CR2_STOP    // TODO:2-Bit
                        );

    USARTx -> CR3   |=  (0
                        | USART_CR3_EIE     // Error Int Enable
                        | USART_CR3_IREN    // IrDA Mode Enable
                        | USART_CR3_IRLP    // IrDA Low Power Enable
                        | USART_CR3_HDSEL   // Half-Duplex Selection, 0=Off

                        | USART_CR3_SCEN    // Smartcade Mode Enable
                        | USART_CR3_NACK    // Smartcard NACK Enable

                        | USART_CR3_DMAR    // Direct Memory Access Rx Enable
                        | USART_CR3_DMAT    // Direct Memory Access Tx Enable

                        | USART_CR3_RTSE    // Request to Send Enable
                        | USART_CR3_CTSE    // Clear to Send Enable
                        | USART_CR3_CTSIE   // CTS Interrupt Enable

                        | USART_CR3_ONEBIT  // One Sample bit method
                                            // 0: Three Sample Bits
                                            // 1: One Sample Bit
                        );

    USARTx -> GTPR  =   (0
                        | USART_GTPR_PSC    // Prescalar Val: TODO: 8-Bit
                        | USART_GTPR_GT     // Guard Time TODO: 8-Bit
                        );
    */

    USARTx -> BRR   =   AMP_USART_FCLK_SPEED / AMP_USART_BAUD_RATE;

    // Re-enable the peripheral
    USARTx -> CR1   |=  USART_CR1_UE;

    // Set GPIO pins to AF, High Speed, NoPull
    GPIOx -> MODER      |=  (0x02 << (2 * txPin)) | (0x02 << (2 * rxPin))
        | (0x02 << (2 * ckPin)) | (0x02 << (2 * ctsPin))
        | (0x02 << (2 * rtsPin)); 
    GPIOx -> OSPEEDR    |=  (0x03 << (2 * txPin)) | (0x03 << (2 * rxPin))
        | (0x03 << (2 * ckPin)) | (0x03 << (2 * ctsPin))
        | (0x03 << (2 * rtsPin)); 
    GPIOx -> PUPDR      &=  ~((0x03 << (2 * txPin)) | (0x03 << (2 * rxPin))
        | (0x03 << (2 * ckPin)) | (0x03 << (2 * ctsPin))
        | (0x03 << (2 * rtsPin)));

    // Set GPIO Pins to Alternate Function Setting
    // Please refer to the datasheet for which numbers to use.
    // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00102166.pdf
    // Page 45
    // TODO: Make this prettier and more readable for future maintainers.
    if(rxPin > 7) GPIOx -> AFR[1] |= (afMode << (4 * (rxPin - 8)));
    else GPIOx -> AFR[0] |= (afMode << (4 * rxPin));
    if(txPin > 7) GPIOx -> AFR[1] |= (afMode << (4 * (txPin - 8)));
    else GPIOx -> AFR[0] |= (afMode << (4 * txPin));
    if(ckPin  > 7) GPIOx -> AFR[1] |= (afMode << (4 * (ckPin  - 8)));
    else GPIOx -> AFR[0] |= (afMode << (4 * ckPin));
    if(ctsPin > 7) GPIOx -> AFR[1] |= (afMode << (4 * (ctsPin - 8)));
    else GPIOx -> AFR[0] |= (afMode << (4 * ctsPin));
    if(rtsPin > 7) GPIOx -> AFR[1] |= (afMode << (4 * (rtsPin - 8)));
    else GPIOx -> AFR[0] |= (afMode << (4 * rtsPin));
}

uint8_t usartByteReceive(USART_TypeDef* USARTx){
    uint8_t data = 0;
    while( !((USARTx -> SR) & USART_SR_RXNE));
    data = USARTx -> DR;
    return data;
}

void usartByteSend(USART_TypeDef* USARTx, uint8_t data){
    while( !(USARTx -> SR & USART_SR_TXE));
    USARTx -> DR = data;
}

void usartStringSend(USART_TypeDef* USARTx, uint8_t* data){
    while(*data){
        usartByteSend(USARTx, *(data++));
    }
}
