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
#include <mod_ds3231.h>

#define DS3231_MODULE_ADDR 0x68

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

void printDateInfo(ds3231Date* dateIn){
    uint8_t dateStringBuffer[5] = {0,0,0,0,0};
    uint8_t i = 0;

    usartStringSend(USART2, "Display:\r\n");

    itoa(dateIn->hour, dateStringBuffer, 10);
    usartStringSend(USART2, "Hr  = ");
    usartStringSend(USART2, dateStringBuffer);
    usartStringSend(USART2, "\r\n");

    itoa(dateIn->minute, dateStringBuffer, 10);
    usartStringSend(USART2, "Min = ");
    usartStringSend(USART2, dateStringBuffer);
    usartStringSend(USART2, "\r\n");

    itoa(dateIn->second, dateStringBuffer, 10);
    usartStringSend(USART2, "Sec = ");
    usartStringSend(USART2, dateStringBuffer);
    usartStringSend(USART2, "\r\n");

    itoa(dateIn->dayOfWeek, dateStringBuffer, 10);
    usartStringSend(USART2, "DOW = ");
    usartStringSend(USART2, dateStringBuffer);
    usartStringSend(USART2, "\r\n");

    itoa(dateIn->dayOfMonth, dateStringBuffer, 10);
    usartStringSend(USART2, "DOM = ");
    usartStringSend(USART2, dateStringBuffer);
    usartStringSend(USART2, "\r\n");

    itoa(dateIn->month, dateStringBuffer, 10);
    usartStringSend(USART2, "Mon = ");
    usartStringSend(USART2, dateStringBuffer);
    usartStringSend(USART2, "\r\n");

    itoa(dateIn->year, dateStringBuffer, 10);
    usartStringSend(USART2, "Yr  = ");
    usartStringSend(USART2, dateStringBuffer);
    usartStringSend(USART2, "\r\n\r\n");
}

void checkDateChange(){
    ds3231Date currentDate;
    uint8_t termData = usartByteReceive(USART2);

    switch(termData){
        case 'd':
            ds3231_readDate(I2C1, &currentDate);
            printDateInfo(&currentDate);
            return;
            break;

        case 'c':
            configureMenu(&currentDate);
            return;
            break;

        case 'h':
            usartStringSend(USART2, "Help: \r\n");
            usartStringSend(USART2, "   Configure        - c\r\n");
            usartStringSend(USART2, "   Display Date     - d\r\n");
            usartStringSend(USART2, "   Help (This Menu) - h\r\n");
            usartStringSend(USART2, "\r\n\r\n");
            return; 
            break;
    }
}

