#include "spi_display.h"
#include "delay.h"

#include "stm32f10x.h"

static void GPIO_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

    GPIOA->CRL &= ~( GPIO_CRL_CNF0 | GPIO_CRL_MODE0 |
                    GPIO_CRL_CNF1 | GPIO_CRL_MODE1 |
                    GPIO_CRL_CNF4 | GPIO_CRL_MODE4);

    GPIOA->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE1_1 | GPIO_CRL_MODE4_1);

    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5 |
                    GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
    GPIOA->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5_1 | GPIO_CRL_MODE5_0 |
                   GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0);
}

void SPI1_Init(void) {
    GPIO_Init();
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    SPI1->CR1 = 0;
    SPI1->CR1 |= SPI_CR1_CPOL | SPI_CR1_CPHA;
    SPI1->CR1 |= SPI_CR1_MSTR;
    SPI1->CR1 |= SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI1_Write(uint8_t data) {
    
    while(!(SPI1->SR & SPI_SR_TXE));
    
    SPI1->DR = data;
    
    while(SPI1->SR & SPI_SR_BSY);
}

uint8_t SPI1_Read(void) {
    SPI1->DR = 0x00; 
    
    while(!(SPI1->SR & SPI_SR_RXNE));
    
    return SPI1->DR;
}

void display_reset(void) {
    GPIOA->BRR = GPIO_BRR_BR0;
    delay_ms(50);
    GPIOA->BSRR = GPIO_BSRR_BS0;
    delay_ms(50);
}

void display_cmd(uint8_t cmd) {
    GPIOA->BRR = GPIO_BRR_BR4;      
    GPIOA->BRR = GPIO_BRR_BR1;  
    SPI1_Write(cmd);
    GPIOA->BSRR = GPIO_BSRR_BS4;     
}

void display_data(uint8_t data) {
    GPIOA->BRR = GPIO_BRR_BR4;      
    GPIOA->BSRR = GPIO_BSRR_BS1;     
    SPI1_Write(data);
    GPIOA->BSRR = GPIO_BSRR_BS4;     
}

void display_init(void) {
    display_reset();

    display_cmd(0xAE); 
    display_cmd(0xD5); 
    display_cmd(0x80);
    display_cmd(0xA8); 
    display_cmd(0x3F); 
    display_cmd(0xD3); 
    display_cmd(0x00);
    display_cmd(0x40); 
    display_cmd(0x8D); 
    display_cmd(0x14); 
    display_cmd(0x20); 
    display_cmd(0x00); 
    display_cmd(0xA1); 
    display_cmd(0xC8); 
    display_cmd(0xDA); 
    display_cmd(0x12);
    display_cmd(0x81); 
    display_cmd(0xCF);
    display_cmd(0xD9); 
    display_cmd(0xF1);
    display_cmd(0xDB); 
    display_cmd(0x40);
    display_cmd(0xA4); 
    display_cmd(0xA6); 
    display_cmd(0xAF); 
}

