
// -----------------------------------------------------------------------------
// main program
//
// DISCLAIMER:
// Feabhas is furnishing this item "as is". Feabhas does not provide any
// warranty of the item whatsoever, whether express, implied, or statutory,
// including, but not limited to, any warranty of merchantability or fitness
// for a particular purpose or any warranty that the contents of the item will
// be error-free.
// In no respect shall Feabhas incur any liability for any damages, including,
// but limited to, direct, indirect, special, or consequential damages arising
// out of, resulting from, or any way connected to the use of the item, whether
// or not based upon warranty, contract, tort, or otherwise; whether or not
// injury was sustained by persons or property or otherwise; and whether or not
// loss was sustained from, or arose out of, the results of, the item, or any
// services that may be provided by Feabhas.
// -----------------------------------------------------------------------------

#include <cstdint>
#include "cmsis_device.h"

static void sleep(uint32_t t);

// constexpr uint32_t GPIOD_BASE {0x40020C00u};

static volatile uint32_t *const AHB1_enable {reinterpret_cast<uint32_t*>(0x40023830u)};
static volatile uint32_t *const GPIOD_mode_r {reinterpret_cast<uint32_t*>(GPIOD_BASE)};
static volatile uint32_t *const GPIOD_out_r {reinterpret_cast<uint32_t*>(GPIOD_BASE + 0x14u)};
static volatile const uint32_t *const GPIOD_in_r {reinterpret_cast<uint32_t*>(GPIOD_BASE + 0x10u)};

inline void set(volatile uint32_t *const port, uint32_t pattern) 
{
    uint32_t value = *port;
    value |= pattern; 
    *port = value;
}

inline void clear(volatile uint32_t *const port, uint32_t pattern) 
{
    uint32_t value = *port;
    value &= ~pattern; 
    *port = value;
}

constexpr int led_start {8};
constexpr int led_end {11};

int main()
{
    uint32_t value {};
    value = *AHB1_enable;
    value |= (0x1u << 3); 
    *AHB1_enable = value;

    value = *GPIOD_mode_r;
    value &= ~(0b11111111u << led_start*2); 
    *GPIOD_mode_r = value;
    
    value = *GPIOD_mode_r;
    value |= 0b01010101u << led_start*2; 
    *GPIOD_mode_r = value;

    unsigned int led = led_start;
    while(true)
    {
        clear(GPIOD_out_r, 0xFu << led_start);
        set(GPIOD_out_r, 0x1u << led);
        sleep(1000);
        led = (led != led_end) ? led + 1 : led_start;
    }
}

static constexpr uint32_t TIMER_FREQUENCY_HZ {1000u};

static uint32_t delay_count;

static void sleep(uint32_t t)
{
    SysTick_Config(SystemCoreClock / TIMER_FREQUENCY_HZ);
    delay_count = t;

    // Busy wait until the SysTick decrements the counter to zero.
    while (delay_count != 0)
    {
        __WFE();
    }
}

// -----------------------------------------------------------------------------
// CMSIS ISR Handler.  This function uses weak linkage
// to override the default ISR handler (provided by CMSIS)
//
extern "C"
void SysTick_Handler(void)
{
    // Decrement to zero the counter used by the delay routine.
    if (delay_count != 0)
    {
        --delay_count;
    }
}
