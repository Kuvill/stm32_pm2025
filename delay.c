#include "delay.h"

void delay_ms( uint32_t ms ) {
    __asm volatile (
        "push {r0}\n"
        "mov R0, %0\n"
        "_loop:\n"
            "cmp R0, #0\n"
            "beq _exit\n"
            "sub R0, R0, #1\n"
            "nop\n"
            "b _loop\n"
        "_exit:\n"
        "pop {r0}\n"
        :: "r"( 9 * ms )
    );
}


