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
#include <usart.h>

void Delay(uint32_t nCount){
    for(; nCount != 0; nCount--);
}

void initHeartbeat(){
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA -> MODER |= (1 << 10); 
    GPIOA -> ODR &= ~(1 << 5); 
}

int main(void){
    // Initialize USART2 Clock to be connected to GPIO:
    // USART2_RX      -> PA_3
    // USART2_TX      -> PA_2
    // GPIO AF MODE   -> 5
    usartInit(USART2, GPIOA, 2, 3, 2, 3, 2, 7);

    initHeartbeat();

    int j = 1;
    for(;;){
        int i = 0;

        // Heartbeat
        GPIOA -> ODR ^= (1 << 5);
        usartStringSend(USART2, "Hello world!!");

        Delay(1000);

    }
}
