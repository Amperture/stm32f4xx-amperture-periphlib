void spiInit(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint8_t mosiPin,
        uint8_t misoPin, uint8_t sckPin, uint8_t afMode);
void spiByteSend(SPI_TypeDef *SPIx, uint8_t data);
