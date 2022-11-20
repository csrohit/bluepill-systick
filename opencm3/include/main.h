#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
    void sys_tick_handler(void);
    static void systick_setup(void);
    void delay(uint32_t duration);
    static void gpio_setup(void);
    int main(void);
#ifdef __cplusplus
}
#endif
