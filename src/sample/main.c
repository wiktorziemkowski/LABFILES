#include "stuff.h"
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrf_gpio.h>
#include <nrfx_uarte.h>
#include <nrfx_systick.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <nrfx_rtc.h>

nrfx_uarte_t uarte_instance = NRFX_UARTE_INSTANCE(0);
const nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);

int button_status(void)
{
    if (nrf_gpio_pin_read(BUTTON1) == 0) return 1;
    
    return 0;
}

void main()
{
    init_stuff();
    int last1 = 1;

    while (1)
    {
        nrfx_systick_delay_ms(10);


        int b1 = nrf_gpio_pin_read(BUTTON1);
        if (last1 == 1 && b1 == 0)
        {
            nrf_gpio_pin_toggle(LED1);
        }
        last1 = b1;
}
}