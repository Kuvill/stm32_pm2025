#pragma once

#include <stdint.h>

void SPI1_Init( void );

uint8_t SPI1_Read( void );

void display_reset(void);

void display_cmd(uint8_t cmd);

void display_data(uint8_t data);

void display_init(void);

