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


void i2cInit(I2C_TypeDef* I2Cx, GPIO_TypeDef* GPIOx, uint8_t sclPin,
        uint8_t sdaPin, uint8_t afMode){

    // Enable the I2C and GPIO Clocks in the RCC
    // NOTE: Change this line to the appropriate GPIOxEN
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // Set GPIO Pins to AF Mode, Medium Speed, PullUp, OpenDrain
    GPIOx -> MODER      |=  (0x02 << (2 * sclPin) | (0x02 << (2 * sdaPin)));
    GPIOx -> OSPEEDR    |=  (0x01 << (2 * sclPin) | (0x01 << (2 * sdaPin)));
    GPIOx -> PUPDR      |=  (0x01 << (2 * sclPin) | (0x01 << (2 * sdaPin)));
    GPIOx -> OTYPER     |=  (0x0001 << sclPin) | (0x0001 << sdaPin);

    // Set the GPIO Pins to the appropriate Alternate Function Setting
    // Please refer to the datasheet for which numbers to use.
    // http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00102166.pdf
    // Page 45
    if(sclPin > 7) GPIOx -> AFR[1] |= (afMode << (4 * (sclPin - 8)));
    else GPIOx -> AFR[0] |= (afMode << (4 * sclPin));

    if(sdaPin > 7) GPIOx -> AFR[1] |= (afMode << (4 * (sdaPin - 8)));
    else GPIOx -> AFR[0] |= (afMode << (4 * sdaPin));

    // Enable I2C Core Clock
    RCC -> APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Disable the I2C Peripheral
    I2Cx -> CR1     &=      ~(I2C_CR1_PE);

    // Set up the Second Control Register
    // NOTE: Users, please comment out unnecessary bits.
    I2Cx -> CR2     =   (0
                        | (0x0010)    // Frequency
                        // | I2C_CR2_ITERREN       // Error Int Enable
                        // | I2C_CR2_ITEVTEN       // Event Int Enable
                        // | I2C_CR2_ITBUFEN       // Buffer Int Enable
                        // | I2C_CR2_DMAEN         // DMA Request Enable
                        // | I2C_CR2_LAST          // DMA Last Transfer
                        );

    // Set up the First Control Register
    // NOTE: Users, please comment out unnecessary bits.
    I2Cx -> CR1     =   (0
                        // | I2C_CR1_SMBUS         // SMBus Mode
                        // | I2C_CR1_SMBTYPE       // SMBus Type
                        // | I2C_CR1_ENARP         // ARP Enable
                        // | I2C_CR1_ENPEC         // PEC Enable
                        + I2C_CR1_ACK           // ACK Enable
                        // | I2C_CR1_ENGC          // General Call Enable
                        // | I2C_CR1_NOSTRETCH     // (SLV)ClockStretchDisable
                        // | I2C_CR1_POS           // ACK/PEC postition (rec)
                        // | I2C_CR1_PEC           // Packet Error Check
                        // | I2C_CR1_ALERT         // SMB Alert
                        // | I2C_CR1_SWRST         // Soft Reset
                        );

    //TODO: THIS CODE IS NOT FINAL, TESTING ONLY, RESEARCH!

    // CCR[15] -> Fast vs. Slow mode (1 = fast)
    // CCR[14] -> Duty Cycle (Fast mode only)
    // CCR[11:0] -> Clock Control Speed
    I2Cx -> CCR     =   (0x800d);
    I2Cx -> TRISE   =   (5);
    I2Cx -> OAR1    =   (1);

    // Re-Enable the I2C Peripheral
    I2Cx -> CR1     |=  I2C_CR1_PE;

}

void i2cSendStart(I2C_TypeDef* I2Cx){
    I2Cx -> CR1     |=  I2C_CR1_START;
    return; 
}

void i2cSendStop(I2C_TypeDef* I2Cx){
    I2Cx -> CR1     |=  I2C_CR1_STOP;
    return;
}

void i2cSendAddr7bit(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t dir){

    // Check Direction Bit, then send with the address
    if (dir != 0){
        addr = (addr << 1) | 0x01;
    } else {
        addr = (addr << 1) & ~(0x01);
    }

    I2Cx -> DR  = addr;
}

void i2cSendData(I2C_TypeDef* I2Cx, uint8_t data){
    I2Cx -> DR = data;
    return; 
}

uint8_t i2cRecvData(I2C_TypeDef* I2Cx){
    return (uint8_t) I2Cx -> DR;
}

uint8_t i2cStateCheck(I2C_TypeDef* I2Cx, uint16_t stateSR1, uint16_t stateSR2){
    uint16_t i2cSR1Mask = (I2Cx -> SR1) & stateSR1;
    uint16_t i2cSR2Mask = (I2Cx -> SR2) & stateSR2;

    if ( (stateSR1 != 0) && (i2cSR1Mask != stateSR1) ) return 0;
    if ( (stateSR2 != 0) && (i2cSR2Mask != stateSR2) ) return 0;

    return 1;
}

void i2cActivateAck(I2C_TypeDef* I2Cx){
    I2Cx -> CR1 |= I2C_CR1_ACK; 
    return; 
}

void i2cDeactivateAck(I2C_TypeDef* I2Cx){
    I2Cx -> CR1 &= ~(I2C_CR1_ACK); 
    return; 
}
