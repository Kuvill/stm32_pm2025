#include "stm32f10x.h"

#include "delay.h"
#include "spi_display.h"

void display_draw_chessboard(void) {
    for (uint8_t page = 0; page < 8; page++) {
        display_cmd(0xB0 + page);      
        display_cmd(0x00);             
        display_cmd(0x10);             

        for (uint8_t col = 0; col < 128; col++) {
            
            if ((page + (col / 8)) % 2 == 0) {
                
                display_data(0xFF); 
            } else {
                
                display_data(0x00); 
            }
        }
    }
}


int main(void)
{
    SPI1_Init();

    display_init();

    display_draw_chessboard();

    while (1){}
}
