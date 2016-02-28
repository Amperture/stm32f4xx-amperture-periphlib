/*
 * ===========================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Example of a real time clock derived from a DS3231 module.
 *
 *      Libs Used:  USART I2C
 *
 *        Version:  1.0
 *        Created:  01/28/2016 
 *       Revision:  none
 *       Compiler:  arm-none-eabi-gcc
 *
 * ===========================================================================
 */

#include <stm32f4xx.h>
#include <stdint.h>
#include <usart.h>
#include <i2c.h>
#include <stdlib.h>

#define DS3231_MODULE_ADDR 0x68
#define DS3231_SECONDS_REGISTER 0x01
#define DS3231_CONTROL_REGISTER 0x0E

void Delay(uint32_t nCount){
    for(; nCount != 0; nCount--);
}

/* I shouldn't really be including this function, as it's no longer necessary,
 * but it proved far too useful in the process of learning and debugging the
 * I2C Code, so I'm leaving it as a snippet for future use.
 */
void i2cStatePrint(I2C_TypeDef* I2Cx){
    uint16_t i2cState;
    uint8_t i2cStateString[30];

    /*
    i2cState = I2Cx -> TRISE;
    itoa(i2cState, i2cStateString, 16);
    usartStringSend(USART2, "TRISE = "); 
    usartStringSend(USART2, i2cStateString); 
    usartStringSend(USART2, "\r\n"); 

    i2cState = I2Cx -> CCR;
    itoa(i2cState, i2cStateString, 16);
    usartStringSend(USART2, "CCR = "); 
    usartStringSend(USART2, i2cStateString); 
    usartStringSend(USART2, "\r\n"); 

    i2cState = I2Cx -> CR1;
    itoa(i2cState, i2cStateString, 16);
    usartStringSend(USART2, "CR1 = "); 
    usartStringSend(USART2, i2cStateString); 
    usartStringSend(USART2, "\r\n"); 

    i2cState = I2Cx -> CR2;
    itoa(i2cState, i2cStateString, 16);
    usartStringSend(USART2, "CR2 = "); 
    usartStringSend(USART2, i2cStateString); 
    usartStringSend(USART2, "\r\n"); 
    */

    i2cState = I2Cx -> DR;
    itoa(i2cState, i2cStateString, 16);
    usartStringSend(USART2, "DR = "); 
    usartStringSend(USART2, i2cStateString); 
    usartStringSend(USART2, "\r\n"); 

    i2cState = I2Cx -> SR1;
    itoa(i2cState, i2cStateString, 16);
    usartStringSend(USART2, "SR1 = "); 
    usartStringSend(USART2, i2cStateString); 
    usartStringSend(USART2, "\r\n"); 

    i2cState = I2Cx -> SR2;
    itoa(i2cState, i2cStateString, 16);
    usartStringSend(USART2, "SR2 = "); 
    usartStringSend(USART2, i2cStateString); 
    usartStringSend(USART2, "\r\n"); 

    usartStringSend(USART2, "\r\n"); 
}

void initHeartbeat(){
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA -> MODER |= (1 << 10); 
    GPIOA -> ODR &= ~(1 << 5); 
}

uint8_t ds3231_readSeconds(){
    uint8_t i;

    i2cActivateAck(I2C1);

    i2cSendStart(I2C1);
    while ( !i2cStateCheck(I2C1, I2C_STATE_MASTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_MODE_ACTIVE_SR2));

    i2cSendAddr7bit(I2C1, DS3231_MODULE_ADDR, 0);
    while ( !i2cStateCheck(I2C1, I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_TRANSMITTER_MODE_ACTIVE_SR2));

    i2cSendData(I2C1, DS3231_SECONDS_REGISTER);
    while ( !i2cStateCheck(I2C1, I2C_STATE_MASTER_BYTE_TRANSMITTED_SR1,
                I2C_STATE_MASTER_BYTE_TRANSMITTED_SR2));

    i2cSendStart(I2C1);
    while ( !i2cStateCheck(I2C1, I2C_STATE_MASTER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_MODE_ACTIVE_SR2));

    i2cSendAddr7bit(I2C1, DS3231_MODULE_ADDR, 1);
    while ( !i2cStateCheck(I2C1, I2C_STATE_MASTER_RECEIVER_MODE_ACTIVE_SR1,
                I2C_STATE_MASTER_RECEIVER_MODE_ACTIVE_SR2));

    while ( !i2cStateCheck(I2C1, I2C_STATE_MASTER_BYTE_RECEIVED_SR1,
                I2C_STATE_MASTER_BYTE_RECEIVED_SR2));
    i2cDeactivateAck(I2C1);
    i2cSendStop(I2C1);

    i = i2cRecvData(I2C1);

    return i;
}

int main(void){
    // Initialize USART2 Clock to be connected to GPIO:
    // USART2_RX      -> PA_3
    // USART2_TX      -> PA_2
    // GPIO AF MODE   -> 5
    usartInit(USART2, GPIOA, 2, 3, 2, 3, 2, 7);

    // Initialize I2C1 to be connected to GPIO:
    // I2C1_SCL       -> PB_8
    // I2C1_SDA       -> PB_9
    i2cInit(I2C1, GPIOB, 8, 9, 4);

    initHeartbeat();

    uint8_t j = 0;
    uint8_t stringOfJ[8] = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 0};
    for(;;){
        // Heartbeat
        GPIOA -> ODR ^= (1 << 5);

        Delay(400000);
        j = ds3231_readSeconds();

        itoa(j, stringOfJ, 16); 

        usartStringSend(USART2, stringOfJ);
        Delay(1000);

    }
}
