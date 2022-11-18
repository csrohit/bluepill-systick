#include <errno.h>

#include <main.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>



/**
 * @brief Initialize GPIOC port 
 * Enable clock given to GPIOC port in order to use the port
 */
static void clock_setup(void)
{
    rcc_periph_clock_enable(RCC_GPIOC);
}


/**
 * @brief Configure systick to generate tick interrupt every millisecond
 * 
 */
static void systick_setup(void)
{
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);     // set clock source
    STK_CVR = 0;                                        // reset current value register
    systick_set_reload(rcc_ahb_frequency / 1000 - 1);   // set reload value register to generate tick every millisecond
    systick_interrupt_enable();                         // enable tick interrupt
    systick_counter_enable();                           // start systick counter/timer
}

/**
 * @brief Tick count variable
 * - Needs to be declared volatile as it is getting updated in intreeupt context
 * 
 */
static volatile uint64_t _millis = 0;

/**
 * @brief Interrupt service routine for systick handler
 * - Increments tick variable 
 */
void sys_tick_handler(void)
{
    _millis++;
}

/**
 * Delay for a real number of milliseconds
 */
void delay(uint32_t ms)
{
    const uint64_t until = _millis + ms;
    while (_millis < until)
        ;
}

/**
 * @brief Initialize GPIO port
 *
 */
static void gpio_setup(void)
{
    // Set GPIOC pin 13 to output @ 2MHZ in push pull configuration
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

int main(void)
{
    clock_setup();
    systick_setup();
    gpio_setup();

    // Toggle the LED on and off forever
    while (1)
    {
        gpio_toggle(GPIOC, GPIO13);
        delay(500);
    }
    return 0;
}