void configureMenu(){
    usartStringSend(USART2, "Configure:\r\n");
    usartStringSend(USART2, "   Hours      - h\r\n");
    usartStringSend(USART2, "   Minutes    - m\r\n");
    usartStringSend(USART2, "   Seconds    - s\r\n");
    usartStringSend(USART2, "   DayOfWeek  - w\r\n");
    usartStringSend(USART2, "   DayOfMonth - d\r\n");
    usartStringSend(USART2, "   Month      - l\r\n");
    usartStringSend(USART2, "   Year       - y\r\n");
    usartStringSend(USART2, "\r\n\r\n");

    uint8_t termData = usartByteReceive(USART2);
    switch(termData){
        case 'h': // Configure Hours Menu
            usartStringSend(USART2, "Configure Hours:\r\n");
            usartStringSend(USART2, "  Increment - +\r\n");
            usartStringSend(USART2, "  Decrement - -\r\n");
            usartStringSend(USART2, "  Set       - Input 2-Digit Number\r\n");
            usartStringSend(USART2, "              (\"00\" - \"23\")\r\n");

            termData = usartByteReceive(USART2);
            if (termData == '+'){ // Increment Hours by 1
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.hour++;
                    if (currentDate.hour > 23) currentDate.hour -= 24;
                    ds3231_writeDate(I2C1, &currentDate); 

            } else if (termData == '-'){ // Decrement Hours by 1 

                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    if (currentDate.hour == 0) currentDate.hour = 23;
                    else currentDate.hour--;
                    ds3231_writeDate(I2C1, &currentDate); 

            } else if ((termData < '0') || (termData > '2')){

                usartStringSend(USART2, "Invalid Input.\r\n");
                usartStringSend(USART2, "Returning to Main Menu.\r\n");
                return;
            
            } else { // Set Hours Value Manually
                uint8_t tempHours = 0;
                
                // Convert ASCII input to raw number
                tempHours = (termData - '0') * 10;
                termData = usartByteReceive(USART2);

                // Check if ASCII input is a valid number
                if ((termData < '0') || (termData > '9')){
                    usartStringSend(USART2, "Invalid Input.\r\n");
                    usartStringSend(USART2, "Returning to Main Menu.\r\n");
                    return;
                } else {
                    tempHours = tempHours + (termData - '0'); 

                    // Data finally ready, send to clock. 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.hour = tempHours;
                    ds3231_writeDate(I2C1, &currentDate); 
                    return;
                }
            
            }
            break;

        case 'm': // Configure Minutes
            usartStringSend(USART2, "Configure Minutes:\r\n");
            usartStringSend(USART2, "  Increment - +\r\n");
            usartStringSend(USART2, "  Decrement - -\r\n");
            usartStringSend(USART2, "  Set       - Input 2-Digit Number\r\n");
            usartStringSend(USART2, "              (\"00\" - \"59\")\r\n");

            termData = usartByteReceive(USART2);
            if (termData == '+'){ // Increment Mintues by 1
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.minute++;
                    if (currentDate.minute > 59) currentDate.minute -= 60;
                    ds3231_writeDate(I2C1, &currentDate); 

            } else if (termData == '-'){ // Decrement Minutes by 1 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    if (currentDate.minute == 0) currentDate.minute = 59;
                    else currentDate.minute--;
                    ds3231_writeDate(I2C1, &currentDate); 

            } else if ((termData < '0') || (termData > '5')){
                usartStringSend(USART2, "Invalid Input.\r\n");
                usartStringSend(USART2, "Returning to Main Menu.\r\n");
                return;
            
            } else { // Set Minutes Value Manually
                uint8_t tempMinute = 0;
                
                // Convert ASCII input to raw number
                tempMinute = (termData - '0') * 10;
                termData = usartByteReceive(USART2);

                // Check if ASCII input is a valid number
                if ((termData < '0') || (termData > '9')){
                    usartStringSend(USART2, "Invalid Input.\r\n");
                    usartStringSend(USART2, "Returning to Main Menu.\r\n");
                    return;
                } else {
                    tempMinute = tempMinute + (termData - '0'); 

                    // Data finally ready, send to clock. 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.minute = tempMinute;
                    ds3231_writeDate(I2C1, &currentDate); 
                    return;
                }
            
            }
            break;

        case 's': // Configure Seconds
            usartStringSend(USART2, "Configure Seconds:\r\n");
            usartStringSend(USART2, "  Increment - +\r\n");
            usartStringSend(USART2, "  Decrement - -\r\n");
            usartStringSend(USART2, "  Set       - Input 2-Digit Number\r\n");
            usartStringSend(USART2, "              (\"00\" - \"59\")\r\n");

            termData = usartByteReceive(USART2);
            if (termData == '+'){ // Increment Seconds by 1
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.second++;
                    if (currentDate.second > 59) currentDate.second -= 60;
                    ds3231_writeDate(I2C1, &currentDate); 
            } else if (termData == '-'){ // Decrement Seconds by 1 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    if (currentDate.second == 0) currentDate.second = 59;
                    else currentDate.second--;
                    ds3231_writeDate(I2C1, &currentDate); 
            } else if ((termData < '0') || (termData > '5')){
                usartStringSend(USART2, "Invalid Input.\r\n");
                usartStringSend(USART2, "Returning to Main Menu.\r\n");
                return;
            
            } else { // Set Seconds Value Manually
                uint8_t tempSeconds = 0;
                
                // Convert ASCII input to raw number
                tempSeconds = (termData - '0') * 10;
                termData = usartByteReceive(USART2);

                // Check if ASCII input is a valid number
                if ((termData < '0') || (termData > '9')){
                    usartStringSend(USART2, "Invalid Input.\r\n");
                    usartStringSend(USART2, "Returning to Main Menu.\r\n");
                    return;
                } else {
                    tempSeconds = tempSeconds + (termData - '0'); 

                    // Data finally ready, send to clock. 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.second = tempSeconds;
                    ds3231_writeDate(I2C1, &currentDate); 
                    return;
                }
            
            }
            break;

        case 'w': // Configure DayOfWeek
            usartStringSend(USART2, "Configure Weekday:\r\n");
            usartStringSend(USART2, "  Increment - +\r\n");
            usartStringSend(USART2, "  Decrement - -\r\n");
            usartStringSend(USART2, "  Set       - Input 1-Digit Number\r\n");
            usartStringSend(USART2, "              (\"1\" - \"7\")\r\n");

            termData = usartByteReceive(USART2);
            if (termData == '+'){ // Increment Weekday by 1
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.dayOfWeek++;
                    if (currentDate.dayOfWeek > 7){
                        currentDate.dayOfWeek -= 7;
                    }
                    ds3231_writeDate(I2C1, &currentDate); 

            } else if (termData == '-'){ // Decrement Weekday by 1 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.dayOfWeek--;
                    if (currentDate.dayOfWeek < 1){
                        currentDate.dayOfWeek += 7;
                    }
                    ds3231_writeDate(I2C1, &currentDate); 

            } else if ((termData < '1') || (termData > '7')){
                usartStringSend(USART2, "Invalid Input.\r\n");
                usartStringSend(USART2, "Returning to Main Menu.\r\n");
                return;
            
            } else { // Set Weekday Value Manually
                uint8_t tempWeekday = 0;
                
                // Convert ASCII input to raw number
                // Then Send to device
                tempWeekday = (termData - '0');

                // Data finally ready, send to clock. 
                ds3231Date currentDate;
                ds3231_readDate(I2C1, &currentDate);
                currentDate.dayOfWeek = tempWeekday;
                ds3231_writeDate(I2C1, &currentDate); 
                return;
            
            }
            break;

        case 'd': // Configure DayOfMonth
            usartStringSend(USART2, "Configure Seconds:\r\n");
            usartStringSend(USART2, "  Increment - +\r\n");
            usartStringSend(USART2, "  Decrement - -\r\n");
            usartStringSend(USART2, "  Set       - Input 2-Digit Number\r\n");
            usartStringSend(USART2, "              (\"01\" - \"31\")\r\n");

            termData = usartByteReceive(USART2);
            if (termData == '+'){ // Increment Day Of Month by 1
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.dayOfMonth++; 
                    if (currentDate.dayOfMonth > 31) 
                        currentDate.dayOfMonth -= 31;
                    ds3231_writeDate(I2C1, &currentDate); 

            } else if (termData == '-'){ // Decrement Day Of Month by 1 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    if (currentDate.dayOfMonth == 1) 
                        currentDate.dayOfMonth = 31;
                    else currentDate.dayOfMonth--;
                    ds3231_writeDate(I2C1, &currentDate); 

            } else if ((termData < '0') || (termData > '3')){
                usartStringSend(USART2, "Invalid Input.\r\n");
                usartStringSend(USART2, "Returning to Main Menu.\r\n");
                return;
            
            } else { // Set Day Of Month Value Manually
                uint8_t tempMonthday = 0;
                
                // Convert ASCII input to raw number
                tempMonthday = (termData - '0') * 10;
                termData = usartByteReceive(USART2);

                // Check if ASCII input is a valid number
                if ((termData < '0') || (termData > '9')){
                    usartStringSend(USART2, "Invalid Input.\r\n");
                    usartStringSend(USART2, "Returning to Main Menu.\r\n");
                    return;
                } else {
                    tempMonthday = tempMonthday + (termData - '0'); 

                    // Data finally ready, send to clock. 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.dayOfMonth = tempMonthday;
                    ds3231_writeDate(I2C1, &currentDate); 
                    return;
                }
            
            }
            break;

        case 'l': // Configure Month
            usartStringSend(USART2, "Configure Month:\r\n");
            usartStringSend(USART2, "  Increment - +\r\n");
            usartStringSend(USART2, "  Decrement - -\r\n");
            usartStringSend(USART2, "  Set       - Input 2-Digit Number\r\n");
            usartStringSend(USART2, "              (\"01\" - \"12\")\r\n");

            termData = usartByteReceive(USART2);
            if (termData == '+'){ // Increment Month by 1
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.month++;
                    if (currentDate.month > 99) currentDate.month -= 100;
                    ds3231_writeDate(I2C1, &currentDate); 
                    return;

            } else if (termData == '-'){ // Decrement Month by 1 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    if (currentDate.month = 1) currentDate.month = 12;
                    else currentDate.month--;
                    ds3231_writeDate(I2C1, &currentDate); 
                    return; 

            } else if ((termData < '0') || (termData > '1')){
                usartStringSend(USART2, "Invalid Input.\r\n");
                usartStringSend(USART2, "Returning to Main Menu.\r\n");
                return;
            
            } else { // Set Year Value Manually
                uint8_t tempMonth = 0;
                
                // Convert ASCII input to raw number
                tempMonth = (termData - '0') * 10;
                termData = usartByteReceive(USART2);

                // Check if ASCII input is a valid number
                if ((termData < '0') || (termData > '9')){
                    usartStringSend(USART2, "Invalid Input.\r\n");
                    usartStringSend(USART2, "Returning to Main Menu.\r\n");
                    return;
                } else {
                    tempMonth = tempMonth + (termData - '0'); 

                    // Data finally ready, send to clock. 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.month = tempMonth;
                    ds3231_writeDate(I2C1, &currentDate); 
                    return;
                }
            
            }
            break;

        case 'y': // Configure Year
            usartStringSend(USART2, "Configure Seconds:\r\n");
            usartStringSend(USART2, "  Increment - +\r\n");
            usartStringSend(USART2, "  Decrement - -\r\n");
            usartStringSend(USART2, "  Set       - Input 2-Digit Number\r\n");
            usartStringSend(USART2, "              (\"00\" - \"99\")\r\n");

            termData = usartByteReceive(USART2);
            if (termData == '+'){ // Increment Year by 1
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.year++;
                    if (currentDate.year > 99) currentDate.year -= 100;
                    ds3231_writeDate(I2C1, &currentDate); 

            } else if (termData == '-'){ // Decrement Year by 1 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    if (currentDate.year = 0) currentDate.year = 99;
                    else currentDate.year--;
                    ds3231_writeDate(I2C1, &currentDate); 

            } else if ((termData < '0') || (termData > '9')){
                usartStringSend(USART2, "Invalid Input.\r\n");
                usartStringSend(USART2, "Returning to Main Menu.\r\n");
                return;
            
            } else { // Set Year Value Manually
                uint8_t tempYear = 0;
                
                // Convert ASCII input to raw number
                tempYear = (termData - '0') * 10;
                termData = usartByteReceive(USART2);

                // Check if ASCII input is a valid number
                if ((termData < '0') || (termData > '9')){
                    usartStringSend(USART2, "Invalid Input.\r\n");
                    usartStringSend(USART2, "Returning to Main Menu.\r\n");
                    return;
                } else {
                    tempYear = tempYear + (termData - '0'); 

                    // Data finally ready, send to clock. 
                    ds3231Date currentDate;
                    ds3231_readDate(I2C1, &currentDate);
                    currentDate.year = tempYear;
                    ds3231_writeDate(I2C1, &currentDate); 
                    return;
                }
            
            }
            break;

        default:
            usartStringSend(USART2, "Invalid Input.\r\n");
            usartStringSend(USART2, "Returning to Main Menu.\r\n");
    }
}

int main(void){
    // Create storage info for current date/time.
    ds3231Date currentDate;
    uint8_t dateStringBuffer[8] = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 0};


    // Initialize USART2 Clock to be connected to GPIO:
    // USART2_RX      -> PA_3
    // USART2_TX      -> PA_2
    // GPIO AF MODE   -> 5
    usartInit(USART2, GPIOA, 2, 3, 2, 3, 2, 7);

    // Initialize I2C1 to be connected to GPIO:
    // I2C1_SCL       -> PB_8
    // I2C1_SDA       -> PB_9
    i2cInit(I2C1, GPIOB, 8, 9, 4);
    Delay(400000);

    initHeartbeat();
    usartStringSend(USART2, "System Init Successful!\r\n");
    usartStringSend(USART2, "Ready to accept commands! Or press h for Help!");
    usartStringSend(USART2, "\r\n\n\r");

    for(;;){
        // Heartbeat
        GPIOA -> ODR ^= (1 << 5);

        Delay(400000);
        //ds3231_readDate(I2C1, &currentDate);
        //printDateInfo(&currentDate);
        checkDateChange();

        Delay(1000);

    }
}
