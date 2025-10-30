#include <stdint.h>
#include <stm32f10x.h>

#include "delay.h"

[[noreturn]] int main(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    
    GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;

    
    GPIOC->CRH &= ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14);
    GPIOC->CRH |= GPIO_CRH_CNF14_0;

    GPIOC->CRH &= ~(GPIO_CRH_CNF15 | GPIO_CRH_MODE15);
    GPIOC->CRH |= GPIO_CRH_CNF15_0;

    
    uint32_t blink_frequency = 1;

    // in ms
    uint32_t blink_period = 1000;
    uint8_t button_state = 0;

    while (1) {
        
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        delay_ms(blink_period);

        
        if (!(GPIOC->IDR & GPIO_IDR_IDR14)) {
            if ((button_state & 0x01) == 0) { 
                blink_frequency *= 2;
                if (blink_frequency > 64) blink_frequency = 64;
                blink_period = 1000 / blink_frequency;
                button_state |= 0x01;

                // while (!(GPIOC->IDR & GPIO_IDR_IDR14));
                delay_ms(50);
            }
        } else {
            button_state &= ~0x01;
        }

        
        if (!(GPIOC->IDR & GPIO_IDR_IDR15)) {
            if ((button_state & 0x02) == 0) { 
                blink_frequency /= 2; 
                if (blink_frequency < 1) blink_frequency = 1; 
                blink_period = 1000 / blink_frequency; 
                button_state |= 0x02; 

                // while (!(GPIOC->IDR & GPIO_IDR_IDR15));
                delay_ms(50);
            }
        } else {
            button_state &= ~0x02; 
        }
    }
}

