/**
 * @file i2c.c
 * @author W. Alex Best
 * @website http://www.amperture.com
 * @date 3 Jan 2016
 * @brief File containing driver functions for i2c peripheral on stm32f4xx.
 *
 * This file contains private and public functions for using the I2C peripheral
 * on an stm32f4xx microcontroller. Comes as part of the 
 * stm32f4xx-amperture-periphlib package. At current does not support 
 * interrupts or DMA.
 *
 * This driver package at current is not meant to be simply included without
 * review into a project. It is fully expected that the programmer will 
 * read the functions they are calling, and edit them accordingly to their 
 * needs.
 *
 * @see http://www.st.com/web/en/resource/technical/document/programming_manual/DM00046982.pdf
 */

#include <stm32f4xx.h>


/** I2C Init
 * @brief Init function for i2c peripheral.
 * @param *I2Cx: Which I2C internal peripheral to use.
 * @param *GPIOx: Which GPIO Port to use.
 * @param sclPin: GPIO pin to use for SCL, send as integer, NOT Bitmask.
 * @param sdaPin: GPIO pin to use for SDA, send as integer, NOT Bitmask.
 * @param afMode: Alternate Function mode for GPIO pins, refer to datasheet.
 * @retval void
 * @see http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00102166.pdf
 */
void i2cInit(I2C_TypeDef I2Cx, GPIO_TypeDef GPIOx, uint8_t sclPin,
        uint8_t sdaPin, uint8_t afMode){

    // Enable the I2C and GPIO Clocks in the RCC
    RCC -> APB1ENR |= RCC_APB1ENR_I2C1EN;
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Disable the I2C Peripheral
    I2Cx -> CR1     &=      ~(I2C_CR1_PE);

    // Set up the First Control Register
    // NOTE: Users, please comment out unnecessary bits.
    I2Cx -> CR1     =   (0
                        // | I2C_CR1_SMBUS         // SMBus Mode
                        // | I2C_CR1_SMBTYPE       // SMBus Type
                        | I2C_CR1_ENARP         // ARP Enable
                        | I2C_CR1_ENPEC         // PEC Enable
                        | I2C_CR1_ENGC          // General Call Enable
                        | I2C_CR1_NOSTRETCH     // (SLV)ClockStretchDisable
                        | I2C_CR1_START         // Start Gen Enable
                        | I2C_CR1_STOP          // Stop Gen Enable
                        | I2C_CR1_ACK           // Enable Acknowledge
                        | I2C_CR1_POS           // ACK/PEC postition (rec)
                        | I2C_CR1_PEC           // Packet Error Check
                        // | I2C_CR1_ALERT         // SMB Alert
                        // | I2C_CR1_SWRST         // Soft Reset
                        );

    // Set up the Second Control Register
    // NOTE: Users, please comment out unnecessary bits.
    I2Cx -> CR2     =   (0
                        | (I2C_CR2_FREQ & 2)    // Frequency
                        // | I2C_CR2_ITERREN       // Error Int Enable
                        // | I2C_CR2_ITEVTEN       // Event Int Enable
                        // | I2C_CR2_ITBUFEN       // Buffer Int Enable
                        // | I2C_CR2_DMAEN         // DMA Request Enable
                        // | I2C_CR2_LAST          // DMA Last Transfer
                        );

    // Re-Enable the I2C Peripheral
    I2Cx -> CR1     |=  I2C_CR1_PE;

    // Set GPIO Pins to Alternate Function Mode, High Speed, NoPull, 
    GPIOx -> MODER      |=  (0x02 << (2 * sclPin) + 0x02 << (2 * sdaPin));
    GPIOx -> OSPEEDR    |=  (0x03 << (2 * sclPin) + 0x03 << (2 * sdaPin));
    GPIOx -> PUPDR      &=  ~(0x03 << (2 * sclPin) + 0x03 << (2 * sdaPin));

    // Set the GPIO Pins to the appropriate Alternate Function Setting
    // Please refer to the datasheet for which numbers to use.
    // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00102166.pdf
    // Page 45
    if(sclPin > 7) GPIOx -> AFR[1] |= (afMode << (4 * (sclPin - 7)));
    else GPIOx -> AFR[0] |= (afMode << (4 * sclPin));

    if(sdaPin > 7) GPIOx -> AFR[1] |= (afMode << (4 * (sdaPin - 7)));
    else GPIOx -> AFR[0] |= (afMode << (4 * sdaPin));

}

