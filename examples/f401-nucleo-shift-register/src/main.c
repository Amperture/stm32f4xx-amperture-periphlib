/*
 * ===========================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Example of SPI peripheral use to drive a 74HC595 Shift
 *                  Register to achieve a crawling light bar.
 *
 *      Libs Used:  SPI
 *
 *        Version:  1.0
 *        Created:  01/12/2016 
 *       Revision:  none
 *       Compiler:  arm-none-eabi-gcc
 *
 * ===========================================================================
 */

#include <stm32f4xx.h>
#include <stdint.h>
#include <spi.h>

#define BLUE_LED_PIN GPIO_Pin_5

void shiftRegisterSendData(uint8_t data){
    spiByteSend(SPI1, data);
}

void Delay(uint32_t nCount){
    for(; nCount != 0; nCount--);
}

void shiftRegisterLatch(){
    GPIOB -> ODR           |=   (1 << 4);
    Delay(10);
    GPIOB -> ODR           &=   ~(1 << 4);
    Delay(10);
}

void initHeartbeat(){
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA -> MODER = (1 << 10); 
    GPIOA -> ODR &= ~(1 << 5); 
}

int main(void){
    // Initialize SPI1 Clock to be connected to GPIO:
    // SPI1_MOSI      -> PB_5
    // SPI1_SCK       -> PB_3
    // GPIO AF MODE   -> 5
    spiInit(SPI1, GPIOB, 5, 4, 3, 5);


    // Initialize PB_5 to be the Shift Register Latch
    GPIOB -> MODER &= ~(3 << (2*4));
    GPIOB -> MODER |= (1 << (2*4));

    initHeartbeat();

    int j = 1;
    for(;;){
        int i = 0;

        // Turn Heartbeat Light ON
        GPIOB -> ODR           |=   (1 << 4);

        for(i = 0; i < 3; i++){
            shiftRegisterSendData(j);
        }
        
        shiftRegisterLatch();

        // Heartbeat
        GPIOA -> ODR ^= (1 << 5);

        // Delay Function, sets speed of crawling light.
        // NOTE: Watch what happens when you set delay length to 0.
        Delay(300000);

        j = j << 1;
        if (j >= 0xFF) j = 1;
    }
}
