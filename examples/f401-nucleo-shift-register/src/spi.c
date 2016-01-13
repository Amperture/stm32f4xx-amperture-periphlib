#include <stm32f4xx.h>
#include <stdint.h>

// Valid values for SPI_BAUD_RATE: 0 - 7
#define AMP_SPI_BAUD_RATE 7

// spiInit
void spiInit(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint8_t mosiPin,
        uint8_t misoPin, uint8_t sckPin, uint8_t afMode){

    // Enable RCC APB Register for SPI Peripheral.
    // Uncomment the desired SPI register enable.

    //RCC -> APB1ENR |= RCC_APB1ENR_SPI2EN;
    //RCC -> APB1ENR |= RCC_APB1ENR_SPI3EN;
    //RCC -> APB2ENR |= RCC_APB2ENR_SPI0EN;
    RCC -> APB2ENR |= RCC_APB2ENR_SPI1EN;
    //RCC -> APB2ENR |= RCC_APB2ENR_SPI4EN;

    // Enable RCC AHB Register for GPIO
    // Please replace RCC_AHB1ENR_GPIOxEN, x = (A, B, C, ... ,I)
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Disable Internal SPI Peripheral Clock.
    SPIx -> CR1     &=  ~(SPI_CR1_SPE);

    SPIx -> CR1     =   (0
                        | SPI_CR1_CPHA  // Clock Phase, 0 = 1st Edge, 1 = 2nd
                        // | SPI_CR1_CPOL  // Clock Polariy, 0=Rising, 1=Falling
                        | SPI_CR1_MSTR  // Master Enable
                        // | SPI_CR1_LSBFIRST  // Lowest Significant Bit First
                        | SPI_CR1_SSM   // Software Slave Management
                        | SPI_CR1_SSI   // Internal Slave Select Bit
                        // | SPI_CR1_RXONLY    // Receive Only
                        // | SPI_CR1_DFF   // Data Frame Format, 0=8-bit, 1=16-bit
                        // | SPI_CR1_CRCNEXT   // CRC Next (TODO:Learn about this)
                        // | SPI_CR1_CRCEN // Enable CRC Calculation
                        // | SPI_CR1_BIDIMODE  // 2-direc data mode, 0=2-line
                        // | SPI_CR1_DIDIOE    // 2-direc Output Enable
                        | (SPI_CR1_BR & (AMP_SPI_BAUD_RATE << 3)) // Baud Rate
                        );

    SPIx -> CR2     =   (0
                        // | SPI_CR2_RXDMAEN   // Receive DMA Buffer Enable
                        // | SPI_CR2_TXDMAEN   // Transmit DMA Buffer Enable
                        // | SPI_CR2_SSOE      // SS Output Enable (Multimaster=0)
                        // | SPI_CR2_ERRIE     // Error Interrupt Enable
                        // | SPI_CR2_RXNEIE    // Receive Buffer Not Empty IntEnab
                        // | SPI_CR2_TXEIE     // Transmit Buffer Empty IntEnable
                        );

    // Re-Enable SPI Peripheral
    SPIx -> CR1 |= SPI_CR1_SPE; 

    // Set GPIO pins to AF, High Speed, NoPull
    GPIOx -> MODER      |=  (0x02 << (2 * mosiPin)) | /*(0x02 << (2 * misoPin))*/
        (0x02 << (2 * sckPin)); 
    GPIOx -> OSPEEDR    |=  (0x03 << (2 * mosiPin)) | (0x03 << (2 * misoPin))
        | (0x03 << (2 * sckPin)); 
    GPIOx -> PUPDR      &=  ~((0x03 << (2 * mosiPin)) | (0x03 << (2 * misoPin))
        | (0x03 << (2 * sckPin)));

    // Set GPIO Pins to Alternate Function Setting
    // Please refer to the datasheet for which numbers to use.
    // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00102166.pdf
    // Page 45
    if(mosiPin > 7) GPIOx -> AFR[1] |= (afMode << (4 * (mosiPin - 7)));
    else GPIOx -> AFR[0] |= (afMode << (4 * mosiPin));
    if(misoPin > 7) GPIOx -> AFR[1] |= (afMode << (4 * (misoPin - 7)));
    else GPIOx -> AFR[0] |= (afMode << (4 * misoPin));
    if(sckPin > 7) GPIOx -> AFR[1] |= (afMode << (4 * (sckPin - 7)));
    else GPIOx -> AFR[0] |= (afMode << (4 * sckPin));
}

void spiByteSend(SPI_TypeDef* SPIx, uint8_t data){

    // Wait until transmit buffer is empty.
    while(!(SPIx -> SR & SPI_SR_TXE)); 
    while(SPIx -> SR & SPI_SR_BSY);

    // Feed Data Register
    SPIx -> DR = data; 
}
