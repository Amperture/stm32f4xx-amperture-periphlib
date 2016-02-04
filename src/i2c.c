/**
 * @file i2c.c
 * @author W. Alex Best
 * @website http://www.amperture.com
 * @date 3 Jan 2016
 * @brief I2C Peripheral Driver Code for stm32f4xx
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
 * @see http://www.st.com/web/en/resource/technical/document/reference_manual/DM00096844.pdf
 */


#include <stm32f4xx.h>
#include "i2c.h"


/** I2C Init
 * @brief Init function for i2c peripheral.
 * @param *I2Cx: Which I2C internal peripheral to use.
 * @param *GPIOx: Which GPIO Port to use.
 * @param sclPin: GPIO pin to use for SCL, send as integer, NOT Bitmask.
 * @param sdaPin: GPIO pin to use for SDA, send as integer, NOT Bitmask.
 * @param afMode: Alternate Function mode for GPIO pins, refer to datasheet.
 * @see http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00102166.pdf
 */
void i2cInit(I2C_TypeDef* I2Cx, GPIO_TypeDef GPIOx, uint8_t sclPin,
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
                        | I2C_CR1_ACK           // ACK Enable
                        | I2C_CR1_ENGC          // General Call Enable
                        | I2C_CR1_NOSTRETCH     // (SLV)ClockStretchDisable
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

/** I2C Send Start
 * @brief Sends the Start Bit on the I2C Peripheral.
 * @param *I2Cx: Which peripheral to send the Start bit over.
 */
void i2cSendStart(I2C_TypeDef* I2Cx){
    I2Cx -> CR1     |=  I2C_CR1_START;
}

/** I2C Send Stop
 * @brief Sends the Stop Bit on the I2C Peripheral.
 * @param *I2Cx: Which peripheral to send the Stop bit over.
 */
void i2cSendStop(I2C_TypeDef* I2Cx){
    I2Cx -> CR1     |=  I2C_CR1_STOP;
}

/** I2C Send 7 Bit Address
 * @brief Sends the 7 Bit Address call over the I2C peripheral.
 * @param *I2Cx: Which peripheral to send the ACK bit over.
 * @param addr: 7 Bit Address
 * @param dir: 0 = Write, 1 = Read
 */
void i2cSendAddr7bit(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t dir){
    if (dir != 0){
        addr = (addr << 1) | 0x01;
    } else {
        addr = (addr << 1) & ~(0x01);
    }

    I2Cx -> DR  =   addr;
}

/** I2C Send Byte
 * @brief Sends an 8 bit data value over the I2C peripheral.
 * @param *I2Cx: Which peripheral to send the ACK bit over.
 * @param data: byte to send
 * @retval void
 */
void i2cSendData(I2C_TypeDef* I2Cx, uint8_t data){
    I2Cx -> DR = data;
}

/** I2C Receive Byte
 * @brief Retreives the data on the I2C peripheral.
 * @param *I2Cx: Which peripheral to send the ACK bit over.
 * @retval Byte pulled from I2C Data Register
 */
uint8_t i2cRecvData(I2C_TypeDef* I2Cx){
    return (uint8_t) I2Cx -> DR;
}

/** I2C State Check
 * @brief Check the state of both status registers on the I2C Peripheral.
 * @param *I2Cx: Where x can be 1, 2, or 3.
 * @param i2cStateSR1: Status Register 1 state desired.
 * @param i2cStateSR2: Status Register 2 state desired.
 * @retval Will return 1 for success, 0 for failure.
 * 
 * TODO: Don't like the fact that StdPeriph uses single variable for 
 * I2C_CheckEvent(), but that allows them to use preprocessor macros for 
 * states, giving better readability. Think about this one and look for a 
 * desired solution.
 */
uint8_t i2cStateCheck(I2C_TypeDef* I2Cx, uint16_t stateSR1, uint16_t stateSR2){
    uint16_t i2cSR1Mask = (I2Cx -> SR1) & stateSR1;
    uint16_t i2cSR2Mask = (I2Cx -> SR2) & stateSR2 & 0x00FF;

    if ( (stateSR1 != 0) && (i2cSR1Mask != stateSR1) ) return 0;
    if ( (stateSR2 != 0) && (i2cSR2Mask != stateSR2) ) return 0;

    return 1;
}
