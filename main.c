#include <stdint.h>
#include <stm32f10x.h>

volatile uint32_t blink_period = 4096; 
uint8_t button_state = 0;

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) { 
        GPIOC->ODR ^= GPIO_ODR_ODR13; 
        TIM2->SR &= ~TIM_SR_UIF; 
    }
}

void set_timer_period(uint32_t ms) {
    TIM2->PSC = 1023; 
    TIM2->ARR = ms - 1; 
    TIM2->CR1 |= TIM_CR1_CEN; 
}

[[noreturn]] int main(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    
    GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0; 

    GPIOC->CRH &= ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14);
    GPIOC->CRH |= GPIO_CRH_CNF14_0; 

    GPIOC->CRH &= ~(GPIO_CRH_CNF15 | GPIO_CRH_MODE15);
    GPIOC->CRH |= GPIO_CRH_CNF15_0; 

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    
    set_timer_period(blink_period);
    
    TIM2->DIER |= TIM_DIER_UIE; 
    NVIC_EnableIRQ(TIM2_IRQn); 

    while (1) {
        
        if (!(GPIOC->IDR & GPIO_IDR_IDR14)) {
            if ((button_state & 0x01) == 0) {
                blink_period = blink_period > 50 ? blink_period / 2 : blink_period; 
                set_timer_period(blink_period); 
                button_state |= 0x01;

                
                for (volatile int i = 0; i < 100000; i++);
            }
        } else {
            button_state &= ~0x01;
        }

        if (!(GPIOC->IDR & GPIO_IDR_IDR15)) {
            if ((button_state & 0x02) == 0) {
                blink_period = blink_period < 2000 ? blink_period * 2 : blink_period; 
                set_timer_period(blink_period); 
                button_state |= 0x02;

                
                for (volatile int i = 0; i < 100000; i++);
            }
        } else {
            button_state &= ~0x02;
        }
    }
}

